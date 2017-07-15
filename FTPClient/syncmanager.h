#ifndef SYNCMANAGER_H
#define SYNCMANAGER_H

#pragma once

#include "dirscanner.h"
#include "abscallback.h"
#include "qtablewidgetex.h"
#include "ftpsenderdaemon.h"

#include <QWidget>
#include <QTimer>
#include <QThread>
#include <QtNetwork/QNetworkConfigurationManager>
#include <QtNetwork/QNetworkSession>
//-----------------------------------------------------------------------------------------------------------------------------------------
class syncManager :  public QObject, public Abscallback
{
    Q_OBJECT
public:
    syncManager(QTableWidgetEx* mainViewCtrl,QTableWidgetEx* errorViewCtrl);

    void setSyncState(bool state) {_syncState = state;};
    bool getSyncState() {return _syncState;};
    bool sendToFTP(QFileInfo* newFiles, int rowNum);
    void run();

    //---------------- AbsCB
    void onReportDirScanComplete();
    void report(QString err, SOURCE source = DIR_SC, TWE type = TEXT);
    void onScanTimerDurationChanged(int newDuration);
    void onUploadStatusNotification(int pgbRow, int percentage);

private:
    dirScanner* _directoryScanner       = nullptr;
    QTableWidgetEx* _mainViewCtrl    = nullptr;
    QTableWidgetEx*  _errorViewCtrl     = nullptr;
    QTimer*   _scanLoopTimer  = nullptr;
    QTimer*   _netConnTimer  = nullptr;
    fe_error*      _lastError = nullptr;
    bool _syncState = true;
    int  _syncInterval = 0;

    void initNetworkSession();
    ftpSenderDaemon** _ftpAgents;
    QNetworkSession* _networkSession    = nullptr;
    QNetworkConfigurationManager* _manager  = nullptr;

private slots:
    void onDiscScanTimer();
    void onNetworkStateChanged(QNetworkSession::State state);
    void onNetworkSessionError(QNetworkSession::SessionError error);
    void onNetworkConnEstablished();
    void onNetworkReconnectTimer();
    void onNetworkConfigChange(const QNetworkConfiguration & config);
};

#endif // SYNCMANAGER_H
