#include "syncmanager.h"
#include "mainwindow.h"

#include <QDebug>
#include <QProgressBar>
#include <QtNetwork/QNetworkConfiguration>
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
        _ftpAgents[i] = new ftpSenderDaemon(this,i,_networkSession);
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

     _manager = new QNetworkConfigurationManager;
    connect(_manager,SIGNAL(configurationChanged(const QNetworkConfiguration &)),
            this,SLOT(onNetworkConfigChange(const QNetworkConfiguration &)));

    initNetworkSession();
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

     if (!_networkSession || !_networkSession->isOpen())
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
     case NTWK:
         return "Network";
         break;
     default:
         return "Other";
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
//-----------------------------------------------------------------------------------------------------------------------------------------
void syncManager::onUploadStatusNotification(int pgbRow, int percentage)
{

}
//-----------------------------------------------------------------------------------------------------------------------------------------
void syncManager::onNetworkStateChanged(QNetworkSession::State state)
{
    qDebug() << "Network changed";
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void syncManager::onNetworkSessionError(QNetworkSession::SessionError error)
{
    reportError("Connection Error : " + _networkSession->errorString() + "(Network name : " +
                _networkSession->configuration().name() + ", State : "
                ,NTWK);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void syncManager::onNetworkConnEstablished()
{
        qDebug() <<  "Connected : " << _networkSession->configuration().name();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void syncManager::initNetworkSession()
{
    if (_netConnTimer)
        delete _netConnTimer;

    if (_networkSession)
        delete _networkSession;

    QSettings networkSettings;
    networkSettings.beginGroup("network");
    const QString defNtwk = networkSettings.value("DefaultNetConfig").toString();
    networkSettings.endGroup();

    QNetworkConfiguration ntwkConfig = _manager->configurationFromIdentifier(defNtwk);
    if ((ntwkConfig.state() & QNetworkConfiguration::Discovered) != QNetworkConfiguration::Discovered)
    {
           QList<QNetworkConfiguration> ntConf =_manager->allConfigurations(QNetworkConfiguration::Active);
           for (auto cfg : ntConf)
           {
               // get rid of VMWare adapters
               if (QNetworkConfiguration::BearerUnknown == cfg.bearerType())
                   continue;
               // TODO -  connecting to very first adaptor
               qDebug() << cfg.bearerType() << "|" <<cfg.name();
               ntwkConfig = cfg;
               break;
           }
    }

    if (ntwkConfig.name() == "")
    {
        _netConnTimer = new QTimer;
        connect(_netConnTimer,SIGNAL(timeout()),this,SLOT(onNetworkReconnectTimer()));
        _netConnTimer->start(2000);
    }
    _networkSession = new QNetworkSession(ntwkConfig,this);

    // Slots for Network session callbacks

    connect(_networkSession,SIGNAL(opened()),this,SLOT(onNetworkConnEstablished()));
    connect(_networkSession, SIGNAL(error(QNetworkSession::SessionError)),
                this, SLOT(onNetworkSessionError(QNetworkSession::SessionError)));
    connect(_networkSession,SIGNAL(stateChanged(QNetworkSession::State)),
                this,SLOT(onNetworkStateChanged(QNetworkSession::State)));

    _networkSession->open();
}

//-----------------------------------------------------------------------------------------------------------------------------------------
void syncManager::onNetworkReconnectTimer()
{
    initNetworkSession();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void syncManager::onNetworkConfigChange(const QNetworkConfiguration &config)
{
    qDebug() << "NTC C : " << config.name() <<", States : " <<  config.state() << ", family : " << config.bearerTypeFamily();
    // fires for the adaptor family
}
