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
#include <QLabel>
#include <QContextMenuEvent>
#include <QtConcurrent/QtConcurrent>
#include <QMessageBox>
#include "hypervisor.h"
#include "hypervisordialog.h"
#include "hypervisoritem.h"
#include <domainviewmodel.h>
#include "domainitem.h"
#include "domainviewitemdelegate.h"
#include "aboutdialog.h"
#include "settingsdialog.h"

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

public slots:
    void dataChanged();
    void setStatusMessage(QString text);

protected:
    void contextMenuEvent(QContextMenuEvent *event);

signals:

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
    void on_actionAbout_triggered();
    void on_actionAboutQt_triggered();
    void on_actionPreferences_triggered();
    void on_actionExit_triggered();
    void selectionChanged(const QModelIndex &current, const QModelIndex &previous);

    void handleConnectingStarted();
    void handleConnectingFinished();

    void on_menuVmActionViewer_triggered();

private:
    Ui::VirtnosisWindow *ui;
    QSettings m_settings;
    AboutDialog *about;
    SettingsDialog *settingsDialog;
    QLabel *statusMessage;
    QFuture<void> connectingThread;
    QFutureWatcher<void> connectingThreadWatcher;
    QFuture<void> checkDomainStateChangeTread;
    QFutureWatcher<void> checkDomainStateChangeTreadWatcher;

    void enableVirtualMachineActions(const QModelIndex &index);
    Domain selectedDomain();
    Hypervisor selectedHypervisor();
    void selectedDataChanged();
    DomainViewModel *model();
    QModelIndex currentIndex();
    void readSettings();
    void writeSettings();
    void writeHypervisorSettings();
    void readHypervisorSettings();
    void createStatusBar();
    void connectHypervisor(QModelIndex index);
    void checkDomainStateChange(QModelIndex index, int state);
};

#endif // VIRTNOSISWINDOW_H
