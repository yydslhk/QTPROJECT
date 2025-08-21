#ifndef HOMEPAGE_H
#define HOMEPAGE_H
#include <QWebSocket>
#include <memory>
#include <QWidget>
#include "titlebar.h"
#include "basewindow.h"

namespace Ui {
class HomePage;
}

class HomePage : public BaseWindow
{
    Q_OBJECT
public:
    explicit HomePage(QWidget *parent = nullptr);
    ~HomePage();
private:
    void GetTableName();

    void initTitleBar();

private slots:
    void on_connect_clicked();

    void on_send_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_connect_type_currentIndexChanged(int index);

private:
    Ui::HomePage *ui;
    std::shared_ptr<QWebSocket> web;
    bool b_connect;
};

#endif // HOMEPAGE_H
