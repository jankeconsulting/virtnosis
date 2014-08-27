/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#include "hypervisordialog.h"
#include "ui_hypervisordialog.h"

HypervisorDialog::HypervisorDialog(QString host, QString account, int port, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HypervisorDialog)
{
    ui->setupUi(this);
    ui->hostField->setText(host);
    ui->accountField->setText(account);
    ui->portField->setText(QString().arg(port));
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
