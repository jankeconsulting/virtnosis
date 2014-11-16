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
 * @brief constructs HypervisorDialog object
 * @param parent - parent of the constructed object
 * @param host - host of the hypervisor
 * @param account - account for the connection
 * @param port - port for the connection
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
 * @brief destroys the object
 */
HypervisorDialog::~HypervisorDialog()
{
    delete ui;
}

/**
 * @brief provides the host entered into the dialog
 * @return - hostname
 */
QString HypervisorDialog::host()
{
    return ui->hostField->text();
}

/**
 * @brief provides the account entered into the dialog
 * @return account
 */
QString HypervisorDialog::account()
{
    return ui->accountField->text();
}

/**
 * @brief provides the port entered into the dialog
 * @return port
 */
int HypervisorDialog::port()
{
    return ui->portField->text().toInt();

}

/**
 * @brief updates the uri displayed in the dialog
 *
 * The uri is build from the information entered into
 * the host, account and port fields in the dialog
 */
void HypervisorDialog::updateUri()
{
    ui->uri->setText(Hypervisor::uri(host(), account(), port()));
}

/**
 * @brief slot that is called when dialog is accepted
 *
 * A hypervisor object is created, set to auto-connect and
 * added to the view (and due to auto-connect, automatically
 * connected)
 */
void HypervisorDialog::on_hypervisorDialogbuttonBox_accepted()
{
    Hypervisor *hypervisor = new Hypervisor(host(), account(), port());
    VirtnosisWindow *widget = qobject_cast<VirtnosisWindow *>(parent());
    hypervisor->setAutoConnect(true);
    widget->addHypervisor(hypervisor);
}

/**
 * @brief slot that is called when dialog field for the host is changed
 * @param text - hostname
 *
 * Calls updateUri so that changed uri is displayed
 */
void HypervisorDialog::on_hostField_textChanged(const QString &text)
{
    Q_UNUSED(text);
    updateUri();
}

/**
 * @brief slot that is called when dialog field for the account is changed
 * @param text - account
 *
 * Calls updateUri so that changed uri is displayed
 */
void HypervisorDialog::on_accountField_textChanged(const QString &text)
{
    Q_UNUSED(text);
    updateUri();
}


/**
 * @brief slot that is called when dialog field for the port is changed
 * @param text - port
 *
 * Calls updateUri so that changed uri is displayed
 */
void HypervisorDialog::on_portField_textChanged(const QString &text)
{
    Q_UNUSED(text);
    updateUri();
}
