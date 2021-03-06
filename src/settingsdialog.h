/**
 * Copyright (c) 2014 Ralph Janke. All rights reserved.
 *
 * Author: Ralph Janke virtnosis@jankeconsulting.ca
 */

#ifndef SRC_SETTINGSDIALOG_H_
#define SRC_SETTINGSDIALOG_H_

#include <QDialog>

#ifndef VIRTNOSIS_DEFAULT_CHANGE_TIME
#define VIRTNOSIS_DEFAULT_CHANGE_TIME 30
#endif

namespace Ui
{
class SettingsDialog;
}

/**
 * @brief The SettingsDialog class
 */
class SettingsDialog : public QDialog
{
    Q_OBJECT

 public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();
    int changeTimeout();
    void setChangeTimeout(int value);

 private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

 private:
    Ui::SettingsDialog *ui;
    int m_changeTimeout;
};

#endif  // SRC_SETTINGSDIALOG_H_
