/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#include "./aboutdialog.h"
#include "./ui_aboutdialog.h"

/**
 * @brief constructs the About dialog
 * @param parent - parent object of the dialog
 * @param version - version of virtnosis that is display in dialog
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
 * @brief destroys the about dialog
 */
AboutDialog::~AboutDialog()
{
    delete license;
    delete ui;
}

/**
 * @brief slot that is called when license is requested
 */
void AboutDialog::on_licenseButton_clicked()
{
    license->show();
}
