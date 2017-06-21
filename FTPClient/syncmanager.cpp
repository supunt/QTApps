#include "syncmanager.h"
#include "QDebug"
#include "QProgressBar"
#include "mainwindow.h"

//-----------------------------------------------------------------------------------------------------------------------------------------
syncManager::syncManager(QTableWidgetEx* mainViewCtrl,QTableWidgetEx* errorViewCtrl)
{
    _mainViewCtrl = mainViewCtrl;
    _errorViewCtrl = errorViewCtrl;
    _directoryScanner = new dirScanner(this);
    QString errStr = "";

    int ftpThrCnt = MainWindow::getSetting("thread_count",QString::number(FTP_DEF_THREAD_COUNT)).toInt();

    _ftpAgents = new ftpSenderDaemon*[ftpThrCnt];
    QString err = "";
    for (int i = 0; i < ftpThrCnt; ++i)
    {
        _ftpAgents[i] = new ftpSenderDaemon(this,i);
        if(!_ftpAgents[i]->startDaemon(err))
        {
            // report to FE TODO
            terminate();
        }
    }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void syncManager::run()
{
    _scanLoopTimer = new QTimer;
    connect(_scanLoopTimer,SIGNAL(timeout()),this,SLOT(onDiscScanTimer()));
    _syncInterval = MainWindow::getSetting("sync_interval").toInt();
    _scanLoopTimer->start(_syncInterval*1000);
 }
//-----------------------------------------------------------------------------------------------------------------------------------------
 void syncManager::onReportDirScanComplete()
 {
        VEC_FI* newFiles = _directoryScanner->getNewlyAddedFiles();

        int num = 0;
         for (auto file : *newFiles)
             ((QTableWidgetEx*)_mainViewCtrl)->Insert_Row(file,num);

        sendToFTP(newFiles);
 }
 //-----------------------------------------------------------------------------------------------------------------------------------------
  bool syncManager::sendToFTP(VEC_FI* newFiles)
  {
          int row = _mainViewCtrl->rowCount() - newFiles->size();
          for (auto file : *newFiles)
          {
              QWidget* wi =_mainViewCtrl->cellWidget(row,3);
              for (int i= 0; i < 100; i++)
                ((QProgressBar*)wi)->setValue(i);

              row++;
          }

          return true;
  }
 //-----------------------------------------------------------------------------------------------------------------------------------------
 void syncManager::reportError(QString err, SOURCE source)
 {
     if (_errorViewCtrl->isHidden())
     {
         QSize size = _errorViewCtrl->size();
         _errorViewCtrl->show();
    }
     fe_error* er = nullptr;
     if (_lastError && _lastError->_err == err)
     {
            _errorViewCtrl->updateCellValue(_lastError->_index,3,QString::number(++_lastError->_count));
            return;
     }
     else
     {
         if (_lastError)
            delete _lastError;

         _lastError = nullptr;
         er = new fe_error;
         er->_source = getSource(source);
         er->_err = err;
         er->_count = 1;
         er->_dt = QDateTime::currentDateTime();
         _lastError = er;
         ((QTableWidgetEx*)_errorViewCtrl)->Insert_Row(er,er->_index);
    }
 }
//-----------------------------------------------------------------------------------------------------------------------------------------
 void syncManager::onDiscScanTimer()
 {
     if (!getSyncState())
         return;

     QString err = "";
    if (!_directoryScanner->OnSyncTimer(err))
        qDebug() << err;
 }
 //-----------------------------------------------------------------------------------------------------------------------------------------
 QString syncManager::getSource(SOURCE source)
 {
     switch (source) {
     case DIR_SC:
         return "File Scanner";
         break;
     case FTP:
         return "FTP";
         break;
     default:
         return "OTHER";
         break;
     }
 }
//-----------------------------------------------------------------------------------------------------------------------------------------
void syncManager::onScanTimerDurationChanged(int newDuration)
{
    if (_syncInterval != newDuration)
    {
        if (_scanLoopTimer)
            delete _scanLoopTimer;

        _scanLoopTimer = new QTimer;
        connect(_scanLoopTimer,SIGNAL(timeout()),this,SLOT(onDiscScanTimer()));
        _syncInterval = MainWindow::getSetting("sync_interval").toInt();
        _scanLoopTimer->start(_syncInterval*1000);
    }
}
