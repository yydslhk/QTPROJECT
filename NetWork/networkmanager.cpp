#include "networkmanager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QDebug>
NetWorkManager* NetWorkManager::minstance;

NetWorkManager* NetWorkManager::getinstance()
{
    minstance = new NetWorkManager();
    return minstance;
}

QNetworkReply* NetWorkManager::SendForm( QString address, QJsonObject data)
{
    data["token"] = Uuid;
    QNetworkRequest request;
    QString ip = "http://" + _url + ":" + QString::number(_port) + address;
    qDebug() << ip;
    request.setUrl(ip);

    request.setHeader(QNetworkRequest::ContentTypeHeader,
                       QVariant("application/json"));

    QNetworkReply *reply = manager->post(request, QJsonDocument(data).toJson());
    return reply;
}

QNetworkReply *NetWorkManager::SendData(QString address, QByteArray data)
{
    QNetworkRequest request;
    QString ip = "http://" + _url + ":" + QString::number(_port) + address;
    qDebug() << ip;
    request.setUrl(ip);

    request.setHeader(QNetworkRequest::ContentTypeHeader,
                       QVariant("application/json"));

    QNetworkReply *reply = manager->post(request, data);
    return reply;
}

void NetWorkManager::SetUUID(QString uuid)
{
    Uuid = uuid;
}

NetWorkManager::~NetWorkManager()
{
    delete minstance;
}

NetWorkManager::NetWorkManager()
{
    LoadConfig();
    manager = std::make_shared<QNetworkAccessManager>();
    timer = std::make_shared<QTimer>();


    // 连接 timeout 信号到槽函数
    connect(timer.get(), &QTimer::timeout, this, &NetWorkManager::SendHeart);

    // 每 1000 毫秒（1 秒）执行一次
//    timer->start(1000);
}

bool NetWorkManager::LoadConfig()
{
    QFile file("./config.json");
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "文件打开失败";
        return false;
    }
    qDebug() << "文件打开成功";
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject obj = doc.object();
    _url = obj["server_url"].toString();
    _port = obj["port"].toInt();
    return true;
}

void NetWorkManager::SendHeart()
{
    QJsonObject obj;
    auto reply = SendForm("/heart", obj);
    connect(reply, &QNetworkReply::finished, this, []()
    {
        qDebug() << "收到心跳包";
    });
}
