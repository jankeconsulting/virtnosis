/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#ifndef VIRTNOSISWINDOW_H
#define VIRTNOSISWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QSettings>
#include "hypervisor.h"
#include "hypervisordialog.h"
#include "hypervisoritem.h"
#include <domainviewmodel.h>
#include "domainitem.h"
#include "domainviewitemdelegate.h"
#include "aboutdialog.h"

#ifndef VIRTNOSIS_REVISION
#define VIRTNOSIS_REVISION "1.0.0"
#endif

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
    void on_menuHypervisorActionConnect_triggered();
    void on_menuHypervisorActionDisconnect_triggered();
    void on_menuHypervisorActionRemove_triggered();
    void on_menuVmActionStart_triggered();
    void on_menuVmActionReboot_triggered();
    void on_menuVmActionShutoff_triggered();
    void on_menuVmActionPause_triggered();
    void on_menuVmActionResume_triggered();
    void selectionChanged(const QModelIndex &current, const QModelIndex &previous);
    void on_actionAbout_triggered();
    void on_actionAboutQt_triggered();

    void on_actionExit_triggered();

private:
    Ui::VirtnosisWindow *ui;
    QSettings m_settings;
    AboutDialog *about;

    void enableVirtualMachineActions(const QModelIndex &index);
    Domain selectedDomain();
    Hypervisor selectedHypervisor();
    HypervisorItem *selectedItem();
    void selectedDataChanged();
    DomainViewModel *model();
    QModelIndex currentIndex();
    void writeHypervisorSettings();
    void readHypervisorSettings();
};

#endif // VIRTNOSISWINDOW_H
