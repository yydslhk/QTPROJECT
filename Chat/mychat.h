#ifndef MYCHAT_H
#define MYCHAT_H
#include "ch.h"
#include <QWidget>
#include "chatlistmanager.h"
#include "basewindow.h"
#include <QFileInfo>
#include <QDateTime>
struct FolderInfo
{
    QString name;
    QString path;
    QDateTime lastModified;
};
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
    static bool compareByLastModified(const FolderInfo &a, const FolderInfo &b);

    QList<FolderInfo> listSubdirsSortedByTime(const QString &folderPath);
    QStringList listFilesInDirectory(const QString &dirPath);
    void loadhistory();
    void Chat2AI();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MyChat *ui;

    QString name;
    QString path = "MssageHistory/";
};

#endif // MYCHAT_H
