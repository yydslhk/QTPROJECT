#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <memory>
#include <QMutex>
#include <string>
#include <QJsonObject>
#include <QTimer>
#include "ch.h"
class NetWorkManager : public QObject
{
    Q_OBJECT
public:
    static NetWorkManager* getinstance();                                                      // 获取单例模式
    QNetworkReply* SendForm(QString adress, QJsonObject data);                                           // 发送表单数据
    QNetworkReply* SendData(QString adress, QByteArray data);
    void SetUUID(QString );
    ~NetWorkManager();
private slots:
    void SendHeart();
private:
    NetWorkManager();
    bool LoadConfig();
    QString _url;
    int _port;
    QString Uuid;
    std::shared_ptr<QTimer> timer;
    static NetWorkManager* minstance;
    std::shared_ptr<QNetworkAccessManager> manager;             // 用来处理程序中涉及到的表单数据，只声明一个，但是会存在长连接的情况，到时候再考虑。
};

#endif // NETWORKMANAGER_H
