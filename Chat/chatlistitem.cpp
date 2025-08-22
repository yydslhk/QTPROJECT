#include "chatlistitem.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDateTime>
#include <QMouseEvent>
#include <QDir>
#include <QDebug>

ChatListItem::ChatListItem(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void ChatListItem::initUI()
{
    // 头像标签
    m_avatarLabel = new QLabel(this);
    m_avatarLabel->setFixedSize(40, 40);
    m_avatarLabel->setScaledContents(true);
    m_avatarLabel->setStyleSheet("border-radius: 20px;");

    // 用户名标签
    m_usernameLabel = new QLabel(this);
    m_usernameLabel->setStyleSheet("font-weight: bold; color: #333;");

    // 最后消息标签
    m_lastMsgLabel = new QLabel(this);
    m_lastMsgLabel->setStyleSheet("color: #888; font-size: 12px;");
    m_lastMsgLabel->setWordWrap(true);
    m_lastMsgLabel->setMaximumWidth(180);

    // 时间标签
    m_timeLabel = new QLabel(this);
    m_timeLabel->setStyleSheet("color: #999; font-size: 11px;");
    m_timeLabel->setAlignment(Qt::AlignRight | Qt::AlignTop);

    // 未读消息标签
    m_unreadLabel = new QLabel(this);
    m_unreadLabel->setFixedSize(18, 18);
    m_unreadLabel->setAlignment(Qt::AlignCenter);
    m_unreadLabel->setStyleSheet("background-color: #f44336; color: white; border-radius: 9px; font-size: 10px;");

    // 右侧布局（用户名和最后消息）
    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->setSpacing(2);
    rightLayout->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *topRightLayout = new QHBoxLayout();
    topRightLayout->addWidget(m_usernameLabel);
    topRightLayout->addStretch();
    topRightLayout->addWidget(m_timeLabel);

    rightLayout->addLayout(topRightLayout);
    rightLayout->addWidget(m_lastMsgLabel);
    rightLayout->addStretch();

    // 主布局
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(10, 8, 10, 8);
    mainLayout->addWidget(m_avatarLabel);
    mainLayout->addLayout(rightLayout);
    mainLayout->addWidget(m_unreadLabel);

    // 设置背景样式
    setStyleSheet("ChatListItem { background-color: white; border-bottom: 1px solid #f0f0f0; }"
                  "ChatListItem:hover { background-color: #f5f5f5; }");

}

void ChatListItem::setAvatar(const QPixmap &pixmap)
{
    m_avatarLabel->setPixmap(pixmap.scaled(m_avatarLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void ChatListItem::setAvatar(const QString &imagePath)
{
    QPixmap pixmap(imagePath);
    if (pixmap.isNull()) {
        pixmap = QPixmap(":/images/default_avatar.png");
    }
    setAvatar(pixmap);
}

void ChatListItem::setUsername(const QString &name)
{
    m_usernameLabel->setText(name);
    m_username = name;
}

void ChatListItem::setLastMessage(const QString &message)
{
    QString elidedText = m_lastMsgLabel->fontMetrics().elidedText(
        message, Qt::ElideRight, m_lastMsgLabel->width());
    m_lastMsgLabel->setText(elidedText);
}

void ChatListItem::setTimestamp(const QString &time)
{
    m_timeLabel->setText(time);
}

void ChatListItem::setUnreadCount(int count)
{
    if (count <= 0) {
        m_unreadLabel->hide();
    } else {
        m_unreadLabel->setText(QString::number(count > 99 ? 99 : count));
        m_unreadLabel->show();
    }
}

void ChatListItem::SetUser(User *user)
{
    current_user = user;
}

void ChatListItem::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_pressed = true;
    }
    QWidget::mousePressEvent(event);
}

void ChatListItem::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_pressed && event->button() == Qt::LeftButton) {
        m_pressed = false;
        if (rect().contains(event->pos())) {
            emit itemClicked(m_username);
        }
    }
    QWidget::mouseReleaseEvent(event);
}


bool ChatListItem::AddHistory(const char *data, QString Owener)
{
    QString filepath = "MssageHistory/" + Owener +"/" + GetName();
    MkDir(filepath);
//    QString file = ;
    QFile file(filepath + "/" + QDateTime::currentDateTime().toString("yyyy-MM-dd") + ".txt");
    qDebug() << filepath + "/" + QDateTime::currentDateTime().toString("yyyy-MM-dd") + ".txt";
    QIODevice::OpenMode mode = file.exists() ?
        QIODevice::Append | QIODevice::Text :
        QIODevice::WriteOnly | QIODevice::Text;
    // 以追加模式打开文件
    if (!file.open(mode)) {
        qDebug() << "无法打开文件:" << file.errorString();
        return false;
    }

    qint64 bytesWritten = file.write(data);

    file.close();
    return bytesWritten != -1;
}

bool ChatListItem::LoadHistory(QString name)
{
    QString path = "MssageHistory/" + name + "/" + GetName();
    qDebug() << path;
    QFileInfoList list = getSortedTxtFiles(path);
    qDebug() << list.size();
    parseFile(path + "/" + list[0].fileName());
    return true;
}

void ChatListItem::parseFile(const QString filepath)
{
    qDebug() << "正在解析" <<filepath;
    QFile file(filepath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        frontdata message;
        if (parseLine(line, message)) {
            emit newMessageParsed(message);
        }
    }
    file.close();

}

bool ChatListItem::parseLine(const QString &line, frontdata &message) {
    QStringList parts = line.split("@");
    if (parts.size() < 5) {
        return false;
    }

    // 解析 isMe 字段
    message.isMe = (parts[0] == "1");

    // 解析内容
    message.content = parts[1];
    message.sender = parts[2];
    message.avatar = parts[3];

    // 解析时间戳
    QString timeStr = parts[4];
    message.userid = "user";
    message.timestamp = QDateTime::fromString(timeStr, "yyyy-MM-dd hh:mm:ss.zzz");
    if (!message.timestamp.isValid()) {
        return false;
    }

    return true;
}

QString ChatListItem::GetName()
{
    return current_user->GetName();
}

bool ChatListItem::MkDir(QString path)
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

QStringList ChatListItem::listFilesInDirectory(const QString &dirPath) {
    QDir directory(dirPath);
    qDebug() << dirPath;
    // 设置过滤器：只获取文件（排除子目录）
    QStringList filters;
    filters << "*.txt"; // 匹配所有文件（可根据需求修改，如 "*.txt"）

    // 获取文件列表
    QStringList files = directory.entryList(filters, QDir::Files | QDir::NoDotAndDotDot);

    // 打印文件列表
    return files;
}

QFileInfoList ChatListItem::getSortedTxtFiles(const QString &directoryPath) {
    QDir dir(directoryPath);
    if (!dir.exists()) {
        qDebug() << "Directory does not exist!";
        return QFileInfoList();  // 返回空列表
    }
    qDebug() << directoryPath;
    // 设置过滤器，获取所有的 .txt 文件
    dir.setNameFilters(QStringList() << "*.txt");
    dir.setFilter(QDir::Files | QDir::NoSymLinks);

    // 获取文件信息列表
    QFileInfoList fileList = dir.entryInfoList();

    // 按修改时间排序
    std::sort(fileList.begin(), fileList.end(), [](const QFileInfo &a, const QFileInfo &b) {
        return a.lastModified() > b.lastModified();
    });

    return fileList;
}
