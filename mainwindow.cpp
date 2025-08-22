#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "NetWork/networkmanager.h"
#include <QJsonDocument>
#include <QNetworkReply>
#include <QDebug>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    home_page = std::make_shared<HomePage>();
    home_page->hide();
    chat = std::make_shared<MyChat>();
    chat->hide();
}

MainWindow::~MainWindow()
{
    LogOut();
    delete ui;
}


void MainWindow::on_login_clicked()
{
    QJsonObject obj;
    obj["username"] = ui->user_name->text();
    obj["password"] = ui->password->text();
    qDebug() << obj["username"].toString() << " " << obj["password"].toString();
    if(ui->user_name->text().size() == 0 || ui->password->text().size() == 0) return;
    auto reply = NetWorkManager::getinstance()->SendForm("/login", obj);
    connect(reply, &QNetworkReply::finished, this, [=]()
    {
        QByteArray data = reply->readAll();

        if(data.size() > 0)
        {
            auto doc = QJsonDocument::fromJson(data);
            auto obj = doc.object();
            NetWorkManager::getinstance()->SetUUID(obj["token"].toString());
            qDebug() << obj["token"].toString() << obj["result"].toString();
            if(obj["result"] == 1)
            {
                this->hide();
                uuid = obj["token"].toString();
                username = obj["username"].toString();
                chat->SetName(username);

                chat->loadhistory();
                chat->show();
                home_page->show();
            }
            else
            {
                qDebug() << "用户名或密码不正确";
            }
        }
        else
        {
            qDebug() << "服务器没有响应";
        }
    });
}

void MainWindow::on_user_register_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButton_clicked()
{
    QJsonObject obj;
    obj["username"] = ui->username2->text();
    obj["password"] = ui->password1->text();
    obj["password"] = ui->password2->text();
    obj["phoneNum"] = ui->phoneNum->text();

    if(ui->username2->text().isEmpty() || ui->password1->text().isEmpty() || ui->password2->text().isEmpty() || ui->password1->text()!=ui->password2->text())
    {
        return;
    }
    auto reply = NetWorkManager::getinstance()->SendForm("/register", obj);
    connect(reply, &QNetworkReply::finished, this, [=]()
    {
        QByteArray data = reply->readAll();
        auto doc = QJsonDocument::fromJson(data);
        auto obj2 = doc.object();
        qDebug() << obj2["token"].toString() << obj2["result"].toString();

        if(obj2["result"] == 1)
        {
            qDebug() << "注册成功";
            ui->stackedWidget->setCurrentIndex(0);
        }
        else
        {
            qDebug() << "注册失败";
        }
    });
}

void MainWindow::LogOut()
{
    QJsonObject obj;
    obj["message"] = "logout";
    obj["uuid"] = uuid;
    obj["username"] = username;

    auto reply = NetWorkManager::getinstance()->SendForm("/logout", obj);
}
