/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QDebug>

/**
 * @brief constructs the SettingsDialog object
 * @param parent - parent of the constructed object
 */
SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog),
    m_changeTimeout(VIRTNOSIS_DEFAULT_CHANGE_TIME)
{
    ui->setupUi(this);
    ui->checkTimeoutTime->setText(QString("%1").arg(m_changeTimeout));
}

/**
 * @brief destroys the SettingsDialog object
 */
SettingsDialog::~SettingsDialog()
{
    delete ui;
}

/**
 * @brief provides the timeout for the watchdog function
 * @return
 */
int SettingsDialog::changeTimeout()
{
    return m_changeTimeout;
}

/**
 * @brief sets the timeout for the watchdog function
 * @param value
 */
void SettingsDialog::setChangeTimeout(int value)
{
    m_changeTimeout = value;
    ui->checkTimeoutTime->setText(QString("%1").arg(m_changeTimeout));
}

/**
 * @brief slot that is called when the the accept button is called
 *
 * Saves all the settings in the dialog
 */
void SettingsDialog::on_buttonBox_accepted()
{
    m_changeTimeout = ui->checkTimeoutTime->text().toInt();
}

/**
 * @brief slot that is called when the reject button is called
 *
 * Resets the settings in the dialog
 */
void SettingsDialog::on_buttonBox_rejected()
{
    ui->checkTimeoutTime->setText(QString("%1").arg(m_changeTimeout));
}
