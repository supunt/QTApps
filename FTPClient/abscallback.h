#ifndef ABSCALLBACK_H
#define ABSCALLBACK_H

#include "defs.h"
#include "QString"

class Abscallback
{
public:
    Abscallback();
    virtual void onReportDirScanComplete() = 0;       // This is for discScanner
    virtual void reportError(QString err, SOURCE source) = 0;      // this is for both discScanner and ftpSyncMgr
    virtual void onScanTimerDurationChanged(int newDuration) = 0;

};

#endif // ABSCALLBACK_H
