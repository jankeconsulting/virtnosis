/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#ifndef SRC_HYPERVISORDIALOG_H_
#define SRC_HYPERVISORDIALOG_H_

#include <QDialog>
#include <QStandardItem>
#include "./hypervisor.h"

namespace Ui
{
class HypervisorDialog;
}

/**
 * @brief The HypervisorDialog class
 *
 * The HypervisorDialog class defines a dialog to enter
 * the information needed to create a connection to
 * a hypervisor.
 *
 */
class HypervisorDialog : public QDialog
{
    Q_OBJECT

 public:
    explicit HypervisorDialog(QWidget *parent = 0, QString host = "", QString account = "",
                              int port = 22);
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

#endif  // SRC_HYPERVISORDIALOG_H_
