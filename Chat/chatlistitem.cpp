#include "chatlistitem.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDateTime>
#include <QMouseEvent>

ChatListItem::ChatListItem(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void ChatListItem::initUI()
{
    // 头像标签
    m_avatarLabel = new QLabel(this);
    m_avatarLabel->setFixedSize(40, 40);
    m_avatarLabel->setScaledContents(true);
    m_avatarLabel->setStyleSheet("border-radius: 20px;");

    // 用户名标签
    m_usernameLabel = new QLabel(this);
    m_usernameLabel->setStyleSheet("font-weight: bold; color: #333;");

    // 最后消息标签
    m_lastMsgLabel = new QLabel(this);
    m_lastMsgLabel->setStyleSheet("color: #888; font-size: 12px;");
    m_lastMsgLabel->setWordWrap(true);
    m_lastMsgLabel->setMaximumWidth(180);

    // 时间标签
    m_timeLabel = new QLabel(this);
    m_timeLabel->setStyleSheet("color: #999; font-size: 11px;");
    m_timeLabel->setAlignment(Qt::AlignRight | Qt::AlignTop);

    // 未读消息标签
    m_unreadLabel = new QLabel(this);
    m_unreadLabel->setFixedSize(18, 18);
    m_unreadLabel->setAlignment(Qt::AlignCenter);
    m_unreadLabel->setStyleSheet("background-color: #f44336; color: white; border-radius: 9px; font-size: 10px;");

    // 右侧布局（用户名和最后消息）
    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->setSpacing(2);
    rightLayout->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *topRightLayout = new QHBoxLayout();
    topRightLayout->addWidget(m_usernameLabel);
    topRightLayout->addStretch();
    topRightLayout->addWidget(m_timeLabel);

    rightLayout->addLayout(topRightLayout);
    rightLayout->addWidget(m_lastMsgLabel);
    rightLayout->addStretch();

    // 主布局
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(10, 8, 10, 8);
    mainLayout->addWidget(m_avatarLabel);
    mainLayout->addLayout(rightLayout);
    mainLayout->addWidget(m_unreadLabel);

    // 设置背景样式
    setStyleSheet("ChatListItem { background-color: white; border-bottom: 1px solid #f0f0f0; }"
                  "ChatListItem:hover { background-color: #f5f5f5; }");

}

void ChatListItem::setAvatar(const QPixmap &pixmap)
{
    m_avatarLabel->setPixmap(pixmap.scaled(m_avatarLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void ChatListItem::setAvatar(const QString &imagePath)
{
    QPixmap pixmap(imagePath);
    if (pixmap.isNull()) {
        pixmap = QPixmap(":/images/default_avatar.png");
    }
    setAvatar(pixmap);
}

void ChatListItem::setUsername(const QString &name)
{
    m_usernameLabel->setText(name);
}

void ChatListItem::setLastMessage(const QString &message)
{
    QString elidedText = m_lastMsgLabel->fontMetrics().elidedText(
        message, Qt::ElideRight, m_lastMsgLabel->width());
    m_lastMsgLabel->setText(elidedText);
}

void ChatListItem::setTimestamp(const QString &time)
{
    m_timeLabel->setText(time);
}

void ChatListItem::setUnreadCount(int count)
{
    if (count <= 0) {
        m_unreadLabel->hide();
    } else {
        m_unreadLabel->setText(QString::number(count > 99 ? 99 : count));
        m_unreadLabel->show();
    }
}

void ChatListItem::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_pressed = true;
    }
    QWidget::mousePressEvent(event);
}

void ChatListItem::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_pressed && event->button() == Qt::LeftButton) {
        m_pressed = false;
        if (rect().contains(event->pos())) {
            emit itemClicked(m_userId);
        }
    }
    QWidget::mouseReleaseEvent(event);
}
