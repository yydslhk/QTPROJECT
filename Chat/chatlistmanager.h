#ifndef CHATLISTMANAGER_H
#define CHATLISTMANAGER_H
#include "ch.h"
#include <QListWidget>
#include "chatlistitem.h"
#include <unordered_map>
#include <memory>
#include <map>

class ChatListManager : public QListWidget
{
    Q_OBJECT
public:
    explicit ChatListManager(QWidget *parent = nullptr);

    // 添加新聊天项
    QListWidgetItem* addChatItem(const QString &userId,
                               const QString &username,
                               const QString &lastMessage,
                               const QString &avatarPath = "",
                               int unreadCount = 0);

    // 更新已有聊天项
    void updateChatItem(const QString &userId,
                       const QString &lastMessage,
                       int unreadCount = 0);

    // 设置头像
    void setAvatar(const QString &userId, const QString &avatarPath);
    void setAvatar(const QString &userId, const QPixmap &avatar);

    // 设置用户名
    void setUsername(const QString &userId, const QString &username);

    // 设置最后消息
    void setLastMessage(const QString &userId, const QString &message);

    // 设置未读消息数
    void setUnreadCount(const QString &userId, int count);

    std::shared_ptr<ChatListItem> GetItem(const QString& userid);
signals:
    void chatItemClicked(const QString &userId);

private slots:
    void onItemClicked(const QString &userId);
private:
    QListWidgetItem* findItem(const QString &userId);
    ChatListItem* getItemWidget(QListWidgetItem *item);
    std::map<QString, std::shared_ptr<ChatListItem>> map;
};

#endif // CHATLISTMANAGER_H
