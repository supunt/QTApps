#include "ftpsenderdaemon.h"
#include "mainwindow.h"
#include <QDebug>

//-----------------------------------------------------------------------------------------------------------------------------------------
ftpSenderDaemon::ftpSenderDaemon(Abscallback* cb, int chThreadID)
{
    _tid = chThreadID;
    _syncManager = cb;
    _thread =  new QThreadEx(_tid);
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
