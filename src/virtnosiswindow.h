/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#ifndef VIRTNOSISWINDOW_H
#define VIRTNOSISWINDOW_H

#include <QMainWindow>
#include "libvirt/libvirt.h"
#include "libvirt/virterror.h"
#include "hypervisor.h"
#include "hypervisordialog.h"
#include <QStandardItemModel>

namespace Ui {
class VirtnosisWindow;
}

class VirtnosisWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit VirtnosisWindow(QWidget *parent = 0);
    ~VirtnosisWindow();
    void retrieve_domains(Hypervisor *hypervisor);
    void addHypervisor(Hypervisor *hypervisor);
    void addDomainsFromHypervisor(Hypervisor *hypervisor, QStandardItem *item);

private slots:
    void on_menuHypervisorActionNew_triggered();

private:
    Ui::VirtnosisWindow *ui;
};

#endif // VIRTNOSISWINDOW_H
