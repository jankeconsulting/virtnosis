/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#ifndef SRC_ABOUTDIALOG_H_
#define SRC_ABOUTDIALOG_H_

#include <QDialog>
#include "./licensedialog.h"

namespace Ui
{
class AboutDialog;
}

/**
 * @brief The AboutDialog class
 */
class AboutDialog : public QDialog
{
    Q_OBJECT

 public:
    explicit AboutDialog(QWidget *parent = 0, QString version = "");
    ~AboutDialog();

 private slots:
    void on_licenseButton_clicked();

 private:
    Ui::AboutDialog *ui;
    LicenseDialog *license;
};

#endif  // SRC_ABOUTDIALOG_H_
