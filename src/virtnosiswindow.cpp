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
    connect(ui->domainView->model(), SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(selectionChanged(QModelIndex,QModelIndex)));
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
    model->connectHypervisor(index);
}

void VirtnosisWindow::on_menuHypervisorActionNew_triggered()
{
    HypervisorDialog *dialog = new HypervisorDialog(this);
    dialog->show();
}

void VirtnosisWindow::on_menuHypervisorActionConnect_triggered()
{
    model()->connectHypervisor(currentIndex());
}

void VirtnosisWindow::on_menuHypervisorActionDisconnect_triggered()
{
    model()->disconnectHypervisor(currentIndex());
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
    ui->menuHypervisorActionConnect->setDisabled(true);
    ui->menuHypervisorActionDisconnect->setDisabled(true);

    if(index.data(DomainViewModel::domainTypeRole) == DomainViewModel::typeHypervisor) {
        Hypervisor hypervisor = qvariant_cast<Hypervisor>(index.data(DomainViewModel::domainHypervisorRole));
        if(hypervisor.alive()) {
            ui->menuHypervisorActionDisconnect->setEnabled(true);
        } else {
            ui->menuHypervisorActionConnect->setEnabled(true);
        }
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
    QModelIndex index = currentIndex();
    return qvariant_cast<Domain>(index.data(DomainViewModel::domainDomainRole));
}

Hypervisor VirtnosisWindow::selectedHypervisor()
{
    QModelIndex index = currentIndex();
    qDebug() << "VirtnosisWindow::selectedHypervisor: index = " << index;
    return qvariant_cast<Hypervisor>(index.data(DomainViewModel::domainHypervisorRole));
}

HypervisorItem *VirtnosisWindow::selectedItem()
{
//    TODO: Check if index is on hypervisor
    QModelIndex index = currentIndex();
    if (!index.isValid())
        return 0;
    if(!(index.data(DomainViewModel::domainTypeRole) == DomainViewModel::typeHypervisor))
        return 0;
    Hypervisor hypervisor = qvariant_cast<Hypervisor>(index.data(DomainViewModel::domainHypervisorRole));
    //return static_cast<HypervisorItem*>(hypervisor.item());
    return 0;
}

void VirtnosisWindow::selectedDataChanged()
{
    QModelIndex index = currentIndex();
    ui->domainView->dataChanged(index, index);
}

DomainViewModel *VirtnosisWindow::model()
{
    return qobject_cast<DomainViewModel *>(ui->domainView->model());
}

QModelIndex VirtnosisWindow::currentIndex()
{
    return ui->domainView->currentIndex();
}

void VirtnosisWindow::selectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    enableVirtualMachineActions(current);
}





