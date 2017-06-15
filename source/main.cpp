#include "mainwindow.h"
#include "category.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qRegisterMetaType<Category>();

    MainWindow w;

    if (a.arguments().size() > 1)
    {
        w.setFilename(a.arguments().at(1));
    }

    w.show();

    return a.exec();
}
