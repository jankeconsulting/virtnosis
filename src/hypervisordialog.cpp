/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#include "hypervisordialog.h"
#include "ui_hypervisordialog.h"
#include "virtnosiswindow.h"
#include <QDebug>

/**
 * @brief HypervisorDialog::HypervisorDialog
 * @param parent
 * @param host
 * @param account
 * @param port
 */
HypervisorDialog::HypervisorDialog(QWidget *parent, QString host, QString account, int port) :
    QDialog(parent),
    ui(new Ui::HypervisorDialog)
{
    ui->setupUi(this);
    ui->hostField->setText(host);
    ui->accountField->setText(account);
    ui->portField->setText(QString("%1").arg(port));
//    QRegExp host_reg_exp("^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])|(([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9\-]*[a-zA-Z0-9])\.)*([A-Za-z0-9]|[A-Za-z0-9][A-Za-z0-9\-]*[A-Za-z0-9])$");
    QRegExp host_reg_exp("^(([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9\-]*[a-zA-Z0-9])\.)*([A-Za-z0-9]|[A-Za-z0-9][A-Za-z0-9\-]*[A-Za-z0-9])$");
    QValidator *host_validator = new QRegExpValidator(host_reg_exp, this);
    ui->hostField->setValidator(host_validator);
    QIntValidator *port_validator = new QIntValidator(1,65355, this);
    ui->portField->setValidator(port_validator);

}

/**
 * @brief HypervisorDialog::~HypervisorDialog
 */
HypervisorDialog::~HypervisorDialog()
{
    delete ui;
}

/**
 * @brief HypervisorDialog::host
 * @return
 */
QString HypervisorDialog::host()
{
    return ui->hostField->text();
}

/**
 * @brief HypervisorDialog::account
 * @return
 */
QString HypervisorDialog::account()
{
    return ui->accountField->text();
}

/**
 * @brief HypervisorDialog::port
 * @return
 */
int HypervisorDialog::port()
{
    return ui->portField->text().toInt();

}

/**
 * @brief HypervisorDialog::updateUri
 */
void HypervisorDialog::updateUri()
{
    ui->uri->setText(Hypervisor::uri(host(), account(), port()));
}

/**
 * @brief HypervisorDialog::on_hypervisorDialogbuttonBox_accepted
 */
void HypervisorDialog::on_hypervisorDialogbuttonBox_accepted()
{
    Hypervisor *hypervisor = new Hypervisor(host(), account(), port());
    VirtnosisWindow *widget = qobject_cast<VirtnosisWindow *>(parent());
    hypervisor->setAutoConnect(true);
    widget->addHypervisor(hypervisor);
}

/**
 * @brief HypervisorDialog::on_hostField_textChanged
 * @param text
 */
void HypervisorDialog::on_hostField_textChanged(const QString &text)
{
    Q_UNUSED(text);
    updateUri();
}

/**
 * @brief HypervisorDialog::on_accountField_textChanged
 * @param text
 */
void HypervisorDialog::on_accountField_textChanged(const QString &text)
{
    Q_UNUSED(text);
    updateUri();
}


/**
 * @brief HypervisorDialog::on_portField_textChanged
 * @param text
 */
void HypervisorDialog::on_portField_textChanged(const QString &text)
{
    Q_UNUSED(text);
    updateUri();
}
