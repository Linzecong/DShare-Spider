#include "widget.h"
#include "mydb.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyDB* abc=new MyDB;
    Widget w;
    w.show();

    return a.exec();
}
