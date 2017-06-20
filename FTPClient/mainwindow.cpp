#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QDebug>
#include <QLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QPalette>
//-----------------------------------------------------------------------------------------------------------------------------------------
QString MainWindow::g_scanDirPath = "";
bool MainWindow::g_mainTblClr = false;
MAP_STR_STR MainWindow::_mapSettings;
//-----------------------------------------------------------------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //----------------------------------------------------------
    QCoreApplication::setOrganizationName("PeerCore");
    QCoreApplication::setOrganizationDomain("peercore.com.au");
    QCoreApplication::setApplicationName("ftp_sync_mgr");
    //----------------------------------------------------------

    ui->setupUi(this);
    _tableWidget = new QTableWidgetEx(this);
    _errortableWidget = new QTableWidgetEx(this);
    _settingsDlg = new settingsDlg(this);
    _settingsDlg->hide();

    LoadSettings();

    initMainTableHeaders();
    initErrorTableHeaders();
   _errortableWidget->hide();

    QVBoxLayout *layout = new QVBoxLayout;
    QWidget* topLayout = new QWidget;
    topLayout->setLayout(ui->topLayout);

    layout->addWidget(topLayout);
    layout->addWidget(_tableWidget);
    layout->addWidget(_errortableWidget);

    QWidget* layedoutWindow = new QWidget;
    layedoutWindow->setLayout(layout);
    setCentralWidget(layedoutWindow);

    SetDefaultDirPath();
    ui->txt_path->setText(MainWindow::g_scanDirPath);

     _syncMan = new syncManager(_tableWidget,_errortableWidget);
    _settingsDlg->setCallback(_syncMan);

     QPalette pal = ui->btnSync->palette();
     pal.setColor(QPalette::Button, QColor(Qt::red));
     ui->btnSync->setAutoFillBackground(true);
     ui->btnSync->setPalette(pal);
     ui->btnSync->update();

     //------------------------------------------------------------
     _syncMan->run();
 }
//-----------------------------------------------------------------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
   // delete _syncMan;
    delete _settingsDlg;
    SaveSettings();
    delete ui;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void MainWindow::LoadSettings()
{
    _appSetting = new QSettings(QCoreApplication::organizationName(),QCoreApplication::applicationName());

    const QSize defaultSize =  this->size();
    const QPoint defaultloc =  this->pos();
     _appSetting->beginGroup("MainWindow");

     resize(_appSetting->value("size", defaultSize).toSize());
     move(_appSetting->value("pos", defaultloc).toPoint());

     ui->txt_path->setText(_appSetting->value("syncDir", MainWindow::g_scanDirPath).toString());
      MainWindow::g_scanDirPath = ui->txt_path->text();
     _mainTableColumns = _appSetting->value("mainTableCols").toStringList();
     _appSetting->endGroup();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void MainWindow::SaveSettings()
{
    _appSetting->beginGroup("MainWindow");
    _appSetting->setValue("size", size());
    _appSetting->setValue("pos", pos());
    _appSetting->setValue("syncDir",ui->txt_path->text());
    _appSetting->setValue("mainTableCols",_mainTableColumns);
    _appSetting->endGroup();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
 void MainWindow::initMainTableHeaders()
 {
     if (_mainTableColumns.size() == 0) // first run has no columns registered
          _mainTableColumns << "File Name" << "File Size" << "Last modified time" << "Status" << "Progress";

     _tableWidget->setColumnCount(_mainTableColumns.size());

     _tableWidget->setHorizontalHeaderLabels(_mainTableColumns);
     for (int i = 0; i < _mainTableColumns.size() ; i++)
         _tableWidget->horizontalHeaderItem(i)->setTextAlignment(Qt::AlignLeft);

        _tableWidget->resizeRowsToContents();
        _tableWidget->resizeColumnsToContents();
 }
 //----------------------------------------------------------------------------------------------------------------------------------------
  void MainWindow::initErrorTableHeaders()
  {
      QStringList titles;
      titles << "Reporter" << "Error Message" <<  "Last reported time" << "Times Repeated";
      _errortableWidget->setColumnCount(4);

      _errortableWidget->setHorizontalHeaderLabels(titles);
      for (int i = 0; i < 4 ; i++)
         _errortableWidget->horizontalHeaderItem(i)->setTextAlignment(Qt::AlignLeft);

         _errortableWidget->resizeRowsToContents();
         _errortableWidget->resizeColumnsToContents();
         _errortableWidget->setUpdatesEnabled(true);
  }
  //----------------------------------------------------------------------------------------------------------------------------------------
   void MainWindow::onClickSetDirectory()
   {
       QString oldPath = MainWindow::g_scanDirPath;
       _syncMan->setSyncState(false);
       MainWindow::g_scanDirPath = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                       MainWindow::g_scanDirPath,
                                                       QFileDialog::ShowDirsOnly
                                                       | QFileDialog::DontResolveSymlinks);

       // User cancelled
       if (MainWindow::g_scanDirPath == "")
            MainWindow::g_scanDirPath = oldPath;

        ui->txt_path->setText(MainWindow::g_scanDirPath);

        if (oldPath != MainWindow::g_scanDirPath)
            MainWindow::g_mainTblClr = !MainWindow::g_mainTblClr; // flip color to indicate dir change

        _syncMan->setSyncState(true);
   }
  //----------------------------------------------------------------------------------------------------------------------------------------
void MainWindow::onChangeSyncState()
{
    QPalette pal = ui->btnSync->palette();
    if (_syncMan->getSyncState())
    {
        ui->btnSync->setText("Start sync");
        _syncMan->setSyncState(false);
        pal.setColor(QPalette::Button, QColor(Qt::green));
    }
    else
    {
         ui->btnSync->setText("Pause sync");
         _syncMan->setSyncState(true);
          pal.setColor(QPalette::Button, QColor(Qt::red));
    }
    ui->btnSync->setAutoFillBackground(true);
    ui->btnSync->setPalette(pal);
    ui->btnSync->update();
}
//----------------------------------------------------------------------------------------------------------------------------------------
void MainWindow::SetDefaultDirPath()
{
    MainWindow::g_scanDirPath = QString(getenv("USERPROFILE")) + "\\Desktop\\files";
}
//----------------------------------------------------------------------------------------------------------------------------------------
void MainWindow::onDirPathChange()
{
    QString oldPath = MainWindow::g_scanDirPath;

    MainWindow::g_scanDirPath = ui->txt_path->text();

    if (oldPath != MainWindow::g_scanDirPath)
        MainWindow::g_mainTblClr = !MainWindow::g_mainTblClr; // flip
}
//----------------------------------------------------------------------------------------------------------------------------------------
void MainWindow::on_actionSettings_triggered()
{
     _settingsDlg->show();
}
//----------------------------------------------------------------------------------------------------------------------------------------
QString MainWindow::getSetting(QString key)
{
    MAP_STR_STR::iterator ite = MainWindow::_mapSettings.find(key);
    if (ite == MainWindow::_mapSettings.end())
        return "";
    else
        return ite->second;
}

//----------------------------------------------------------------------------------------------------------------------------------------
void MainWindow::updateSetting(QString key, QString value)
{
    MAP_STR_STR::iterator ite = MainWindow::_mapSettings.find(key);
    if (ite == MainWindow::_mapSettings.end())
        return;
    else
        ite->second = value;
}
