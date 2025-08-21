#include "user.h"
#include<QDebug>
#include<QFile>
#include <QDir>
#include <QDateTime>
User::User()
{

}

User::User(QString name, QString avatar)
{
    this->_name = name;
    this->_avatar = avatar;
}

bool User::AddHistory(const char *data, QString Owener)
{
    QString filepath = "MssageHistory/" + Owener +"/" + _name;
    MkDir(filepath + "/" + QDateTime::currentDateTime().toString("yyyy-MM-dd"));
    QFile file(filepath);

    // 以追加模式打开文件
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        qDebug() << "无法打开文件:" << file.errorString();
        return false;
    }

    qint64 bytesWritten = file.write(data);

    file.close();
    return bytesWritten != -1;
}

bool User::LoadHistory(QString name)
{
    QString path = "MessageHistory/" + name + "/" +_name + ".txt";
    parseFile(path);
    return true;
}

void User::parseFile(const QString filepath)
{
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

bool User::parseLine(const QString &line, frontdata &message) {
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

QString User::GetName()
{
    return _name;
}

bool User::MkDir(QString path)
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
