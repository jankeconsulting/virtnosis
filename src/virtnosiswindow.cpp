/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#include "virtnosiswindow.h"
#include "ui_virtnosiswindow.h"

#include <QDebug>

//    TODO: Refactor threading
/**
 * @brief VirtnosisWindow::VirtnosisWindow
 * @param parent
 */
VirtnosisWindow::VirtnosisWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VirtnosisWindow),
    m_settings()
{
    qRegisterMetaType<Hypervisor>("Hypervisor");
    qRegisterMetaTypeStreamOperators<Hypervisor>("Hypervisor");
    qRegisterMetaType<Domain>("Domain");
    qRegisterMetaType<QVector<int> >("QVector<int>");
    ui->setupUi(this);
    setWindowTitle(QString(tr("Virtnosis - Virtual Manchine Manager")));
    about = new AboutDialog(this, VIRTNOSIS_REVISION);
    settingsDialog = new SettingsDialog();
    createStatusBar();

    DomainViewModel *model = new DomainViewModel(this);
    ui->domainView->setModel(model);
    DomainViewItemDelegate *delegate = new DomainViewItemDelegate(this);
    ui->domainView->setItemDelegate(qobject_cast<QAbstractItemDelegate *>(delegate));
    connect(ui->domainView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(selectionChanged(QModelIndex,QModelIndex)));
    connect(ui->domainView->model(), SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(selectionChanged(QModelIndex,QModelIndex)));
    connect(&connectingThreadWatcher, SIGNAL(started()), this, SLOT(handleConnectingStarted()));
    connect(&connectingThreadWatcher, SIGNAL(finished()), this, SLOT(handleConnectingFinished()));
    readSettings();
}

/**
 * @brief VirtnosisWindow::~VirtnosisWindow
 */
VirtnosisWindow::~VirtnosisWindow()
{
    delete about;
    delete ui;
}

/**
 * @brief VirtnosisWindow::addHypervisor
 * @param hypervisor
 */
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
    if(hypervisor->autoConnect()) {
        connectHypervisor(index);
    }
}

/**
 * @brief VirtnosisWindow::dataChanged
 */
void VirtnosisWindow::dataChanged()
{
    selectionChanged(currentIndex(), currentIndex());
}

/**
 * @brief VirtnosisWindow::setStatusMessage
 * @param text
 */
void VirtnosisWindow::setStatusMessage(QString text)
{
    statusMessage->setText(text);
}

/**
 * @brief VirtnosisWindow::contextMenuEvent
 * @param event
 */
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
        menu.addAction(ui->menuVmActionDestroy);
        menu.addAction(ui->menuVmActionPause);
        menu.addAction(ui->menuVmActionResume);
        menu.addAction(ui->menuVmActionViewer);
        menu.exec(event->globalPos());
    }
}

/**
 * @brief VirtnosisWindow::on_menuHypervisorActionNew_triggered
 */
void VirtnosisWindow::on_menuHypervisorActionNew_triggered()
{
    HypervisorDialog *dialog = new HypervisorDialog(this);
    dialog->show();
}

/**
 * @brief VirtnosisWindow::on_menuHypervisorActionConnect_triggered
 */
void VirtnosisWindow::on_menuHypervisorActionConnect_triggered()
{
    connectHypervisor(currentIndex());
}

/**
 * @brief VirtnosisWindow::on_menuHypervisorActionDisconnect_triggered
 */
void VirtnosisWindow::on_menuHypervisorActionDisconnect_triggered()
{
    model()->disconnectHypervisor(currentIndex());
}

/**
 * @brief VirtnosisWindow::on_menuHypervisorActionRemove_triggered
 */
void VirtnosisWindow::on_menuHypervisorActionRemove_triggered()
{
    model()->removeRow(currentIndex().row());
    writeHypervisorSettings();
}

/**
 * @brief VirtnosisWindow::on_menuVmActionStart_triggered
 */
void VirtnosisWindow::on_menuVmActionStart_triggered()
{
    Domain domain = selectedDomain();
    domain.start();
    dataChanged();
}

/**
 * @brief VirtnosisWindow::on_menuVmActionReboot_triggered
 */
void VirtnosisWindow::on_menuVmActionReboot_triggered()
{
    Domain domain = selectedDomain();
    domain.reboot();
    dataChanged();
}

/**
 * @brief VirtnosisWindow::on_menuVmActionShutoff_triggered
 */
void VirtnosisWindow::on_menuVmActionShutoff_triggered()
{
    Domain domain = selectedDomain();
    domain.shutdown();
    dataChanged();
    checkDomainStateChangeTread = QtConcurrent::run(this, &VirtnosisWindow::checkDomainStateChange, currentIndex(), VIR_DOMAIN_SHUTOFF);
    checkDomainStateChangeTreadWatcher.setFuture(checkDomainStateChangeTread);
}

/**
 * @brief VirtnosisWindow::on_menuVmActionDestroy_triggered
 */
void VirtnosisWindow::on_menuVmActionDestroy_triggered()
{
    Domain domain = selectedDomain();
    domain.destroy();
    dataChanged();
    checkDomainStateChangeTread = QtConcurrent::run(this, &VirtnosisWindow::checkDomainStateChange, currentIndex(), VIR_DOMAIN_SHUTOFF);
    checkDomainStateChangeTreadWatcher.setFuture(checkDomainStateChangeTread);
}

/**
 * @brief VirtnosisWindow::on_menuVmActionPause_triggered
 */
void VirtnosisWindow::on_menuVmActionPause_triggered()
{
    Domain domain = selectedDomain();
    domain.pause();
    dataChanged();
}

/**
 * @brief VirtnosisWindow::on_menuVmActionResume_triggered
 */
void VirtnosisWindow::on_menuVmActionResume_triggered()
{
    Domain domain = selectedDomain();
    domain.resume();
    dataChanged();
}

/**
 * @brief VirtnosisWindow::enableVirtualMachineActions
 * @param index
 */
void VirtnosisWindow::enableVirtualMachineActions(const QModelIndex &index)
{
    ui->menuVmActionStart->setDisabled(true);
    ui->menuVmActionReboot->setDisabled(true);
    ui->menuVmActionShutoff->setDisabled(true);
    ui->menuVmActionDestroy->setDisabled(true);
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
            ui->menuVmActionDestroy->setEnabled(true);
            ui->menuVmActionPause->setEnabled(true);
            ui->menuVmActionReboot->setEnabled(true);
        } else if (domain.isPaused()) {
            ui->menuVmActionResume->setEnabled(true);
        } else {
            ui->menuVmActionStart->setEnabled(true);
        }
    }    
}

/**
 * @brief VirtnosisWindow::selectedDomain
 * @return
 */
Domain VirtnosisWindow::selectedDomain()
{
    QModelIndex index = currentIndex();
    return qvariant_cast<Domain>(index.data(DomainViewModel::domainDomainRole));
}

/**
 * @brief VirtnosisWindow::selectedHypervisor
 * @return
 */
Hypervisor VirtnosisWindow::selectedHypervisor()
{
    QModelIndex index = currentIndex();
    return qvariant_cast<Hypervisor>(index.data(DomainViewModel::domainHypervisorRole));
}

/**
 * @brief VirtnosisWindow::selectedDataChanged
 */
void VirtnosisWindow::selectedDataChanged()
{
    QModelIndex index = currentIndex();
    ui->domainView->dataChanged(index, index);
}

/**
 * @brief VirtnosisWindow::model
 * @return
 */
DomainViewModel *VirtnosisWindow::model()
{
    if(ui->domainView->model()) {
        return qobject_cast<DomainViewModel *>(ui->domainView->model());
    }
    return 0;
}

/**
 * @brief VirtnosisWindow::currentIndex
 * @return
 */
QModelIndex VirtnosisWindow::currentIndex()
{
    return ui->domainView->currentIndex();
}

/**
 * @brief VirtnosisWindow::readSettings
 */
void VirtnosisWindow::readSettings()
{
    m_settings.beginGroup("VirtnosisWindow");
    QByteArray state = m_settings.value("state", QByteArray()).toByteArray();
    restoreState(state);
    m_settings.endGroup();
    readHypervisorSettings();
}

/**
 * @brief VirtnosisWindow::writeSettings
 */
void VirtnosisWindow::writeSettings()
{
    m_settings.beginGroup("VirtnosisWindow");
    m_settings.setValue("state", saveState());
    m_settings.endGroup();
    writeHypervisorSettings();
}

/**
 * @brief VirtnosisWindow::writeHypervisorSettings
 */
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
    m_settings.beginGroup("Settings");
    QVariant var_changeTimeout;
    var_changeTimeout.setValue(settingsDialog->changeTimeout());
    m_settings.setValue("changeTimeout", var_changeTimeout);
    m_settings.endGroup();
}

/**
 * @brief VirtnosisWindow::readHypervisorSettings
 */
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
    m_settings.beginGroup("Settings");
    settingsDialog->setChangeTimeout(qvariant_cast<int>(m_settings.value("changeTimeout", VIRTNOSIS_DEFAULT_CHANGE_TIME)));
    m_settings.endGroup();
}

/**
 * @brief VirtnosisWindow::createStatusBar
 */
void VirtnosisWindow::createStatusBar()
{
    statusMessage = new QLabel();
    statusBar()->addPermanentWidget(statusMessage);
}

/**
 * @brief VirtnosisWindow::connectHypervisor
 * @param index
 */
void VirtnosisWindow::connectHypervisor(QModelIndex index)
{
    connectingThread = QtConcurrent::run(model(), &DomainViewModel::connectHypervisor, index);
    connectingThreadWatcher.setFuture(connectingThread);
}

/**
 * @brief VirtnosisWindow::checkDomainStateChange
 * @param index
 * @param state
 */
void VirtnosisWindow::checkDomainStateChange(QModelIndex index, int state)
{
    if(model()) {
        QThread::sleep(VIRTNOSIS_DEFAULT_CHANGE_TIME);
        if(model() && model()->data(index, DomainViewModel::domainStateRole) != state) {
            QMessageBox dialog;
            dialog.setText(tr("Domain did not change to requested state"));
            dialog.exec();
        }
    }
}

/**
 * @brief VirtnosisWindow::displayViewer
 * @param command
 */
void VirtnosisWindow::displayViewer(QString command)
{
    system(command.toLocal8Bit().data());
}

/**
 * @brief VirtnosisWindow::selectionChanged
 * @param current
 * @param previous
 */
void VirtnosisWindow::selectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    enableVirtualMachineActions(current);
}

/**
 * @brief VirtnosisWindow::handleConnectingStarted
 */
void VirtnosisWindow::handleConnectingStarted()
{
    setStatusMessage(tr("Connecting to Hypervisor..."));
}

/**
 * @brief VirtnosisWindow::handleConnectingFinished
 */
void VirtnosisWindow::handleConnectingFinished()
{
    setStatusMessage(tr("Connected to Hypervisor!"));
}

/**
 * @brief VirtnosisWindow::on_actionAbout_triggered
 */
void VirtnosisWindow::on_actionAbout_triggered()
{
    about->show();
}

/**
 * @brief VirtnosisWindow::on_actionAboutQt_triggered
 */
void VirtnosisWindow::on_actionAboutQt_triggered()
{
    qApp->aboutQt();
}

/**
 * @brief VirtnosisWindow::on_actionExit_triggered
 */
void VirtnosisWindow::on_actionExit_triggered()
{
    writeSettings();
    connectingThreadWatcher.cancel();
    checkDomainStateChangeTreadWatcher.cancel();
}

/**
 * @brief VirtnosisWindow::on_actionPreferences_triggered
 */
void VirtnosisWindow::on_actionPreferences_triggered()
{
    settingsDialog->show();
}

/**
 * @brief VirtnosisWindow::on_menuVmActionViewer_triggered
 */
void VirtnosisWindow::on_menuVmActionViewer_triggered()
{
//    TODO: temporarily using virt-viewer. Replace with Qt-based implementation
    Domain domain = selectedDomain();
    QString command = QString("virt-viewer -c ");
//    command.append(domain.hypervisor().uri());
    command.append(domain.uri());
    command.append(" ");
    command.append(domain.name());
    qDebug() << "VirtnosisWindow::on_menuVmActionViewer_triggered: command = " << command;
    QtConcurrent::run(this, &VirtnosisWindow::displayViewer, command);
}

