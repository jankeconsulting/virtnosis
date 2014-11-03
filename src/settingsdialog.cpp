/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QDebug>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog),
    m_changeTimeout(VIRTNOSIS_DEFAULT_CHANGE_TIME)
{
    ui->setupUi(this);
    ui->checkTimeoutTime->setText(QString("%1").arg(m_changeTimeout));
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

int SettingsDialog::changeTimeout()
{
    return m_changeTimeout;
}

void SettingsDialog::setChangeTimeout(int value)
{
    m_changeTimeout = value;
    ui->checkTimeoutTime->setText(QString("%1").arg(m_changeTimeout));
}

void SettingsDialog::on_buttonBox_accepted()
{
    m_changeTimeout = ui->checkTimeoutTime->text().toInt();
}

void SettingsDialog::on_buttonBox_rejected()
{
    ui->checkTimeoutTime->setText(QString("%1").arg(m_changeTimeout));
}
