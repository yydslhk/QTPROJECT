#ifndef USER_H
#define USER_H

#include <QString>
#include <QDateTime>
#include <QMetaType>
#include <QObject>

struct frontdata
{
    bool isMe;          // 是否为本人发送
    QString content;    // 消息内容
    QString sender;     // 发送者
    QString avatar;     // 头像
    QDateTime timestamp;// 时间戳
    QString userid;
};
Q_DECLARE_METATYPE(frontdata);
class User : public QObject
{
    Q_OBJECT
public:
    User();
    User(QString name, QString avatar);
    bool AddHistory(const char* data, QString Owner);
    bool LoadHistory(QString name);
    void parseFile(const QString filepath);
    bool parseLine(const QString &line, frontdata &message);
    QString GetName();
    bool MkDir(QString path);
signals:
    void newMessageParsed(frontdata);
private:
    QString _name;
    QString _avatar; //头像

};

#endif // USER_H
