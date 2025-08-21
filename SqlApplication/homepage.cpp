#include "homepage.h"
#include "ui_homepage.h"
#include <QUrl>
#include "NetWork/networkmanager.h"
#include <QJsonDocument>
#include <QNetworkReply>
#include <QJsonArray>
#include <QVector>
#include <QStandardItemModel>
#include <QMap>
#include <QFile>
HomePage::HomePage(QWidget *parent) :
    BaseWindow(parent),
    ui(new Ui::HomePage)
{
    initTitleBar();
    ui->setupUi(this);
    web = std::make_shared<QWebSocket>();
    GetTableName();
    ui->tabWidget->setCurrentIndex(0);
    ui->connect_type->addItem("长连接");
    ui->connect_type->addItem("短连接");
    ui->connect_type->setCurrentIndex(1);
}

HomePage::~HomePage()
{
    delete ui;
}

void HomePage::GetTableName()
{
    QJsonObject obj;
    obj["request"] = "GetTableName";
    auto reply = NetWorkManager::getinstance()->SendForm("/GetTableName", obj);
    connect(reply, &QNetworkReply::finished, this, [=]()
    {
        QByteArray data = reply->readAll();
        auto doc = QJsonDocument::fromJson(data);
        auto obj = doc.object();
        qDebug() << obj;
        if(obj["result"] == 1)
        {
            // 成功
            auto r = obj["tables"];
            QJsonArray array = r.toArray();
            QStringList result;
            for (const QJsonValue &value : array) {
                ui->comboBox->addItem(value.toString());
            }
        }
        else
        {
            qDebug() << "获取表失败";
        }

    });
}

void HomePage::initTitleBar()
{
    m_titleBar->setTitleRoll();

    m_titleBar->setTitleIcon(":/Resources/titleicon.png");
    m_titleBar->setTitleContent(QStringLiteral("前行中的小猪-前行之路还需前行"));
    m_titleBar->setButtonType(MIN_MAX_BUTTON);
    m_titleBar->setTitleWidth(this->width());
}

void HomePage::on_connect_clicked()
{
    if(ui->connect_type->currentIndex() == 1)
    {
        // 短连接不需要使用websocket
        return ;
    }
    if(!web)
    {
        web->open(QUrl(ui->url->text().trimmed()));
        connect(web.get(), &QWebSocket::connected, this, [this](){
            ui->sendmessage->append("已经连接上" + web->peerAddress().toString());
            b_connect = true;
            ui->connect->setText("断开");
        });

        connect(web.get(), &QWebSocket::disconnected,this,[this]
        {
            ui->sendmessage->append("与" + web->peerAddress().toString() + "断开连接");
            b_connect = false;
            ui->connect->setText("连接");
        });
        connect(web.get(), SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(onError(QAbstractSocket::SocketError)));

        connect(web.get(), &QWebSocket::textMessageReceived, this, [this](const QString message)
        {
           ui->recvmessage->append(message);
        });
    }

    if(!b_connect)
    {
        web->open(QUrl(ui->url->text().trimmed()));
    }
    else
    {
        web->close();
        web->deleteLater();
        web = nullptr;
    }
}

void HomePage::on_send_clicked()
{
    if(ui->connect_type->currentIndex() == 1)
    {
        QString str = ui->message->text();
        QByteArray jsonBytes = str.toUtf8();
        auto reply = NetWorkManager::getinstance()->SendData(ui->url->text(), jsonBytes);
        connect(reply, &QNetworkReply::finished, this, [=]()
        {
            QByteArray data = reply->readAll();
            auto doc = QJsonDocument::fromJson(data);
            auto obj = doc.object();
            QString str = "";
            for (auto it = obj.begin(); it != obj.end(); ++it) {
                qDebug() << "key =" << it.key()
                         << ", value =" << it.value().toString();
                str = str + ("key = " + it.key() + ", value = " + it.value().toString() + "; ");
            }
            ui->recvmessage->append(str);
        });
        ui->sendmessage->append(ui->message->text());

    }
    else if(b_connect)
    {
        web->sendTextMessage(ui->message->text());
        ui->sendmessage->append(ui->message->text());
    }

}

void HomePage::on_pushButton_clicked()
{
    QJsonObject obj;
    obj["request"] = "GetTableData";
    obj["table"] = ui->comboBox->currentText();
    if(ui->comboBox->currentText().size() <= 0)
    {
        return;
    }
    auto reply = NetWorkManager::getinstance()->SendForm("/selectData", obj);
    connect(reply, &QNetworkReply::finished, this, [=]()
    {
        QByteArray data = reply->readAll();
        auto doc = QJsonDocument::fromJson(data);
        auto obj = doc.object();
        if(obj["result"] == 1 && obj["data"].isNull())
        {
            qDebug() << "数据为空";
            return ;
        }
        else if(obj["result"] == 1)
        {
            auto data = obj["data"];
            auto head = obj["head"];
            QJsonArray head_array = head.toArray();
            int col = head_array.size();

            QStringList headers;
            for (const auto value : head_array) {
                headers << value.toString();
            }
            QStandardItemModel* model = new QStandardItemModel(3, headers.size(), this); // 3行2列
            model->setHorizontalHeaderLabels(headers);      // 数据库表的表头
            QJsonArray array = data.toArray();
            int row = array.size();
            for(int i = 0; i< row; i++)
            {
//                QStringList datalist;
                for (int j = 0; j < headers.size(); j++) {
//                    datalist << array[i].toObject().value(headers[j]).toString();
                    model->setItem(i, j, new QStandardItem(array[i].toObject().value(headers[j]).toString()));
                }
            }

            // 填充数据
            ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
            ui->tableView->setAlternatingRowColors(true); // 交替行颜色
            ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection); // 单选模式
            ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked); // 双击编辑
            ui->tableView->resizeColumnsToContents(); // 调整列宽适应内容
            ui->tableView->resizeRowsToContents();
            ui->tableView->setModel(model);
        }
        else if(obj["result"] == 0)
        {
            qDebug() << "获取表数据失败";
        }

    });
}

void HomePage::on_pushButton_2_clicked()
{
    GetTableName();
}

void HomePage::on_connect_type_currentIndexChanged(int index)
{
    if(b_connect && index == 1)
    {
        web->close();
        web->deleteLater();
        web = nullptr;
        ui->connect->setText("连接");
        b_connect = false;
    }
}
