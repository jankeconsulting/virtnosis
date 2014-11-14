/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#ifndef HYPERVISORDIALOG_H
#define HYPERVISORDIALOG_H

#include <QDialog>
#include "hypervisor.h"
#include <QStandardItem>

namespace Ui {
class HypervisorDialog;
}

/**
 * @brief The HypervisorDialog class
 */
class HypervisorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HypervisorDialog(QWidget *parent = 0, QString host = "", QString account = "", int port = 22);
    ~HypervisorDialog();

    QString host();
    QString account();
    int port();
    void updateUri();

private slots:
    void on_hypervisorDialogbuttonBox_accepted();
    void on_hostField_textChanged(const QString &text);
    void on_accountField_textChanged(const QString &text);
    void on_portField_textChanged(const QString &text);

private:
    Ui::HypervisorDialog *ui;
};

#endif // HYPERVISORDIALOG_H
