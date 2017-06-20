#ifndef SYNCMANAGER_H
#define SYNCMANAGER_H

#include "dirscanner.h"
#include "abscallback.h"
#include "QWidget"
#include "QTimer"
#include "qtablewidgetex.h"
//-----------------------------------------------------------------------------------------------------------------------------------------
class syncManager :  public QObject, public Abscallback
{
    Q_OBJECT
public:
    syncManager(QTableWidgetEx* mainViewCtrl,QTableWidgetEx* errorViewCtrl);

    void setSyncState(bool state) {_syncState = state;};
    bool getSyncState() {return _syncState;};
    bool sendToFTP(VEC_FI* newFiles);
    void run();

    //---------------- AbsCB
    void onReportDirScanComplete();
    void reportError(QString err, SOURCE source = DIR_SC);
    void onScanTimerDurationChanged(int newDuration);

private:
    dirScanner* _directoryScanner;
    QTableWidgetEx* _mainViewCtrl;
    QTableWidgetEx*  _errorViewCtrl;
    QTimer*   _scanLoopTimer;
    fe_error*      _lastError = nullptr;
    bool _syncState = true;
    int  _syncInterval = 0;

     QString getSource(SOURCE source);

private slots:
    void onDiscScanTimer();
};

#endif // SYNCMANAGER_H
