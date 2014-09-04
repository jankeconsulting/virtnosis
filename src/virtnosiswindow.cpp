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
    DomainViewModel *model = new DomainViewModel(this);
    ui->domainView->setModel(model);
    DomainViewItemDelegate *delegate = new DomainViewItemDelegate(this);
    ui->domainView->setItemDelegate(qobject_cast<QAbstractItemDelegate *>(delegate));
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
    item->addDomainsFromHypervisor(hypervisor);

    QModelIndex index = model->index(model->rowCount()-1, 0);
    test.setValue(*hypervisor);
    model->setData(index, test, DomainViewModel::domainHypervisorRole);
}

void VirtnosisWindow::on_menuHypervisorActionNew_triggered()
{
    HypervisorDialog *dialog = new HypervisorDialog(this);
    dialog->show();
}
