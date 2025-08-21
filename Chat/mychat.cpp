#include "mychat.h"
#include "ui_mychat.h"
#include <QDebug>
#include <QDir>
#include "user.h"
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
    ui->listWidget->addChatItem(userId, username, lastMessage, avatarPath, unreadCount);
    ChatListItem* item = new ChatListItem();
    item = ui->listWidget->GetItem(username).get();
    connect(item, &ChatListItem::itemClicked, this, [=](const QString& id)
    {
        User* newuser = new User(id, avatarPath);
        User* Me = new User(name, "");
        ui->message_history->SetCurrentUser(newuser);
        ui->message_history->clearMessages();
        newuser->LoadHistory(name);
        connect(newuser, &User::newMessageParsed, this, [this](frontdata data)
        {
            this->ui->message_history->addMessage(data.content, data.isMe, data.sender, data.avatar, true);
        });
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
}

void MyChat::addtest()
{
    ui->message_history->SetCurrentUser(new User("张三", ":/avatars/user1.png"));
    ui->message_history->SetMe(new User(name, ":/avatars/user1.png"));
    AddChatItem("user1", "deepseek", "你好！", ":/images/user1.png", 1);
    ui->message_history->addMessage("你好！", false, "张三", ":/avatars/user1.png", false);
}

void MyChat::on_pushButton_clicked()
{
    ui->message_history->addMessage(ui->message->toPlainText(), true, name, ":/avatars/user1.png", false);
    ui->message->clear();
}

void MyChat::loadhistory()
{
    // TODO: 给每个用户创建一个文件用来保存记录,每次点击都加载文件。
    // 1. 加载当前界面左侧的历史
    QFileInfoList dirlist = listSubdirsSortedByTime("MessageHistory/"+name);
    // 2. 得到文件夹下面的所有子文件夹，按修改时间排列，依次添加到左侧的框，
    for(auto dirInfo :dirlist)
    {
        QFileInfoList list = dirInfo.dir().entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    }
//    AddChatItem("user1", "");
}

bool MyChat::compareByLastModified(const QFileInfo &a, const QFileInfo &b) {
    return a.lastModified() > b.lastModified();
}

QFileInfoList MyChat::listSubdirsSortedByTime(const QString &folderPath) {
    QDir dir(folderPath);

    // 设置过滤器：只获取子目录（不包含文件）
    QFileInfoList dirList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    if (dirList.isEmpty()) {
        qDebug() << "文件夹中没有子目录！";
        return dirList;
    }

    // 按修改时间排序（最新到最老）
    std::sort(dirList.begin(), dirList.end(), compareByLastModified);

    // 输出排序后的子目录列表
    return dirList;
}
