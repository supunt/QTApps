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
#include <queue>

#define STAT_TIMER_INTERVAL 5

class QFileInfo;
typedef std::pair<QFileInfo*,int> PAIR_FI_I;
//-----------------------------------------------------------------------------------------------------------------------------------------
class syncManager :  public QObject, public Abscallback
{
    Q_OBJECT
public:
    syncManager(QTableWidgetEx* mainViewCtrl,QTableWidgetEx* errorViewCtrl,
                QTableWidgetEx* startViewCtrl);

    void setSyncState(bool state) {_syncState = state;};
    bool getSyncState() {return _syncState;};
    void run();

    //---------------- AbsCB
    void onReportDirScanComplete();
    void report(QString err, SOURCE source = DIR_SC, TWE type = TEXT);
    void onScanTimerDurationChanged(int newDuration);
    void onFileUploadStatus(PAIR_FI_I* fileinfo, bool status);
    void onFileUploadProgress(PAIR_FI_I* fileinfo, qint64 now, qint64 total);
    void onFtpClientConnected();
    void onFtpInterrupted();

private:
    dirScanner* _directoryScanner       = nullptr;
    QTableWidgetEx* _mainViewCtrl    = nullptr;
    QTableWidgetEx*  _errorViewCtrl     = nullptr;
    QTableWidgetEx*  _statViewCtrl     = nullptr;
    QTimer*   _scanLoopTimer  = nullptr;
    QTimer*   _netConnTimer  = nullptr;
    QTimer*   _ftpConnTimer  = nullptr;
    QTimer*   _statTimer = nullptr;
    fe_error*      _lastError = nullptr;
    bool _syncState = true;
    int  _syncInterval = 0;
    bool _mainQProcessing = false;
    bool _isNetworkConnected = false;

    void initNetworkSession();
    void processNextInMasterQueue();
    void createTimers();
    ftpSenderDaemon** _ftpAgents;
    QNetworkSession* _networkSession    = nullptr;
    QNetworkConfigurationManager* _manager  = nullptr;

    std::queue<PAIR_FI_I*> _mainTransferQueue;
    std::queue<PAIR_FI_I*> _retryTransferQueue;

private slots:
    void onDiscScanTimer();
    void onNetworkSessionError(QNetworkSession::SessionError error);
    void onNetworkConnEstablished();
    void onNetworkReconnectTimer();
    void onNetworkConfigChange(const QNetworkConfiguration & config);
    void onStatTimer();
};

#endif // SYNCMANAGER_H
