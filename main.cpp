#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
int main(int argc, char *argv[])
{


// 在main函数开始处
//    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    QApplication a(argc, argv);
    QFile file(":/style.qss");   // 可以放在资源文件里（推荐）
    if (file.open(QFile::ReadOnly)) {
        QTextStream in(&file);
        // 根据文件实际编码设置
        in.setCodec("UTF-8");  // 或 "GBK"
        QString styleSheet = in.readAll();
        file.close();
        qApp->setStyleSheet(styleSheet);
    } else {
        qDebug() << "open failed:" << file.errorString();
    }
    MainWindow w;
    w.show();
    return a.exec();
}
