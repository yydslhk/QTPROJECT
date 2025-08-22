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

QString User::GetName()
{
    return _name;
}
//bool User::AddHistory(const char *data, QString Owener)
//{
//    QString filepath = "MssageHistory/" + Owener +"/" + _name;
//    MkDir(filepath);
////    QString file = ;
//    QFile file(filepath + "/" + QDateTime::currentDateTime().toString("yyyy-MM-dd") + ".txt");
//    qDebug() << filepath + "/" + QDateTime::currentDateTime().toString("yyyy-MM-dd") + ".txt";
//    QIODevice::OpenMode mode = file.exists() ?
//        QIODevice::Append | QIODevice::Text :
//        QIODevice::WriteOnly | QIODevice::Text;
//    // 以追加模式打开文件
//    if (!file.open(mode)) {
//        qDebug() << "无法打开文件:" << file.errorString();
//        return false;
//    }

//    qint64 bytesWritten = file.write(data);

//    file.close();
//    return bytesWritten != -1;
//}

//bool User::LoadHistory(QString name)
//{
//    QString path = "MssageHistory/" + name + "/" +_name;
//    qDebug() << path;
//    QFileInfoList list = getSortedTxtFiles(path);
//    qDebug() << list.size();
//    parseFile(path + "/" + list[0].fileName());
//    return true;
//}

//void User::parseFile(const QString filepath)
//{
//    qDebug() << "正在解析" <<filepath;
//    QFile file(filepath);
//    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//        return;
//    }

//    QTextStream in(&file);
//    while (!in.atEnd()) {
//        QString line = in.readLine().trimmed();
//        if (line.isEmpty()) continue;

//        frontdata message;
//        if (parseLine(line, message)) {
//            emit newMessageParsed(message);
//        }
//    }
//    file.close();

//}

//bool User::parseLine(const QString &line, frontdata &message) {
//    QStringList parts = line.split("@");
//    if (parts.size() < 5) {
//        return false;
//    }

//    // 解析 isMe 字段
//    message.isMe = (parts[0] == "1");

//    // 解析内容
//    message.content = parts[1];
//    message.sender = parts[2];
//    message.avatar = parts[3];

//    // 解析时间戳
//    QString timeStr = parts[4];
//    message.userid = "user";
//    message.timestamp = QDateTime::fromString(timeStr, "yyyy-MM-dd hh:mm:ss.zzz");
//    if (!message.timestamp.isValid()) {
//        return false;
//    }

//    return true;
//}

//QString User::GetName()
//{
//    return _name;
//}

//bool User::MkDir(QString path)
//{
//    QDir dir(path);

//    // 目录已存在
//    if (dir.exists()) {
//        qDebug() << "Directory already exists:" << path;
//        return true;
//    }

//    // 尝试递归创建目录
//    if (dir.mkpath(".")) {
//        qDebug() << "Directory created successfully:" << path;
//        return true;
//    }

//    // 创建失败
//    qWarning() << "Failed to create directory:" << path;
//    return false;
//}

//QStringList User::listFilesInDirectory(const QString &dirPath) {
//    QDir directory(dirPath);
//    qDebug() << dirPath;
//    // 设置过滤器：只获取文件（排除子目录）
//    QStringList filters;
//    filters << "*.txt"; // 匹配所有文件（可根据需求修改，如 "*.txt"）

//    // 获取文件列表
//    QStringList files = directory.entryList(filters, QDir::Files | QDir::NoDotAndDotDot);

//    // 打印文件列表
//    return files;
//}

//QFileInfoList User::getSortedTxtFiles(const QString &directoryPath) {
//    QDir dir(directoryPath);
//    if (!dir.exists()) {
//        qDebug() << "Directory does not exist!";
//        return QFileInfoList();  // 返回空列表
//    }
//    qDebug() << directoryPath;
//    // 设置过滤器，获取所有的 .txt 文件
//    dir.setNameFilters(QStringList() << "*.txt");
//    dir.setFilter(QDir::Files | QDir::NoSymLinks);

//    // 获取文件信息列表
//    QFileInfoList fileList = dir.entryInfoList();

//    // 按修改时间排序
//    std::sort(fileList.begin(), fileList.end(), [](const QFileInfo &a, const QFileInfo &b) {
//        return a.lastModified() > b.lastModified();
//    });

//    return fileList;
//}
