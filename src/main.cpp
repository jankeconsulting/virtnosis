/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#include "virtnosiswindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("Janke Consulting");
    a.setOrganizationDomain("jankeconsulting.ca");
    a.setApplicationName("Virtnosis");
    a.setApplicationVersion("1.0");
    VirtnosisWindow w;
    w.show();

    return a.exec();
}
