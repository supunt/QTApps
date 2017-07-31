#include "defs.h"
#include "settingsdlg.h"
#include "ui_settingsdlg.h"
#include "logger/logger.h"

#include <QDebug>
#include <QList>
#include <QDir>
#include <mainwindow.h>
#include <QFileDialog>
#include <QAbstractButton>

//-----------------------------------------------------------------------------------------------------------------------------------------
settingsDlg::settingsDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settingsDlg)
{
    ui->setupUi(this);
    InitDefaultSettings();
    LoadSettings();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
settingsDlg::~settingsDlg()
{
    saveSettings();
    delete ui;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void settingsDlg::LoadSettings()
{
    _generalSettings = new QSettings(QApplication::organizationName(),QApplication::applicationName());
    QList<QLineEdit*> listChildern = this->findChildren<QLineEdit*>();

     _generalSettings->beginGroup("generalSettings");
     QString temp = "";
    for (auto child : listChildern)
    {
        temp =_generalSettings->value(child->objectName()).toString();
        if (temp == "")
        {
            auto ite = _defaultSettings.find(child->objectName());
            if  (ite != _defaultSettings.end())
                temp = ite->second;
        }
        child->setText(temp);
        MainWindow::_mapSettings[child->objectName()] = temp;
        qDebug() << "Loaded Setting : " + child->objectName() + " : " + temp;
    }
    _generalSettings->endGroup();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void settingsDlg::saveSettings()
{
    _generalSettings->beginGroup("generalSettings");
    QList<QLineEdit*> listChildern = this->findChildren<QLineEdit*>();

    for (auto child : listChildern)
    {
       _generalSettings->setValue(child->objectName(),child->text());
       MainWindow::updateSetting(child->objectName(),child->text());
    }
    _generalSettings->endGroup();

    doPostChangeSettingValidations();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void settingsDlg::doPostChangeSettingValidations()
{
    _syncManager->onScanTimerDurationChanged(ui->sync_interval->text().toInt());
    _syncManager->onHKTimerDurationChanged(ui->house_keeping->time());
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void settingsDlg::checkSettings()
{
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void settingsDlg::setLogPath()
{
    QString oldPath = ui->log_path->text();
    QString newPath = "";
    newPath = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                   oldPath,
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

    // User cancelled
    if (newPath == "")
        newPath = oldPath;

     ui->log_path->setText(newPath);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void settingsDlg::setBkupPath()
{
    QString oldPath = ui->bkup_path->text();
    QString newPath = "";
    newPath = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                   oldPath,
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

    // User cancelled
    if (newPath == "")
        newPath = oldPath;

     ui->bkup_path->setText(newPath);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void settingsDlg::InitDefaultSettings()
{
    _defaultSettings[ui->sync_interval->objectName()] = QString::number(DEFAULT_SCAN_TIMER_INTERVAL);
    _defaultSettings[ui->log_path->objectName()] = QDir::tempPath() +"/Peercore/FTPClient";
    _defaultSettings[ui->bkup_path->objectName()] = QDir::tempPath() +"/Peercore/FTPClient/daily_backup";
    _defaultSettings[ui->thread_count->objectName()] = QString::number(FTP_DEF_THREAD_COUNT);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
QString settingsDlg::getDefaultSetting(QString key)
{
    auto ite = _defaultSettings.find(key);
    if (ite == _defaultSettings.end())
        return "";
    else
        return ite->second;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void settingsDlg::resetSettings()
{
    QList<QLineEdit*> listChildern = this->findChildren<QLineEdit*>();

    for (auto child : listChildern)
    {
        child->setText(getDefaultSetting(child->objectName()));
        MainWindow::updateSetting(child->objectName(),child->text());
    }

    doPostChangeSettingValidations();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void settingsDlg::onActionBtnClick(QAbstractButton* btn)
{
    if (btn == ui->buttonBox->button(QDialogButtonBox::Reset))
        resetSettings();
    else if (btn == ui->buttonBox->button(QDialogButtonBox::Save))
        saveSettings();
    else if (btn == ui->buttonBox->button(QDialogButtonBox::Cancel))
       hide();
}
