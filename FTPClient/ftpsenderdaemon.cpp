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
    _ntwkSesstion = session;


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
    thread_start();
    return true;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void ftpSenderDaemon::thread_start()
{
    _ftp = new QFtp(this);

     connect(_ftp, SIGNAL(commandFinished(int,bool)), this, SLOT(ftpCommandFinished(int,bool)));
    _ftp->connectToHost(_host, 21);
    _ftp->login(_user,_pass);

    return;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void ftpSenderDaemon::ftpCommandFinished(int comID, bool error)
{
    if (_ftp->currentCommand() == QFtp::ConnectToHost)
    {
        if (error)
        {
            // TODO NOT THREAD SAFE
            _syncManager->report("Ftp connection of daemon " + QString::number(_tid) + " failed.",FTP,ERROR);
            return;
        }
        else
        {
            _syncManager->report("Ftp daemon " + QString::number(_tid) +
                                 " connected to FTP service at " + MainWindow::getSetting("ftp_host")
                                 ,FTP,SUCCESS);
        }
    }

    if (_ftp->currentCommand() == QFtp::Login)
    {
        if (error)
        {
            // TODO NOT THREAD SAFE
            _syncManager->report("Ftp connection of daemon " + QString::number(_tid) + " failed.",FTP,ERROR);
            return;
        }
        else
        {
            _syncManager->report("Ftp daemon " + QString::number(_tid) +
                                 " logged in to FTP service at " + MainWindow::getSetting("ftp_host")
                                 ,FTP,SUCCESS);
        }
    }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
