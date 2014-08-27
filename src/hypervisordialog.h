/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#ifndef HYPERVISORDIALOG_H
#define HYPERVISORDIALOG_H

#include <QDialog>

namespace Ui {
class HypervisorDialog;
}

class HypervisorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HypervisorDialog(QString host = "", QString account = "", int port = 22, QWidget *parent = 0);
    ~HypervisorDialog();

    QString host();
    QString account();
    int port();

private:
    Ui::HypervisorDialog *ui;
};

#endif // HYPERVISORDIALOG_H
