#include "virtnosiswindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VirtNosisWindow w;
    w.show();

    return a.exec();
}
