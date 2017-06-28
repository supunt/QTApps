#include "ftpsenderdaemon.h"
#include "mainwindow.h"
#include <QDebug>

//-----------------------------------------------------------------------------------------------------------------------------------------
ftpSenderDaemon::ftpSenderDaemon(Abscallback* cb, int chThreadID,QNetworkSession* session)
{
    _tid = chThreadID;
    _syncManager = cb;
    _thread =  new QThreadEx(_tid);
    _ntwkSesstion = session;

    moveToThread(_thread);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
bool ftpSenderDaemon::startDaemon(QString& err)
{
    err = "";
    _thread->start();
    return true;
}
/*-----------------------------------------------------------------------------------------------------------------------------------------
bool ftpSenderDaemon::pauseDaemon(QString& err)
{
    err = "";
    _thread->start();
    return true;
}
*/
