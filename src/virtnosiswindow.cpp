/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#include <QDebug>
#include "./virtnosiswindow.h"
#include "./ui_virtnosiswindow.h"

// TODO(txwikinger): Refactor threading
/**
 * @brief contructor from virtnosis window object
 * @param parent - parent object
 *
 * The virtnosis window object contains the main window for
 * the virtnosis application. All menus, toolsbars, the status bar
 * as well as the main view of the hypervisors and domains are
 * contained in this window.
 *
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
    connect(ui->domainView->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)),
            this, SLOT(selectionChanged(QModelIndex, QModelIndex)));
    connect(ui->domainView->model(), SIGNAL(dataChanged(QModelIndex, QModelIndex)),
            this, SLOT(selectionChanged(QModelIndex, QModelIndex)));
    connect(&connectingThreadWatcher, SIGNAL(started()), this, SLOT(handleConnectingStarted()));
    connect(&connectingThreadWatcher, SIGNAL(finished()), this, SLOT(handleConnectingFinished()));
    readSettings();
}

/**
 * @brief destroys main window object
 */
VirtnosisWindow::~VirtnosisWindow()
{
    delete about;
    delete ui;
}

/**
 * @brief adds a new hypervisor to the view of this window
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
    if (hypervisor->autoConnect())
    {
        connectHypervisor(index);
    }
}

/**
 * @brief slot to be called when data in the view changes
 */
void VirtnosisWindow::dataChanged()
{
    selectionChanged(currentIndex(), currentIndex());
}

/**
 * @brief sets status message in the window
 * @param text
 */
void VirtnosisWindow::setStatusMessage(QString text)
{
    statusMessage->setText(text);
}

/**
 * @brief shows pop-up menu
 * @param event
 *
 * Reimplemented method that is called when pop-up
 * menu is requested. Depending on the underlying
 * context the pop-menu for a hypervisor, or for
 * a domain is shown.
 */
void VirtnosisWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    if (model()->indexIsHypervisor(currentIndex()))
    {
        menu.addAction(ui->menuHypervisorActionConnect);
        menu.addAction(ui->menuHypervisorActionDisconnect);
        menu.addAction(ui->menuHypervisorActionRemove);
        menu.exec(event->globalPos());
    }
    if (model()->indexIsDomain(currentIndex()))
    {
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
 * @brief slot that is called when the new hypervisor action is called
 *
 * When this slot is called, the Hypervisor dialog is shown
 * and a new hypervisor with the entered information is created
 * and added to the model.
 */
void VirtnosisWindow::on_menuHypervisorActionNew_triggered()
{
    HypervisorDialog *dialog = new HypervisorDialog(this);
    dialog->show();
}

/**
 * @brief slot that is called when Hypervisor is to be connected
 *
 * When this slot is called, the currently selected hypervisor is connected
 */
void VirtnosisWindow::on_menuHypervisorActionConnect_triggered()
{
    connectHypervisor(currentIndex());
}

/**
 * @brief slot that is called when Hypervisor is to be disconnected
 *
 * When this slot is called, the currently selected hypervisor is disconnected
 */
void VirtnosisWindow::on_menuHypervisorActionDisconnect_triggered()
{
    model()->disconnectHypervisor(currentIndex());
}

/**
 * @brief slot that is called when Hypervisor is to be deleted
 *
 * When this slot is called, the currently selected hypervisor is deleted
 */
void VirtnosisWindow::on_menuHypervisorActionRemove_triggered()
{
    model()->removeRow(currentIndex().row());
    writeHypervisorSettings();
}

/**
 * @brief slot that is called when domain is to be started
 */
void VirtnosisWindow::on_menuVmActionStart_triggered()
{
    Domain domain = selectedDomain();
    domain.start();
    dataChanged();
}

/**
 * @brief slot that is called when domain is to be rebooted
 */
void VirtnosisWindow::on_menuVmActionReboot_triggered()
{
    Domain domain = selectedDomain();
    domain.reboot();
    dataChanged();
}

/**
 * @brief slot that is called when domain is to be shutoff
 */
void VirtnosisWindow::on_menuVmActionShutoff_triggered()
{
    Domain domain = selectedDomain();
    domain.shutdown();
    dataChanged();
    checkDomainStateChangeTread = QtConcurrent::run(this,
                                                    &VirtnosisWindow::checkDomainStateChange,
                                                    currentIndex(),
                                                    VIR_DOMAIN_SHUTOFF);
    checkDomainStateChangeTreadWatcher.setFuture(checkDomainStateChangeTread);
}

/**
 * @brief slot that is called when domain is to be forcefully shutoff
 */
void VirtnosisWindow::on_menuVmActionDestroy_triggered()
{
    Domain domain = selectedDomain();
    domain.destroy();
    dataChanged();
    checkDomainStateChangeTread = QtConcurrent::run(this,
                                                    &VirtnosisWindow::checkDomainStateChange,
                                                    currentIndex(),
                                                    VIR_DOMAIN_SHUTOFF);
    checkDomainStateChangeTreadWatcher.setFuture(checkDomainStateChangeTread);
}

/**
 * @brief slot that is called when domain is to be paused
 */
void VirtnosisWindow::on_menuVmActionPause_triggered()
{
    Domain domain = selectedDomain();
    domain.pause();
    dataChanged();
}

/**
 * @brief slot that is called when domain is to be resumed
 */
void VirtnosisWindow::on_menuVmActionResume_triggered()
{
    Domain domain = selectedDomain();
    domain.resume();
    dataChanged();
}

/**
 * @brief enables/disables available actions according to context
 * @param index - index of selected item
 *
 * Depending on the item (hypervisor or domain) and their state
 * certain actions are available which are enabled, or unavailable
 * which are disabled.
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

    if (index.data(DomainViewModel::domainTypeRole) == DomainViewModel::typeHypervisor)
    {
        Hypervisor hypervisor = qvariant_cast<Hypervisor>(
                    index.data(DomainViewModel::domainHypervisorRole));
        ui->menuHypervisorActionRemove->setEnabled(true);
        if (hypervisor.alive())
        {
            ui->menuHypervisorActionDisconnect->setEnabled(true);
        } else {
            ui->menuHypervisorActionConnect->setEnabled(true);
        }
    } else {
        Domain domain = qvariant_cast<Domain>(index.data(DomainViewModel::domainDomainRole));
        if (domain.isRunning())
        {
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
 * @brief provides the selected domain
 * @return selected domain object
 */
Domain VirtnosisWindow::selectedDomain()
{
    QModelIndex index = currentIndex();
    return qvariant_cast<Domain>(index.data(DomainViewModel::domainDomainRole));
}

/**
 * @brief provides the selected hypervisor
 * @return selected hypervisor object
 */
Hypervisor VirtnosisWindow::selectedHypervisor()
{
    QModelIndex index = currentIndex();
    return qvariant_cast<Hypervisor>(index.data(DomainViewModel::domainHypervisorRole));
}

/**
 * @brief triggers refresh of view for item that has changed
 */
void VirtnosisWindow::selectedDataChanged()
{
    QModelIndex index = currentIndex();
    ui->domainView->dataChanged(index, index);
}

/**
 * @brief provides the model used in the view
 * @return model object
 */
DomainViewModel *VirtnosisWindow::model()
{
    if (ui->domainView->model())
    {
        return qobject_cast<DomainViewModel *>(ui->domainView->model());
    }
    return 0;
}

/**
 * @brief provides current index
 * @return currently selected index object
 */
QModelIndex VirtnosisWindow::currentIndex()
{
    return ui->domainView->currentIndex();
}

/**
 * @brief reads the stored settings from previous session
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
 * @brief writes the current settings into storage for next session
 */
void VirtnosisWindow::writeSettings()
{
    m_settings.beginGroup("VirtnosisWindow");
    m_settings.setValue("state", saveState());
    m_settings.endGroup();
    writeHypervisorSettings();
}

/**
 * @brief writes all hypervisors into settings
 */
void VirtnosisWindow::writeHypervisorSettings()
{
    m_settings.beginGroup("Hypervisor");
    m_settings.beginWriteArray("hypervisors");
    for (int i = 0; i < (ui->domainView->model()->rowCount()); i++)
    {
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
 * @brief reads all hypervisors from settings
 */
void VirtnosisWindow::readHypervisorSettings()
{
    m_settings.beginGroup("Hypervisor");
    int size = m_settings.beginReadArray("hypervisors");
    for (int i = 0; i < size; ++i)
    {
         m_settings.setArrayIndex(i);
         Hypervisor hypervisor = qvariant_cast<Hypervisor>(m_settings.value("hypervisor"));
         addHypervisor(&hypervisor);
    }
    m_settings.endArray();
    m_settings.endGroup();
    m_settings.beginGroup("Settings");
    settingsDialog->setChangeTimeout(qvariant_cast<int>(m_settings.value("changeTimeout",
                                                        VIRTNOSIS_DEFAULT_CHANGE_TIME)));
    m_settings.endGroup();
}

/**
 * @brief creates the status bar for the main window
 */
void VirtnosisWindow::createStatusBar()
{
    statusMessage = new QLabel();
    statusBar()->addPermanentWidget(statusMessage);
}

/**
 * @brief connects the given hypervisor
 * @param index - index object refering to hypervisor
 *
 * The connection is created in separate thread since it may take a
 * while to retrieve all the domain data associated
 */
void VirtnosisWindow::connectHypervisor(QModelIndex index)
{
    connectingThread = QtConcurrent::run(model(), &DomainViewModel::connectHypervisor, index);
    connectingThreadWatcher.setFuture(connectingThread);
}

/**
 * @brief checks if domain state changed after command
 * @param index - index object refering to domain
 * @param state - expected state after change
 *
 * This is basically a watchdog functionality that checks if a change
 * action for a domain succeeds. If after a timeout the change has
 * not happened, an alert dialog is shown
 */
void VirtnosisWindow::checkDomainStateChange(QModelIndex index, int state)
{
    if (model())
    {
        QThread::sleep(VIRTNOSIS_DEFAULT_CHANGE_TIME);
        if (model() && model()->data(index, DomainViewModel::domainStateRole) != state)
        {
            QMessageBox dialog;
            dialog.setText(tr("Domain did not change to requested state"));
            dialog.exec();
        }
    }
}

/**
 * @brief calls a system command
 * @param command - string represent the command to be executed
 *
 * This method is used to call external executables
 */
void VirtnosisWindow::displayViewer(QString command)
{
    system(command.toLocal8Bit().data());
}

/**
 * @brief slot that is called when the selection in the view has been changed
 * @param current - current selection
 * @param previous - previous selection
 *
 * When the selection is changed, the available actions are reset
 */
void VirtnosisWindow::selectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    enableVirtualMachineActions(current);
}

/**
 * @brief sets status text when connection to hypervisor is started
 */
void VirtnosisWindow::handleConnectingStarted()
{
    setStatusMessage(tr("Connecting to Hypervisor..."));
}

/**
 * @brief sets status text when connection to hypervisor is finished
 */
void VirtnosisWindow::handleConnectingFinished()
{
    setStatusMessage(tr("Connected to Hypervisor!"));
}

/**
 * @brief slot that is called when About dialog is requested
 *
 * Shows the About dialog
 */
void VirtnosisWindow::on_actionAbout_triggered()
{
    about->show();
}

/**
 * @brief slot that is called when Qt-About dialog is requested
 *
 * Shows the Qt-About dialog
 */
void VirtnosisWindow::on_actionAboutQt_triggered()
{
    qApp->aboutQt();
}

/**
 * @brief slot that is called when the main window is exited
 *
 * When the main window is closed, the settings are written
 * into storage, remaining threads are canceled.
 */
void VirtnosisWindow::on_actionExit_triggered()
{
    writeSettings();
    connectingThreadWatcher.cancel();
    checkDomainStateChangeTreadWatcher.cancel();
}

/**
 * @brief slot that is called when the Preferences dialog is requested
 *
 * Shows the Preferences dialog
 */
void VirtnosisWindow::on_actionPreferences_triggered()
{
    settingsDialog->show();
}

/**
 * @brief slot that is called when the domain viewer is called
 *
 * Currently the virt-viewer is called in a separate thread to
 * keep the main window accessible while the vnc screen of the
 * domain is shown in a different window.
 */
void VirtnosisWindow::on_menuVmActionViewer_triggered()
{
// TODO(txwikinger): temporarily using virt-viewer. Replace with Qt-based implementation
    Domain domain = selectedDomain();
    QString command = QString("virt-viewer -c ");
//    command.append(domain.hypervisor().uri());
    command.append(domain.uri());
    command.append(" ");
    command.append(domain.name());
    qDebug() << "VirtnosisWindow::on_menuVmActionViewer_triggered: command = " << command;
    QtConcurrent::run(this, &VirtnosisWindow::displayViewer, command);
}
