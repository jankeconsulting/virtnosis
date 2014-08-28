/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#include "virtnosiswindow.h"
#include "ui_virtnosiswindow.h"

#include <QDebug>

VirtnosisWindow::VirtnosisWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VirtnosisWindow)
{
    ui->setupUi(this);
    QStandardItemModel *model = new QStandardItemModel(this);
    ui->domainView->setModel(model);
}

VirtnosisWindow::~VirtnosisWindow()
{
    delete ui;
}

void VirtnosisWindow::retrieve_domains(Hypervisor *hypervisor)
{
    virDomainPtr *domains;
    int i;

    unsigned int flags = VIR_CONNECT_LIST_DOMAINS_RUNNING |
                         VIR_CONNECT_LIST_DOMAINS_PERSISTENT;

    virConnectPtr connection = virConnectOpen(hypervisor->uri().toLatin1().data());

    qDebug() << "VirtnosisWindow::list_domains: connection = " << connection;

    int number_of_domains = virConnectListAllDomains(connection, &domains, flags);
    qDebug() << "VirtnosisWindow::list_domains: number_of_domains = " << number_of_domains;

    if (number_of_domains < 0)
        qDebug() << "VirtnosisWindow::list_domains: error = " << number_of_domains;

    for (i = 0; i < number_of_domains; i++) {

        qDebug() << "VirtnosisWindow::list_domains: domains[" << i << "] = " << domains[i] << "name = " << virDomainGetName(domains[i]);

        //here or in a separate loop if needed
        virDomainFree(domains[i]);
    }
    free(domains);
}

void VirtnosisWindow::addHypervisor(Hypervisor *hypervisor)
{
    QStandardItem *item = new QStandardItem(hypervisor->name());
    addDomainsFromHypervisor(hypervisor, item);
    QStandardItemModel *model = qobject_cast<QStandardItemModel *>(ui->domainView->model());
    model->appendRow(item);
}

void VirtnosisWindow::addDomainsFromHypervisor(Hypervisor *hypervisor, QStandardItem *item)
{
    virDomainPtr *domains;
    int i;
    QStandardItem *domain_item;

    unsigned int flags = VIR_CONNECT_LIST_DOMAINS_RUNNING |
                         VIR_CONNECT_LIST_DOMAINS_PERSISTENT;

    virConnectPtr connection = virConnectOpen(hypervisor->uri().toLatin1().data());

    qDebug() << "VirtnosisWindow::list_domains: connection = " << connection;

    int number_of_domains = virConnectListAllDomains(connection, &domains, flags);
    qDebug() << "VirtnosisWindow::list_domains: number_of_domains = " << number_of_domains;

    if (number_of_domains < 0)
        qDebug() << "VirtnosisWindow::list_domains: error = " << number_of_domains;

    for (i = 0; i < number_of_domains; i++) {

        qDebug() << "VirtnosisWindow::list_domains: domains[" << i << "] = " << domains[i] << "name = " << virDomainGetName(domains[i]);
        domain_item = new QStandardItem(virDomainGetName(domains[i]));
        item->appendRow(domain_item);
        //here or in a separate loop if needed
        virDomainFree(domains[i]);
    }
    free(domains);
}

void VirtnosisWindow::on_menuHypervisorActionNew_triggered()
{
    HypervisorDialog *dialog = new HypervisorDialog(this);
    dialog->show();
}
