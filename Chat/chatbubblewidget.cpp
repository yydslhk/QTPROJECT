#include "chatbubblewidget.h"
#include <QVBoxLayout>
#include <QDebug>
#include <QDateTime>
#include <QFile>

ChatBubbleWidget::ChatBubbleWidget(QWidget *parent)
    : QWidget(parent)
{
    current_user = new User();
    Me = new User();
    initUI();
}


void ChatBubbleWidget::initUI()
{
    // 创建消息列表视图
    m_messageListView = new QListView(this);
    m_messageListView->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_messageListView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_messageListView->setStyleSheet("background-color: #f5f5f5; border: none;");

    // 创建消息模型
    m_messageModel = new QStandardItemModel(this);
    m_messageListView->setModel(m_messageModel);

    // 创建气泡委托
    m_bubbleDelegate = new ChatBubbleDelegate(this);
    m_messageListView->setItemDelegate(m_bubbleDelegate);

    // 设置布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_messageListView);
    layout->setContentsMargins(0, 0, 0, 0);
}

void ChatBubbleWidget::addMessage(const QString &content, bool isMe,
                                const QString &sender, const QString &avatar, bool isadd)
{
    QStandardItem *item = new QStandardItem(content);

    // 设置额外数据
    item->setData(isMe, Qt::UserRole + 1);      // 是否是我发送的消息
    item->setData(sender, Qt::UserRole + 2);    // 发送者名称
    item->setData(QPixmap(avatar), Qt::UserRole + 3);    // 头像
    item->setData(QDateTime::currentDateTime(), Qt::UserRole + 4); // 时间

    m_messageModel->appendRow(item);

    // 滚动到底部
    m_messageListView->scrollToBottom();

    if(!isadd)
    {
        // 是否已经添加到历史记录
        // 如果没有，将它添加进历史并保存文件。
        QString writedata = (isMe ? "1" : "0") + QString("@") + content + "@" + sender + "@" +
                                   avatar + "@" + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz") + "\r\n";
        current_user->AddHistory(writedata.toStdString().c_str(), Me->GetName());
        qDebug() << "写入的数据为" << writedata;
    }
}

void ChatBubbleWidget::addMessages(const QList<QPair<QString, bool>> &messages,
                                  const QString &sender, const QString &avatar)
{
    foreach (const auto &message, messages) {
        addMessage(message.first, message.second, sender, avatar);
    }
}

void ChatBubbleWidget::clearMessages()
{
    m_messageModel->clear();
}

bool ChatBubbleWidget::AppendToFile(const QString &filepath, const char *data)
{
    QFile file(filepath);

    // 以追加模式打开文件
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        qDebug() << "无法打开文件:" << file.errorString();
        return false;
    }

    qint64 bytesWritten = file.write(data);

    file.close();
    return bytesWritten != -1;
}

void ChatBubbleWidget::SetCurrentUser(User *user)
{
    this->current_user = user;
}

void ChatBubbleWidget::SetMe(User *user)
{
    Me = user;
}
