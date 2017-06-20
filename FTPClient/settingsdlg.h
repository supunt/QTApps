#ifndef SETTINGSDLG_H
#define SETTINGSDLG_H

#include <QDialog>
#include <QString>
#include <map>
#include <QSettings>
#include <abscallback.h>

typedef std::map<QString,QString> MAP_STR_STR;

namespace Ui {
class settingsDlg;
}

class settingsDlg : public QDialog
{
    Q_OBJECT

public:
    explicit settingsDlg(QWidget *parent = 0);
    ~settingsDlg();

    void setCallback(Abscallback* cb) {_syncManager = cb;};

private:
    Ui::settingsDlg *ui;

    QSettings* _generalSettings;
    void LoadSettings();

     void doPostChangeSettingValidations();
    Abscallback* _syncManager = nullptr;

    MAP_STR_STR _defaultSettings;

private slots:
    void SaveSettings();
    void checkSettings();
    void setLogPath();
    void InitDefaultSettings();
};

#endif // SETTINGSDLG_H
