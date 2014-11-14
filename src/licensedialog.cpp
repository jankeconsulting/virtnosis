/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#include "licensedialog.h"
#include "ui_licensedialog.h"

/**
 * @brief LicenseDialog::LicenseDialog
 * @param parent
 */
LicenseDialog::LicenseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LicenseDialog)
{
    ui->setupUi(this);
}

/**
 * @brief LicenseDialog::~LicenseDialog
 */
LicenseDialog::~LicenseDialog()
{
    delete ui;
}
