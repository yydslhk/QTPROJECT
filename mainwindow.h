#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "ch.h"
#include <QMainWindow>
#include "SqlApplication/homepage.h"
#include <memory>
#include "Chat/mychat.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_login_clicked();

    void on_user_register_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    std::shared_ptr<HomePage> home_page;
    std::shared_ptr<MyChat> chat;
    void LogOut();
    QString uuid;
    QString username;
};
#endif // MAINWINDOW_H
