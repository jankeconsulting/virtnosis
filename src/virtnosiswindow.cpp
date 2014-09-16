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

void VirtnosisWindow::on_action_Start_triggered()
{

}

void VirtnosisWindow::on_action_Reboot_triggered()
{

}

void VirtnosisWindow::on_actionShut_off_triggered()
{

}

void VirtnosisWindow::enableVirtualMachineActions(const QModelIndex &index)
{
//    DomainViewModel *model = qobject_cast<DomainViewModel *>(index.model());
    if(index.data(DomainViewModel::domainTypeRole) == DomainViewModel::typeHypervisor) {
        ui->action_Start->setDisabled(true);
        ui->action_Reboot->setDisabled(true);
        ui->actionShut_off->setDisabled(true);
    } else {
        Domain domain = qvariant_cast<Domain>(index.data(DomainViewModel::domainDomainRole));
        if(domain.isActive()) {
            ui->action_Start->setDisabled(true);
            ui->action_Reboot->setDisabled(true);
            ui->actionShut_off->setEnabled(true);
        } else {
            ui->action_Start->setEnabled(true);
            ui->action_Reboot->setEnabled(true);
            ui->actionShut_off->setDisabled(true);
        }
    }
}

void VirtnosisWindow::selectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
    enableVirtualMachineActions(current);
}
