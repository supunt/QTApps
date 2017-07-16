#include "ftpsenderdaemon.h"
#include "mainwindow.h"
#include "syncmanager.h"

#include <QDebug>

//-----------------------------------------------------------------------------------------------------------------------------------------
ftpSenderDaemon::ftpSenderDaemon(Abscallback* cb, int chThreadID,QNetworkSession* session)
{
    _tid = chThreadID;
    _syncManager = (syncManager*)cb;
    _thread =  new QThreadEx(_tid);
 }
//-----------------------------------------------------------------------------------------------------------------------------------------
bool ftpSenderDaemon::startDaemon(QString& err)
{
    err = "";
    _host = MainWindow::getSetting("ftp_host");
    _user = MainWindow::getSetting("ftp_user");
    _pass = MainWindow::getSetting("ftp_pass");

   // this->moveToThread(_thread);
    //connect(_thread,SIGNAL(started()), this,SLOT(thread_start()));
    //_thread->start();
    init();
    return true;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void ftpSenderDaemon::init()
{
    if (!_ftp)
    {
        _ftp = new QFtp(this);
         connect(_ftp, SIGNAL(commandFinished(int,bool)), this, SLOT(ftpCommandFinished(int,bool)));
         connect(_ftp, SIGNAL(dataTransferProgress(qint64,qint64)),
                            this, SLOT(onFtpDataTransferProgress(qint64,qint64)));
         connect(_ftp, SIGNAL(stateChanged(int)),
                            this, SLOT(onFtpStateChanged(int)));
     }

    _ftp->connectToHost(_host, 21);
    return;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void ftpSenderDaemon::onFtpStateChanged(int state)
{
    switch(state)
    {
        case QFtp::Unconnected: qDebug() << "unconnected"; break;
        case QFtp::HostLookup: qDebug() << "HostLookup"; break;
        case QFtp::Connecting: qDebug() << "Connecting"; break;
        case QFtp::Connected: qDebug() << "Connected"; break;
        case QFtp::LoggedIn: qDebug() << "LoggedIn"; break;
        case QFtp::Closing: qDebug() << "Closing"; break;
    default : break;
    }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void ftpSenderDaemon::ftpCommandFinished(int comID, bool error)
{
    if (_ftp->currentCommand() == QFtp::ConnectToHost)
    {
        if (error)
        {
            // TODO NOT THREAD SAFE
            _syncManager->report("Ftp host " + _host + " unreachable.",FTP,ERROR);

            if (!_reconnectTimer)
            {
                _reconnectTimer = new QTimer;
                connect(_reconnectTimer,SIGNAL(timeout()),this,SLOT(onReconnectTimer()));
                _reconnectTimer->start(2000);
            }
            return;
        }
        else
        {
            _syncManager->report("Ftp daemon " + QString::number(_tid) +
                                 " connected to FTP service at " + _host
                                 ,FTP,TEXT);

           if (_reconnectTimer)
           {
               delete _reconnectTimer;
               _reconnectTimer = nullptr;
           }
            _ftp->login(_user,_pass);
        }
    }
    else if (_ftp->currentCommand() == QFtp::Login)
    {
        if (error)
        {
            // TODO NOT THREAD SAFE
            _syncManager->report("Ftp login failed. Please check login detains in settings window",FTP,ERROR);

            if (!_reconnectTimer)
            {
                _reconnectTimer = new QTimer();
                connect(_reconnectTimer,SIGNAL(timeout()),this,SLOT(onReconnectTimer()));
                _reconnectTimer->start(2000);
            }
            return;
        }
        else
        {
            _syncManager->report("Ftp daemon " + QString::number(_tid) +
                                 " logged in to FTP service at " + MainWindow::getSetting("ftp_host")
                                 ,FTP,TEXT);

            if (_reconnectTimer)
            {
                delete _reconnectTimer;
                _reconnectTimer = nullptr;
            }
            return;
        }
    }
    else if (_ftp->currentCommand() == QFtp::Put)
    {
        if (error)
        {
            // TODO NOT THREAD SAFE
            _syncManager->report("File '" + _currentFileInfo.first->filePath() +
                                 "' sending failed. ["+ _ftp->errorString() + "].",FTP,ERROR);

            _file->close();
            _file = nullptr;
            _syncManager->onFileUploadStatus(&_currentFileInfo, !error);
            return;
        }
        else
        {
            _syncManager->report("File '" + _currentFileInfo.first->filePath() +
                                 "' sent successfully.",FTP,TEXT);

            _file->close();
            _file = nullptr;
            _syncManager->onFileUploadStatus(&_currentFileInfo, !error);
            return;
        }
    }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void ftpSenderDaemon::sendFile(PAIR_FI_I* fileInfo)
{
    _currentFileInfo.first = fileInfo->first;
    _currentFileInfo.second = fileInfo->second;

    _file = new QFile(_currentFileInfo.first->absoluteFilePath(),this);
    _file->open(QFile::ReadOnly);
    if (!_file->isOpen())
    {
        _syncManager->report("File " + _currentFileInfo.first->filePath() +
                             " does not exist for transfer "
                             ,FTP,ERROR);
        return;
    }
    _ftp->put(_file,_currentFileInfo.first->fileName());
}

//-----------------------------------------------------------------------------------------------------------------------------------------
void ftpSenderDaemon::onFtpDataTransferProgress(qint64 now,qint64 total)
{
    _syncManager->onFileUploadProgress(&_currentFileInfo,now,total);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void ftpSenderDaemon::onReconnectTimer()
{
    init();
}
