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

class HypervisorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HypervisorDialog(QWidget *parent = 0, QString host = "", QString account = "", int port = 22);
    ~HypervisorDialog();

    QString host();
    QString account();
    int port();

private slots:
    void on_hypervisorDialogbuttonBox_accepted();

private:
    Ui::HypervisorDialog *ui;
};

#endif // HYPERVISORDIALOG_H
