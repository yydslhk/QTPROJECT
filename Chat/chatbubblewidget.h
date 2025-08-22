#ifndef CHATBUBBLEWIDGET_H
#define CHATBUBBLEWIDGET_H
#include "ch.h"
#include <QWidget>
#include <QListView>
#include <QStandardItemModel>
#include "chatbubbledelegate.h"
#include "chatlistitem.h"
#include "user.h"

class ChatBubbleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChatBubbleWidget(QWidget *parent = nullptr);

    // 添加消息接口
    void addMessage(const QString &content, bool isMe,
                   const QString &sender = "",
                   const QString &avatar = "", bool isadd = true);

    // 批量添加消息
    void addMessages(const QList<QPair<QString, bool>> &messages,
                    const QString &sender = "",
                    const QString &avatar = "");

    // 清空所有消息
    void clearMessages();

    bool AppendToFile(const QString& filepath, const char* data);

    void SetCurrentUser(User* user);

    void SetMe(User* user);

    void SetCurrentItem(ChatListItem* item);

private:
    void initUI();

    QListView *m_messageListView;
    QStandardItemModel *m_messageModel;
    ChatBubbleDelegate *m_bubbleDelegate;
    ChatListItem* current_item;
    User* current_user;
    User* Me;
};

#endif // CHATBUBBLEWIDGET_H
