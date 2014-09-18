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
    qRegisterMetaType<Hypervisor>("Hypervisor");
    qRegisterMetaType<Domain>("Domain");
    ui->setupUi(this);
    DomainViewModel *model = new DomainViewModel(this);
    ui->domainView->setModel(model);
    DomainViewItemDelegate *delegate = new DomainViewItemDelegate(this);
    ui->domainView->setItemDelegate(qobject_cast<QAbstractItemDelegate *>(delegate));
    connect(ui->domainView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(selectionChanged(QModelIndex,QModelIndex)));
}

VirtnosisWindow::~VirtnosisWindow()
{
    delete ui;
}

void VirtnosisWindow::addHypervisor(Hypervisor *hypervisor)
{
    QVariant test;

    HypervisorItem *item = new HypervisorItem(hypervisor);
    DomainViewModel *model = qobject_cast<DomainViewModel *>(ui->domainView->model());

    model->appendRow(item);

    QModelIndex index = model->index(model->rowCount()-1, 0);
    test.setValue(*hypervisor);
    model->setData(index, test, DomainViewModel::domainHypervisorRole);
    model->setData(index, DomainViewModel::typeHypervisor, DomainViewModel::domainTypeRole);

    item->addDomainsFromHypervisor(hypervisor);
}

void VirtnosisWindow::on_menuHypervisorActionNew_triggered()
{
    HypervisorDialog *dialog = new HypervisorDialog(this);
    dialog->show();
}

void VirtnosisWindow::on_menuHypervisorActionConnect_triggered()
{
    Hypervisor hypervisor = selectedHypervisor();
    hypervisor.connection();
//    TODO: get all domains to view
}

void VirtnosisWindow::on_menuHypervisorActionDisconnect_triggered()
{
    Hypervisor hypervisor = selectedHypervisor();
    hypervisor.disconnect();
//    TODO: remove all domains from view
    QStandardItem *item = selectedItem();
}

void VirtnosisWindow::on_menuVmActionStart_triggered()
{
    Domain domain = selectedDomain();
    domain.start();
}

void VirtnosisWindow::on_menuVmActionReboot_triggered()
{
    Domain domain = selectedDomain();
    domain.reboot();
}

void VirtnosisWindow::on_menuVmActionShutoff_triggered()
{
    Domain domain = selectedDomain();
    domain.shutdown();
}

void VirtnosisWindow::on_menuVmActionPause_triggered()
{
    Domain domain = selectedDomain();
    domain.pause();
}

void VirtnosisWindow::on_menuVmActionResume_triggered()
{
    Domain domain = selectedDomain();
    domain.resume();
}

void VirtnosisWindow::enableVirtualMachineActions(const QModelIndex &index)
{
    ui->menuVmActionStart->setDisabled(true);
    ui->menuVmActionReboot->setDisabled(true);
    ui->menuVmActionShutoff->setDisabled(true);
    ui->menuVmActionPause->setDisabled(true);
    ui->menuVmActionResume->setDisabled(true);

    if(index.data(DomainViewModel::domainTypeRole) == DomainViewModel::typeHypervisor) {
    } else {
        Domain domain = qvariant_cast<Domain>(index.data(DomainViewModel::domainDomainRole));
        if(domain.isRunning()) {
            ui->menuVmActionShutoff->setEnabled(true);
            ui->menuVmActionPause->setEnabled(true);
            ui->menuVmActionReboot->setEnabled(true);
        } else if (domain.isPaused()) {
            ui->menuVmActionResume->setEnabled(true);
        } else {
            ui->menuVmActionStart->setEnabled(true);
        }
    }
}

Domain VirtnosisWindow::selectedDomain()
{
    QModelIndex index = ui->domainView->currentIndex();
    return qvariant_cast<Domain>(index.data(DomainViewModel::domainDomainRole));
}

Hypervisor VirtnosisWindow::selectedHypervisor()
{
    QModelIndex index = ui->domainView->currentIndex();
    return qvariant_cast<Hypervisor>(index.data(DomainViewModel::domainHypervisorRole));
}

QStandardItem *VirtnosisWindow::selectedItem()
{
    QModelIndex index = ui->domainView->currentIndex();
    //    return ui->domainView->selectionModel()->
}

void VirtnosisWindow::selectedDataChanged()
{
    QModelIndex index = ui->domainView->currentIndex();
    ui->domainView->dataChanged(index, index);
}

void VirtnosisWindow::selectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    enableVirtualMachineActions(current);
}





