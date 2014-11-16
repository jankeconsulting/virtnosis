/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#include "./licensedialog.h"
#include "./ui_licensedialog.h"

/**
 * @brief constructs the LicenseDialog object
 * @param parent - parent of the constructed object
 */
LicenseDialog::LicenseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LicenseDialog)
{
    ui->setupUi(this);
}

/**
 * @brief destroys the LicenseDialog object
 */
LicenseDialog::~LicenseDialog()
{
    delete ui;
}
