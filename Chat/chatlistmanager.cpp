#include "chatlistmanager.h"
#include <QDateTime>

ChatListManager::ChatListManager(QWidget *parent) : QListWidget(parent)
{
    // 设置列表样式
    setFrameShape(QFrame::NoFrame);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setStyleSheet("QListWidget { background-color: #f5f5f5; border: none; }"
                  "QListWidget::item { border: none; }");
}

QListWidgetItem* ChatListManager::addChatItem(const QString &userId,
                                              const QString &username,
                                              const QString &lastMessage,
                                              const QString &avatarPath,
                                              int unreadCount)
{
    // 创建自定义项
    QListWidgetItem *item = new QListWidgetItem(this);
    item->setData(Qt::UserRole, userId);
    item->setSizeHint(QSize(300, 60)); // 设置项大小

    map[username] = std::make_shared<ChatListItem>();
    map[username]->setUsername(username);
    map[username]->setLastMessage(lastMessage);
    map[username]->setAvatar(avatarPath);
    map[username]->setTimestamp(QDateTime::currentDateTime().toString("hh:mm"));
    map[username]->setUnreadCount(unreadCount);

    setItemWidget(item, map[username].get());
    addItem(item);
    return item;
}

void ChatListManager::updateChatItem(const QString &userId,
                                   const QString &lastMessage,
                                   int unreadCount)
{
    if (QListWidgetItem *item = findItem(userId)) {
        if (ChatListItem *widget = getItemWidget(item)) {
            widget->setLastMessage(lastMessage);
            widget->setTimestamp(QDateTime::currentDateTime().toString("hh:mm"));
            widget->setUnreadCount(unreadCount);
        }
    }
}

void ChatListManager::setAvatar(const QString &userId, const QString &avatarPath)
{
    if (QListWidgetItem *item = findItem(userId)) {
        if (ChatListItem *widget = getItemWidget(item)) {
            widget->setAvatar(avatarPath);
        }
    }
}

void ChatListManager::setAvatar(const QString &userId, const QPixmap &avatar)
{
    if (QListWidgetItem *item = findItem(userId)) {
        if (ChatListItem *widget = getItemWidget(item)) {
            widget->setAvatar(avatar);
        }
    }
}

void ChatListManager::setUsername(const QString &userId, const QString &username)
{
    if (QListWidgetItem *item = findItem(userId)) {
        if (ChatListItem *widget = getItemWidget(item)) {
            widget->setUsername(username);
        }
    }
}

void ChatListManager::setLastMessage(const QString &userId, const QString &message)
{
    if (QListWidgetItem *item = findItem(userId)) {
        if (ChatListItem *widget = getItemWidget(item)) {
            widget->setLastMessage(message);
            widget->setTimestamp(QDateTime::currentDateTime().toString("hh:mm"));
        }
    }
}

void ChatListManager::setUnreadCount(const QString &userId, int count)
{
    if (QListWidgetItem *item = findItem(userId)) {
        if (ChatListItem *widget = getItemWidget(item)) {
            widget->setUnreadCount(count);
        }
    }
}

std::shared_ptr<ChatListItem> ChatListManager::GetItem(const QString &username)
{
    return map[username];
}

void ChatListManager::onItemClicked(const QString &userId)
{
    emit chatItemClicked(userId);
}

QListWidgetItem* ChatListManager::findItem(const QString &userId)
{
    for (int i = 0; i < count(); ++i) {
        QListWidgetItem *item = this->item(i);
        if (item->data(Qt::UserRole).toString() == userId) {
            return item;
        }
    }
    return nullptr;
}

ChatListItem* ChatListManager::getItemWidget(QListWidgetItem *item)
{
    return qobject_cast<ChatListItem*>(itemWidget(item));
}
