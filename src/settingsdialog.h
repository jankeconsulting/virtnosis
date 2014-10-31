/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

#ifndef VIRTNOSIS_DEFAULT_CHANGE_TIME
#define VIRTNOSIS_DEFAULT_CHANGE_TIME 30
#endif

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();
    int changeTimeout();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::SettingsDialog *ui;
    int m_changeTimeout;
};

#endif // SETTINGSDIALOG_H
