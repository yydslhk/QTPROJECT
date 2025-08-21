#include "chatbubbledelegate.h"
#include <QPainter>
#include <QFontMetrics>
#include <QDateTime>

ChatBubbleDelegate::ChatBubbleDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

void ChatBubbleDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);

    // 获取数据
    QString content = index.data(Qt::DisplayRole).toString();
    bool isMe = index.data(Qt::UserRole + 1).toBool();
    QString sender = index.data(Qt::UserRole + 2).toString();
    QPixmap avatar = index.data(Qt::UserRole + 3).value<QPixmap>();
    QDateTime time = index.data(Qt::UserRole + 4).toDateTime();

    // 计算文本区域
    int maxBubbleWidth = option.rect.width() * 0.7; // 气泡最大宽度
    QFontMetrics fm(option.font);
    QRect textRect = fm.boundingRect(QRect(0, 0, maxBubbleWidth - 20, 1000),
                                   Qt::TextWordWrap, content);

    // 计算气泡位置和大小
    int bubbleWidth = textRect.width() + 20;
    int bubbleHeight = textRect.height() + 15;
    int bubbleY = option.rect.y() + 5;
    int bubbleX;

    if (isMe) {
        // ====== 右侧气泡（自己发送的消息） ======
        bubbleX = option.rect.width() - bubbleWidth - 10;

        // 绘制气泡背景
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor(142, 200, 86)); // 微信绿色
        painter->drawRoundedRect(QRect(bubbleX, bubbleY, bubbleWidth, bubbleHeight), 8, 8);

        // 绘制文本
        painter->setPen(Qt::white);
        painter->drawText(QRect(bubbleX + 10, bubbleY + 5, textRect.width(), textRect.height()),
                        Qt::AlignLeft | Qt::TextWordWrap, content);

        // 绘制时间（右下角）
        painter->setPen(QColor(220, 220, 220));
        painter->setFont(QFont(option.font.family(), 8));
        QString timeStr = time.toString("yy:mm:dd:hh:mm:ss");
        painter->drawText(QRect(bubbleX, bubbleY + bubbleHeight + 2, bubbleWidth, 15),
                        Qt::AlignRight | Qt::AlignTop, timeStr);
    } else {
        // ====== 左侧气泡（他人发送的消息） ======
        bubbleX = 50; // 留出头像空间

        // 绘制头像
        if (!avatar.isNull()) {
            painter->drawPixmap(10, bubbleY, 35, 35,
                              avatar.scaled(35, 35, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }

        // 绘制气泡背景
        painter->setPen(QColor(220, 220, 220));
        painter->setBrush(Qt::white);
        painter->drawRoundedRect(QRect(bubbleX, bubbleY, bubbleWidth, bubbleHeight), 8, 8);

        // 绘制发送者名称（左上角）
        if (!sender.isEmpty()) {
            painter->setPen(QColor(100, 100, 100));
            painter->setFont(QFont(option.font.family(), option.font.pointSize() - 1, QFont::Bold));
            painter->drawText(QRect(bubbleX, bubbleY - 15, bubbleWidth, 15),
                            Qt::AlignLeft | Qt::AlignVCenter, sender);
        }

        // 绘制文本
        painter->setPen(Qt::black);
        painter->drawText(QRect(bubbleX + 10, bubbleY + 5, textRect.width(), textRect.height()),
                        Qt::AlignLeft | Qt::TextWordWrap, content);

        // 绘制时间（左下角）
        painter->setPen(QColor(150, 150, 150));
        painter->setFont(QFont(option.font.family(), 8));
        QString timeStr = time.toString("hh:mm");
        painter->drawText(QRect(bubbleX, bubbleY + bubbleHeight + 2, bubbleWidth, 15),
                        Qt::AlignLeft | Qt::AlignTop, timeStr);
    }

    painter->restore();
}

QSize ChatBubbleDelegate::sizeHint(const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const
{
    QString content = index.data(Qt::DisplayRole).toString();
    bool isMe = index.data(Qt::UserRole + 1).toBool();

    int maxBubbleWidth = option.rect.width() * 0.7;
    QFontMetrics fm(option.font);
    QRect textRect = fm.boundingRect(QRect(0, 0, maxBubbleWidth - 20, 1000),
                                   Qt::TextWordWrap, content);

    // 气泡高度 + 时间高度 + 间距 + 发送者名称高度（如果是他人消息）
    int height = textRect.height() + 35;
    if (!isMe && !index.data(Qt::UserRole + 2).toString().isEmpty()) {
        height += 15; // 发送者名称额外高度
    }

    return QSize(option.rect.width(), height);
}

QRect ChatBubbleDelegate::calculateTextRect(const QString &text, const QFont &font, int maxWidth) const
{
    QFontMetrics fm(font);
    return fm.boundingRect(QRect(0, 0, maxWidth, 1000), Qt::TextWordWrap, text);
}

void ChatBubbleDelegate::drawSenderBubble(QPainter *painter, const QRect &rect,
                                         const QString &text, const QPixmap &avatar) const
{
    // 绘制气泡背景
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(155, 220, 100)); // 微信绿色
    painter->drawRoundedRect(rect, 8, 8);

    // 绘制文本
    painter->setPen(Qt::black);
    painter->drawText(rect.adjusted(10, 10, -10, -10), Qt::TextWordWrap, text);
}

void ChatBubbleDelegate::drawReceiverBubble(QPainter *painter, const QRect &rect,
                                           const QString &text, const QPixmap &avatar) const
{
    // 绘制头像
    if (!avatar.isNull()) {
        painter->drawPixmap(10, rect.y() + 5, 30, 30,
                          avatar.scaled(30, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    // 绘制气泡背景
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::white);
    painter->drawRoundedRect(rect, 8, 8);

    // 绘制文本
    painter->setPen(Qt::black);
    painter->drawText(rect.adjusted(10, 10, -10, -10), Qt::TextWordWrap, text);
}
