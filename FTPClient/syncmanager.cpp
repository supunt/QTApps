#include "syncmanager.h"
#include "mainwindow.h"
#include "logger/logger.h"
#include "utils.h"

#include <QDebug>
#include <QProgressBar>
#include <QtNetwork/QNetworkConfiguration>

using namespace std;
//-----------------------------------------------------------------------------------------------------------------------------------------
syncManager::syncManager(QTableWidgetEx* mainViewCtrl,QTableWidgetEx* errorViewCtrl,
                         QTableWidgetEx* startViewCtrl)
{
    _mainViewCtrl = mainViewCtrl;
    _errorViewCtrl = errorViewCtrl;
    _statViewCtrl   = startViewCtrl;

    _directoryScanner = new dirScanner(this);
    QString errStr = "";

    int ftpThrCnt = MainWindow::getSetting("thread_count",QString::number(FTP_DEF_THREAD_COUNT)).toInt();

    _ftpAgents = new ftpSenderDaemon*[ftpThrCnt];

    for (int i = 0; i < ftpThrCnt; ++i)
        _ftpAgents[i] = new ftpSenderDaemon(this,i);

    _cellData = new cellData;
    _cellData->setup(-1,-1,typeid(int).hash_code(),nullptr);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void syncManager::run()
{
     _manager = new QNetworkConfigurationManager;
    connect(_manager,SIGNAL(configurationChanged(const QNetworkConfiguration &)),
            this,SLOT(onNetworkConfigChange(const QNetworkConfiguration &)));

    createStatTimer();
    initNetworkSession();
 }
//-----------------------------------------------------------------------------------------------------------------------------------------
void syncManager::createStatTimer()
{
    _statTimer = new QTimer;
    connect(_statTimer,SIGNAL(timeout()),this,SLOT(onStatTimer()));
    _statTimer->start(STAT_TIMER_INTERVAL*1000);
    report("Stat timer started [Scan interval : " + QString::number(STAT_TIMER_INTERVAL)+
                " seconds]",SYNCMAN,TEXT);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void syncManager::createTransactionTimers()
{
    _scanLoopTimer = new QTimer;
    connect(_scanLoopTimer,SIGNAL(timeout()),this,SLOT(onDiscScanTimer()));
    _syncInterval = MainWindow::getSetting("sync_interval").toInt();
    _scanLoopTimer->start(_syncInterval*1000);
    report("Disk scan timer started [Scan interval : " + QString::number(_syncInterval) +
                " seconds]",SYNCMAN,TEXT);

    _txTimer= new QTimer;
    connect(_txTimer,SIGNAL(timeout()),this,SLOT(onTransferTimer()));
    _txTimer->start(TX_TIMER_INTERVAL*1000);
    report("Transaction timer started [Scan interval : " + QString::number(TX_TIMER_INTERVAL)+
                " seconds]",SYNCMAN,TEXT);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
 void syncManager::onReportDirScanComplete()
 {
        VEC_FI* newFiles = _directoryScanner->getNewlyAddedFiles();

        report("Directory scan run. "+
               ((newFiles->size() == 0) ? "No":QString::number(newFiles->size())) +
               " new files detected.", SYNCMAN, TEXT);

        int num = 0;
         for (auto file : *newFiles)
         {  
             ((QTableWidgetEx*)_mainViewCtrl)->Insert_Row(file,num);
             _mainTransferQueue.push(new PAIR_FI_I(file,num));
         }
         newFiles->clear();


 }
 //-----------------------------------------------------------------------------------------------------------------------------------------
void syncManager::onTransferTimer()
{
    if (!_mainQProcessing)
       processNextInMasterQueue();
}
  //-----------------------------------------------------------------------------------------------------------------------------------------
   void syncManager::processNextInMasterQueue()
   {
       onStatTimer();
       if (_mainTransferQueue.size() == 0)
       {
           _mainQProcessing = false;
            return;
       }

       auto dataPair =_mainTransferQueue.front();
       if (_ftpAgents[0]->getClient() && _ftpAgents[0]->isConnected())
       {
           _mainQProcessing = true;
            _ftpAgents[0]->sendFile(dataPair);
       }
       return;
   }
//-----------------------------------------------------------------------------------------------------------------------------------------
  void syncManager::onFileUploadStatus(PAIR_FI_I* fileinfo, bool status)
  {   
    QProgressBar* wi =(QProgressBar*)_mainViewCtrl->cellWidget(fileinfo->second,3);
    if (!status)
    {

    }
    else
    {
        _filesTransferred++;
        auto dataPair =_mainTransferQueue.front();
        _mainTransferQueue.pop();
        delete dataPair;

        if (fileinfo->first->size() == 0)
        {
            wi->setMaximum(1);
            wi->setValue(1);
        }
        else
        {
            wi->setMaximum(fileinfo->first->size());
            wi->setValue(fileinfo->first->size());
        }
        processNextInMasterQueue();
    }

    onStatTimer();
  }
  //-----------------------------------------------------------------------------------------------------------------------------------------
  void syncManager::onFileUploadProgress(PAIR_FI_I *fileinfo, qint64 now, qint64 total)
  {
        QProgressBar* wi =(QProgressBar*)_mainViewCtrl->cellWidget(fileinfo->second,3);

        wi->setMaximum(total);
        wi->setValue(now);
  }
 //-----------------------------------------------------------------------------------------------------------------------------------------
 void syncManager::report(QString err, SOURCE source, TWE type)
 {
     switch(type)
     {
        case TEXT:
         logger::log(err);
         break;
     case WARNING:
         logger::logWarn(err);
         break;
     case ERROR:
         logger::logError(err);
         break;
     default:
         logger::log(err);
         break;
     }

     if (_errorViewCtrl->isHidden())
     {
         QSize size = _errorViewCtrl->size();
         _errorViewCtrl->show();
    }
     fe_error* er = nullptr;
     if (_lastError && _lastError->_err == err)
     {
         int val = ++_lastError->_count;
          _cellData->setup(_lastError->_index,3,typeid(int).hash_code(), &val);
        _errorViewCtrl->updateCellValue(_cellData);
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
         er->_type = type;

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
        report(err,DIR_SC,ERROR);
 }
//-----------------------------------------------------------------------------------------------------------------------------------------
void syncManager::onScanTimerDurationChanged(int newDuration)
{
    if (_syncInterval != newDuration)
    {
        if (_scanLoopTimer)
            delete _scanLoopTimer;

        report("Sync interval changed from " + QString::number(_syncInterval) +
               " seconds to " + QString::number(newDuration) + " seconds.", SYNCMAN, WARNING);

        _scanLoopTimer = new QTimer;
        connect(_scanLoopTimer,SIGNAL(timeout()),this,SLOT(onDiscScanTimer()));
        _syncInterval = MainWindow::getSetting("sync_interval").toInt();
        _scanLoopTimer->start(_syncInterval*1000);
    }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void syncManager::onNetworkSessionError(QNetworkSession::SessionError error)
{
    report("Connection Error : " + _networkSession->errorString() + "(Network name : " +
                _networkSession->configuration().name() + ", State : "
                ,NTWK, ERROR);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void syncManager::onNetworkConnEstablished()
{
    report("Connected to network '" + _networkSession->configuration().name() +
           "' [ Type : " + _networkSession->configuration().bearerTypeName() + " ].",SYNCMAN, TEXT);

    QString err = "";
    _ftpAgents[0]->startDaemon(err);
    _isNetworkConnected = true;
    onStatTimer();
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
               // connecting to very first adaptor
               ntwkConfig = cfg;
               break;
           }
    }

    if (ntwkConfig.name() == "")
    {
        report("Cannot connnet to network, starting reconnect timer",SYNCMAN, ERROR);
        _netConnTimer = new QTimer;
        connect(_netConnTimer,SIGNAL(timeout()),this,SLOT(onNetworkReconnectTimer()));
        _netConnTimer->start(2000);
        return;
    }
    _networkSession = new QNetworkSession(ntwkConfig,this);

    // Slots for Network session callbacks

    connect(_networkSession,SIGNAL(opened()),this,SLOT(onNetworkConnEstablished()));
    connect(_networkSession, SIGNAL(error(QNetworkSession::SessionError)),
                this, SLOT(onNetworkSessionError(QNetworkSession::SessionError)));

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
    if ( config.bearerTypeName() == "Unknown")
        return;

    QString stateStr = getNetworkStatusString(config.state());

    report("Network connection change detected [ Name : " +
            config.name() + ", Type : " +
           config.bearerTypeName() + ", state : " +
           stateStr + "]", NTWK, (stateStr == "Connected")?SUCCESS:ERROR);

    if (QNetworkConfiguration::Active != config.state())
    {
        _mainQProcessing = false;
        _isNetworkConnected = false;

        if (_ftpAgents[0])
            _ftpAgents[0]->setAsDisconnected();
    }
    onStatTimer();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void syncManager::onFtpInterrupted()
{
    _mainQProcessing = false;
    onStatTimer();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void syncManager::onStatTimer()
{
    if (_statViewCtrl->rowCount() == 0)
       initStatTable();
    else
    {
        QColor qc;
        //-----------------------------------------------------------------------------------------------------------------------------------
        qc.setRgb(255,94,99);
        if (_isNetworkConnected)
            qc.setRgb(111,255,111);

         QString conn = _isNetworkConnected ?"Connected":"Disconnected";
        _cellData->setup(0,1,typeid(QString).hash_code(),
                        &conn, &qc);
        _statViewCtrl->updateCellValue(_cellData);
        //-----------------------------------------------------------------------------------------------------------------------------------
        conn = (_ftpAgents[0] &&  _ftpAgents[0]->isConnected()) ?"Connected":"Disconnected";
        qc.setRgb(255,94,99);
        if (conn == "Connected")
            qc.setRgb(111,255,111);

        _cellData->setup(1,1,typeid(QString).hash_code(),
                         &conn, &qc);

        _statViewCtrl->updateCellValue(_cellData);
        //-----------------------------------------------------------------------------------------------------------------------------------
        int mainQsize =_mainTransferQueue.size();
        _cellData->setup(2,1,typeid(int).hash_code(),
                         &mainQsize);

        _statViewCtrl->updateCellValue(_cellData);
        //-----------------------------------------------------------------------------------------------------------------------------------
        _cellData->setup(3,1,typeid(int).hash_code(),
                         &_filesTransferred);

        _statViewCtrl->updateCellValue(_cellData);
        //-----------------------------------------------------------------------------------------------------------------------------------
    }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void syncManager::onFtpClientConnected()
{
    createTransactionTimers();
    onStatTimer();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void syncManager::initStatTable()
{
    QColor qc;
    //---------------------------------------------------------------------------------------------------------
    void* data =  new QString(_isNetworkConnected?"Connected":"Disconnected");

    if (_isNetworkConnected)
        qc.setRgb(111,255,111);
    else
        qc.setRgb(255,94,99);

    statobject* statObj = new statobject("Network", typeid(QString).hash_code(),
                                        data, &qc);

    _statViewCtrl->Insert_Row(statObj);
    //---------------------------------------------------------------------------------------------------------
    bool status = (_ftpAgents[0] &&  _ftpAgents[0]->isConnected());
    data =  new QString(status ?"Connected":"Disconnected");

    if (status)
        qc.setRgb(111,255,111);
    else
        qc.setRgb(255,94,99);

    statObj = new statobject("FTP", typeid(QString).hash_code(),
                                        data, &qc);
    _statViewCtrl->Insert_Row(statObj);
    //---------------------------------------------------------------------------------------------------------
    data = new int(_mainTransferQueue.size());
    statObj = new statobject("Transfer Queue size", typeid(int).hash_code(),
                                         data);
    _statViewCtrl->Insert_Row(statObj);
    //---------------------------------------------------------------------------------------------------------
    data = new int(_filesTransferred);
    statObj = new statobject("Files transferred", typeid(int).hash_code(),
                                         data);

    _statViewCtrl->Insert_Row(statObj);
}
