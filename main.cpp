#include <QApplication>

#include "menu/menu.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Menu w;
    w.addModule("主页");

    w.addModule("文件管理");
    w.addFunc("文件管理", "文件上传");
    w.addFunc("文件管理", "文件审核");
    w.addModule("音响管理");
    w.addFunc("音响管理", "分组管理");


    w.show();

    return a.exec();
}
