#ifndef MYCHAT_H
#define MYCHAT_H
#include "ch.h"
#include <QWidget>
#include "chatlistmanager.h"
#include "basewindow.h"
#include <QFileInfo>

namespace Ui {
class MyChat;
}

class MyChat : public BaseWindow
{
    Q_OBJECT

public:
    explicit MyChat(QWidget *parent = nullptr);
    ~MyChat();
    void AddChatItem(const QString &userId,
                     const QString &username,
                     const QString &lastMessage,
                     const QString &avatarPath,
                     int unreadCount);
    bool MkDir(QString path);
    void SetName(QString name);
    void addtest();
    static bool compareByLastModified(const QFileInfo &a, const QFileInfo &b);
    QFileInfoList listSubdirsSortedByTime(const QString &folderPath);
private slots:
    void on_pushButton_clicked();

private:
    Ui::MyChat *ui;
    void loadhistory();
    QString name;
};

#endif // MYCHAT_H
