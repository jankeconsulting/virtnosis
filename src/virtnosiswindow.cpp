/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#include "virtnosiswindow.h"
#include "ui_virtnosiswindow.h"

#include <QDebug>

VirtnosisWindow::VirtnosisWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VirtnosisWindow),
    m_settings()
{
    qRegisterMetaType<Hypervisor>("Hypervisor");
    qRegisterMetaTypeStreamOperators<Hypervisor>("Hypervisor");
    qRegisterMetaType<Domain>("Domain");
    ui->setupUi(this);
    setWindowTitle(QString(tr("Virtnosis - Virtual Manchine Manager")));
    about = new AboutDialog(this, VIRTNOSIS_REVISION);

    DomainViewModel *model = new DomainViewModel(this);
    ui->domainView->setModel(model);
    DomainViewItemDelegate *delegate = new DomainViewItemDelegate(this);
    ui->domainView->setItemDelegate(qobject_cast<QAbstractItemDelegate *>(delegate));
    connect(ui->domainView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(selectionChanged(QModelIndex,QModelIndex)));
    connect(ui->domainView->model(), SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(selectionChanged(QModelIndex,QModelIndex)));
    readSettings();
}

VirtnosisWindow::~VirtnosisWindow()
{
    delete about;
    delete ui;
}

void VirtnosisWindow::addHypervisor(Hypervisor *hypervisor)
{
    QVariant test;

    HypervisorItem *item = new HypervisorItem(hypervisor);
    DomainViewModel *model = qobject_cast<DomainViewModel *>(ui->domainView->model());

    model->appendRow(item);

    QModelIndex index = model->index(model->rowCount()-1, 0);

    test.setValue(*hypervisor);
    model->setData(index, test, DomainViewModel::domainHypervisorRole);
    model->setData(index, DomainViewModel::typeHypervisor, DomainViewModel::domainTypeRole);
    model->connectHypervisor(index);

    writeHypervisorSettings();
}

void VirtnosisWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    if(model()->indexIsHypervisor(currentIndex())) {
        menu.addAction(ui->menuHypervisorActionConnect);
        menu.addAction(ui->menuHypervisorActionDisconnect);
        menu.addAction(ui->menuHypervisorActionRemove);
        menu.exec(event->globalPos());
    }
    if(model()->indexIsDomain(currentIndex())) {
        menu.addAction(ui->menuVmActionStart);
        menu.addAction(ui->menuVmActionReboot);
        menu.addAction(ui->menuVmActionShutoff);
        menu.addAction(ui->menuVmActionPause);
        menu.addAction(ui->menuVmActionResume);
        menu.exec(event->globalPos());
    }
}

void VirtnosisWindow::on_menuHypervisorActionNew_triggered()
{
    HypervisorDialog *dialog = new HypervisorDialog(this);
    dialog->show();
}

void VirtnosisWindow::on_menuHypervisorActionConnect_triggered()
{
    model()->connectHypervisor(currentIndex());
}

void VirtnosisWindow::on_menuHypervisorActionDisconnect_triggered()
{
    model()->disconnectHypervisor(currentIndex());
}

void VirtnosisWindow::on_menuHypervisorActionRemove_triggered()
{
    model()->removeRow(currentIndex().row());
    writeHypervisorSettings();
}

void VirtnosisWindow::on_menuVmActionStart_triggered()
{
    Domain domain = selectedDomain();
    domain.start();
}

void VirtnosisWindow::on_menuVmActionReboot_triggered()
{
    Domain domain = selectedDomain();
    domain.reboot();
}

void VirtnosisWindow::on_menuVmActionShutoff_triggered()
{
    Domain domain = selectedDomain();
    domain.shutdown();
}

void VirtnosisWindow::on_menuVmActionPause_triggered()
{
    Domain domain = selectedDomain();
    domain.pause();
}

void VirtnosisWindow::on_menuVmActionResume_triggered()
{
    Domain domain = selectedDomain();
    domain.resume();
}

void VirtnosisWindow::enableVirtualMachineActions(const QModelIndex &index)
{
    ui->menuVmActionStart->setDisabled(true);
    ui->menuVmActionReboot->setDisabled(true);
    ui->menuVmActionShutoff->setDisabled(true);
    ui->menuVmActionPause->setDisabled(true);
    ui->menuVmActionResume->setDisabled(true);
    ui->menuHypervisorActionConnect->setDisabled(true);
    ui->menuHypervisorActionDisconnect->setDisabled(true);
    ui->menuHypervisorActionRemove->setDisabled(true);

    if(index.data(DomainViewModel::domainTypeRole) == DomainViewModel::typeHypervisor) {
        Hypervisor hypervisor = qvariant_cast<Hypervisor>(index.data(DomainViewModel::domainHypervisorRole));
        ui->menuHypervisorActionRemove->setEnabled(true);
        if(hypervisor.alive()) {
            ui->menuHypervisorActionDisconnect->setEnabled(true);
        } else {
            ui->menuHypervisorActionConnect->setEnabled(true);
        }
    } else {
        Domain domain = qvariant_cast<Domain>(index.data(DomainViewModel::domainDomainRole));
        if(domain.isRunning()) {
            ui->menuVmActionShutoff->setEnabled(true);
            ui->menuVmActionPause->setEnabled(true);
            ui->menuVmActionReboot->setEnabled(true);
        } else if (domain.isPaused()) {
            ui->menuVmActionResume->setEnabled(true);
        } else {
            ui->menuVmActionStart->setEnabled(true);
        }
    }    
}

Domain VirtnosisWindow::selectedDomain()
{
    QModelIndex index = currentIndex();
    return qvariant_cast<Domain>(index.data(DomainViewModel::domainDomainRole));
}

Hypervisor VirtnosisWindow::selectedHypervisor()
{
    QModelIndex index = currentIndex();
    return qvariant_cast<Hypervisor>(index.data(DomainViewModel::domainHypervisorRole));
}

void VirtnosisWindow::selectedDataChanged()
{
    QModelIndex index = currentIndex();
    ui->domainView->dataChanged(index, index);
}

DomainViewModel *VirtnosisWindow::model()
{
    return qobject_cast<DomainViewModel *>(ui->domainView->model());
}

QModelIndex VirtnosisWindow::currentIndex()
{
    return ui->domainView->currentIndex();
}

void VirtnosisWindow::readSettings()
{
    m_settings.beginGroup("VirtnosisWindow");
    QByteArray state = m_settings.value("state", QByteArray()).toByteArray();
    restoreState(state);
    m_settings.endGroup();
    readHypervisorSettings();
}

void VirtnosisWindow::writeSettings()
{
    m_settings.beginGroup("VirtnosisWindow");
    m_settings.setValue("state", saveState());
    m_settings.endGroup();
}

void VirtnosisWindow::writeHypervisorSettings()
{
    m_settings.beginGroup("Hypervisor");
    m_settings.beginWriteArray("hypervisors");
    for(int i=0; i<(ui->domainView->model()->rowCount()); i++) {
        m_settings.setArrayIndex(i);
        QVariant var_hypervisor;
        var_hypervisor.setValue(model()->hypervisor(i));
        m_settings.setValue("hypervisor", var_hypervisor);
    }
    m_settings.endArray();
    m_settings.endGroup();
}

void VirtnosisWindow::readHypervisorSettings()
{
    m_settings.beginGroup("Hypervisor");
    int size = m_settings.beginReadArray("hypervisors");
    for (int i = 0; i < size; ++i) {
         m_settings.setArrayIndex(i);
         Hypervisor hypervisor = qvariant_cast<Hypervisor>(m_settings.value("hypervisor"));
         addHypervisor(&hypervisor);
    }
    m_settings.endArray();
    m_settings.endGroup();
}

void VirtnosisWindow::selectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    enableVirtualMachineActions(current);
}

void VirtnosisWindow::on_actionAbout_triggered()
{
    about->show();
}

void VirtnosisWindow::on_actionAboutQt_triggered()
{
    qApp->aboutQt();
}

void VirtnosisWindow::on_actionExit_triggered()
{
    writeSettings();
}
