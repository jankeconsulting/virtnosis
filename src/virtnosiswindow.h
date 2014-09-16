/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#ifndef VIRTNOSISWINDOW_H
#define VIRTNOSISWINDOW_H

#include <QMainWindow>
#include "hypervisor.h"
#include "hypervisordialog.h"
#include "hypervisoritem.h"
#include <domainviewmodel.h>
#include "domainitem.h"
#include "domainviewitemdelegate.h"

namespace Ui {
class VirtnosisWindow;
}

class VirtnosisWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit VirtnosisWindow(QWidget *parent = 0);
    ~VirtnosisWindow();
    void addHypervisor(Hypervisor *hypervisor);

private slots:
    void on_menuHypervisorActionNew_triggered();
    void on_action_Start_triggered();
    void on_action_Reboot_triggered();
    void on_actionShut_off_triggered();

    void selectionChanged(const QModelIndex &current, const QModelIndex &previous);

private:
    Ui::VirtnosisWindow *ui;

    void enableVirtualMachineActions(const QModelIndex &index);
};

#endif // VIRTNOSISWINDOW_H
