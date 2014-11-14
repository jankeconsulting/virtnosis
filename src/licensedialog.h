/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#ifndef LICENSEDIALOG_H
#define LICENSEDIALOG_H

#include <QDialog>

namespace Ui {
class LicenseDialog;
}

/**
 * @brief The LicenseDialog class
 */
class LicenseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LicenseDialog(QWidget *parent = 0);
    ~LicenseDialog();

private:
    Ui::LicenseDialog *ui;
};

#endif // LICENSEDIALOG_H
