#include "mychat.h"
#include "ui_mychat.h"
#include <QDebug>
#include <QDir>
#include "user.h"
#include "NetWork/networkmanager.h"
#include <QNetworkReply>
#include <QJsonDocument>
#include <QListWidget>
#include <QThread>
#include <QtConcurrent/QtConcurrentRun>
MyChat::MyChat(QWidget *parent) :
    BaseWindow(parent),
    ui(new Ui::MyChat)
{
    ui->setupUi(this);

}

MyChat::~MyChat()
{
    delete ui;
}

void MyChat::AddChatItem(const QString &userId, const QString &username, const QString &lastMessage, const QString &avatarPath, int unreadCount)
{
    QListWidgetItem* ChatItem = ui->listWidget->addChatItem(userId, username, lastMessage, avatarPath, unreadCount);
    ChatListItem* item = qobject_cast<ChatListItem*>(ui->listWidget->itemWidget(ChatItem));
//    item = ui->listWidget->GetItem(username).get();
//    QThread* thread = new QThread();
    connect(item, &ChatListItem::newMessageParsed, this, [=](frontdata data)
    {
        QtConcurrent::run([this, data]() {
            // 在新线程中处理消息（非UI操作）
            ui->message_history->addMessage(data.content, data.isMe, data.sender, data.avatar, true);
        });
    });
    connect(item, &ChatListItem::itemClicked, this, [=](const QString& id)
    {
        ui->message_history->SetCurrentItem(item);
        ui->message_history->clearMessages();
        QtConcurrent::run([item, username]() {
            qDebug() << "click";
            item->LoadHistory(username);
        });
//        item->LoadHistory(name);
    });
}

bool MyChat::MkDir(QString path)
{
    QDir dir(path);

    // 目录已存在
    if (dir.exists()) {
        qDebug() << "Directory already exists:" << path;
        return true;
    }

    // 尝试递归创建目录
    if (dir.mkpath(".")) {
        qDebug() << "Directory created successfully:" << path;
        return true;
    }

    // 创建失败
    qWarning() << "Failed to create directory:" << path;
    return false;
}

void MyChat::SetName(QString name)
{
    this->name = name;
    ui->message_history->SetMe(new User(name, ":/avatars/user1.png"));
}

void MyChat::addtest()
{
    ui->message_history->SetCurrentUser(new User("张三", ":/avatars/user1.png"));
    ui->message_history->SetMe(new User(name, ":/avatars/user1.png"));
    AddChatItem("user1", "deepseek", "你好！", ":/images/user1.png", 1);
    ui->message_history->addMessage("你好！", false, "张三", ":/avatars/user1.png", false);
}

void MyChat::Chat2AI()
{
    if(ui->message->toPlainText().size() == 0)
    {
        return;
    }
    QJsonObject obj;
    obj["username"] = "user";
    obj["message"] = ui->message->toPlainText();
    auto reply = NetWorkManager::getinstance()->SendForm("/chat", obj);
    connect(reply, &QNetworkReply::finished, this, [=]()
    {
        QByteArray data = reply->readAll();
        if(data.size() > 0)
        {
            auto doc = QJsonDocument::fromJson(data);
            auto ret = doc.object();
            if(ret["result"].toInt() == 1)
            {
                ui->message_history->addMessage(ret["message"].toString(), false, "AI", "", false);
            }
        }
    });
}

void MyChat::on_pushButton_clicked()
{
    if(ui->message->toPlainText().size() == 0)
    {
        return;
    }
    Chat2AI();
    ui->message_history->addMessage(ui->message->toPlainText(), true, name, ":/avatars/user1.png", false);
    ui->message->clear();
}

void MyChat::loadhistory()
{
    // 1. 加载当前界面左侧的历史

    QList<FolderInfo> dirlist = listSubdirsSortedByTime(path+name);
    // 2. 得到文件夹下面的所有子文件夹，按修改时间排列，依次添加到左侧的框，
    for(auto dirInfo :dirlist)
    {
        AddChatItem("User", dirInfo.name, "", "", 0);
    }
}

bool MyChat::compareByLastModified(const FolderInfo &a, const FolderInfo &b) {
    return a.lastModified > b.lastModified;
}


QList<FolderInfo> MyChat::listSubdirsSortedByTime(const QString &folderPath) {
    QDir dir(folderPath);
    QList<FolderInfo> subfolders;
    if (dir.exists()) {
        qDebug() << "文件存在";
        dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);  // 只列出子文件夹，不包括"."和".."
        QFileInfoList folderList = dir.entryInfoList();
        qDebug() << "文件夹中的子文件夹的数量为" << folderList.size();

        for (const QFileInfo &folder : folderList) {
            FolderInfo folderInfo;
            folderInfo.path = folder.absoluteFilePath();
            folderInfo.lastModified = folder.lastModified();
            folderInfo.name = folder.fileName();
            subfolders.append(folderInfo);
        }

        std::sort(subfolders.begin(), subfolders.end(), compareByLastModified);  // 按修改时间排序
    }
    else
    {
        qDebug() << "文件夹不存在";
    }

    return subfolders;
}

QStringList MyChat::listFilesInDirectory(const QString &dirPath) {
    QDir directory(dirPath);

    // 设置过滤器：只获取文件（排除子目录）
    QStringList filters;
    filters << "*.txt"; // 匹配所有文件（可根据需求修改，如 "*.txt"）

    // 获取文件列表
    QStringList files = directory.entryList(filters, QDir::Files | QDir::NoDotAndDotDot);

    // 打印文件列表
    return filters;
}
