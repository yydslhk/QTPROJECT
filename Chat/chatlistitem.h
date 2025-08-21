#ifndef CHATLISTITEM_H
#define CHATLISTITEM_H
#include "ch.h"
#include <QWidget>
#include <QPixmap>

class QLabel;

class ChatListItem : public QWidget
{
    Q_OBJECT
public:
    explicit ChatListItem(QWidget *parent = nullptr);

    // 设置接口
    void setAvatar(const QPixmap &pixmap);
    void setAvatar(const QString &imagePath);
    void setUsername(const QString &name);
    void setLastMessage(const QString &message);
    void setTimestamp(const QString &time);
    void setUnreadCount(int count);
signals:
    // 点击信号
    void itemClicked(const QString &userId);
protected:
    // 重写鼠标事件
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    void initUI();

    QLabel *m_avatarLabel;
    QLabel *m_usernameLabel;
    QLabel *m_lastMsgLabel;
    QLabel *m_timeLabel;
    QLabel *m_unreadLabel;

    QString m_userId;  // 存储用户ID
    bool m_pressed;     // 记录按下状态
};

#endif // CHATLISTITEM_H
