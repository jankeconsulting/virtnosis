/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#include "aboutdialog.h"
#include "ui_aboutdialog.h"

/**
 * @brief AboutDialog::AboutDialog
 * @param parent
 * @param version
 */
AboutDialog::AboutDialog(QWidget *parent, QString version) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    ui->version->setText(tr("Version ").append(version));
    license = new LicenseDialog(this);
}

/**
 * @brief AboutDialog::~AboutDialog
 */
AboutDialog::~AboutDialog()
{
    delete license;
    delete ui;
}

/**
 * @brief AboutDialog::on_licenseButton_clicked
 */
void AboutDialog::on_licenseButton_clicked()
{
    license->show();
}
