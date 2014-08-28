/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#include "hypervisordialog.h"
#include "ui_hypervisordialog.h"
#include "virtnosiswindow.h"
#include <QDebug>

HypervisorDialog::HypervisorDialog(QWidget *parent, QString host, QString account, int port) :
    QDialog(parent),
    ui(new Ui::HypervisorDialog)
{
    ui->setupUi(this);
    ui->hostField->setText(host);
    ui->accountField->setText(account);
    ui->portField->setText(QString("%1").arg(port));
}

HypervisorDialog::~HypervisorDialog()
{
    delete ui;
}

QString HypervisorDialog::host()
{
    return ui->hostField->text();
}

QString HypervisorDialog::account()
{
    return ui->accountField->text();
}

int HypervisorDialog::port()
{
    return ui->portField->text().toInt();

}

void HypervisorDialog::updateUri()
{
    ui->uri->setText(Hypervisor::uri(host(), account(), port()));
}

void HypervisorDialog::on_hypervisorDialogbuttonBox_accepted()
{
//    TODO: EditLineValidators
//    TODO: storing inputs in hypervisor object and return it
    Hypervisor *hypervisor = new Hypervisor(host(), account(), port());
    qDebug() << "HypervisorDialog::on_hypervisorDialogbuttonBox_accepted: hypervisor->uri = " << hypervisor->uri();
    VirtnosisWindow *widget = qobject_cast<VirtnosisWindow *>(parent());
    widget->addHypervisor(hypervisor);
}

void HypervisorDialog::on_hostField_textChanged(const QString &text)
{
    Q_UNUSED(text);
    updateUri();
}

void HypervisorDialog::on_accountField_textChanged(const QString &text)
{
    Q_UNUSED(text);
    updateUri();
}


void HypervisorDialog::on_portField_textChanged(const QString &text)
{
    Q_UNUSED(text);
    updateUri();
}
