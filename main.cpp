#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Q_INIT_RESOURCE(rc);
    mainWindow w;
    w.setWindowIcon(QIcon(":rc/icon"));
    w.show();
    
    return a.exec();
}
