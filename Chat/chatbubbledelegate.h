#ifndef CHATBUBBLEDELEGATE_H
#define CHATBUBBLEDELEGATE_H
#include "ch.h"
#include <QStyledItemDelegate>

class ChatBubbleDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ChatBubbleDelegate(QObject *parent = nullptr);

    // 重写绘制方法
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    // 重写大小提示方法
    QSize sizeHint(const QStyleOptionViewItem &option,
                  const QModelIndex &index) const override;

private:
    // 计算文本绘制区域
    QRect calculateTextRect(const QString &text, const QFont &font, int maxWidth) const;

    // 绘制发送方气泡
    void drawSenderBubble(QPainter *painter, const QRect &rect,
                         const QString &text, const QPixmap &avatar) const;

    // 绘制接收方气泡
    void drawReceiverBubble(QPainter *painter, const QRect &rect,
                           const QString &text, const QPixmap &avatar) const;
};

#endif // CHATBUBBLEDELEGATE_H
