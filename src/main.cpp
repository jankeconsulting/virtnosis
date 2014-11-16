/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#include <QApplication>
#include "./virtnosiswindow.h"

/**
 * @brief Main function of the application
 * @param argc - number of arguments
 * @param argv - list of arguments
 * @return error code of the application
 *
 * This function creates a QApplication object
 * and sets the application wide information.
 *
 * It then creates the main window (VirtnosisWindow),
 * makes it visibles and calles the main event
 * loop of the application
 */
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
