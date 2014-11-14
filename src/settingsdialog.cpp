/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QDebug>

/**
 * @brief SettingsDialog::SettingsDialog
 * @param parent
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
 * @brief SettingsDialog::~SettingsDialog
 */
SettingsDialog::~SettingsDialog()
{
    delete ui;
}

/**
 * @brief SettingsDialog::changeTimeout
 * @return
 */
int SettingsDialog::changeTimeout()
{
    return m_changeTimeout;
}

/**
 * @brief SettingsDialog::setChangeTimeout
 * @param value
 */
void SettingsDialog::setChangeTimeout(int value)
{
    m_changeTimeout = value;
    ui->checkTimeoutTime->setText(QString("%1").arg(m_changeTimeout));
}

/**
 * @brief SettingsDialog::on_buttonBox_accepted
 */
void SettingsDialog::on_buttonBox_accepted()
{
    m_changeTimeout = ui->checkTimeoutTime->text().toInt();
}

/**
 * @brief SettingsDialog::on_buttonBox_rejected
 */
void SettingsDialog::on_buttonBox_rejected()
{
    ui->checkTimeoutTime->setText(QString("%1").arg(m_changeTimeout));
}
