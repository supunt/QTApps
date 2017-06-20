#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qtablewidgetex.h"
#include "syncmanager.h"
#include "settingsdlg.h"
#include "QString"
#include <QSettings>

typedef std::map<QString,QString> MAP_STR_STR;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    static QString g_scanDirPath;
    static bool g_mainTblClr;
    static MAP_STR_STR _mapSettings;
    static QString getSetting(QString key);
    static void updateSetting(QString key, QString value);
private:
    Ui::MainWindow *ui;
    QTableWidgetEx* _tableWidget;
    QTableWidgetEx* _errortableWidget;
    syncManager* _syncMan;
    QSettings* _appSetting;
    QStringList _mainTableColumns;

     void initMainTableHeaders();
     void initErrorTableHeaders();
     void SetDefaultDirPath();
     void LoadSettings();
     void SaveSettings();
     settingsDlg* _settingsDlg;

     //-------------------------------------------------------------
     void DefaultAppSettings();
    bool LoadAppSettings();
    bool SaveAppSettings();

private slots:
    void onClickSetDirectory();
    void onChangeSyncState();
    void onDirPathChange();
    void on_actionSettings_triggered();
};

#endif // MAINWINDOW_H
