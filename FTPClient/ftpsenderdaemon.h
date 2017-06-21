#ifndef FTPSENDERDAEMON_H
#define FTPSENDERDAEMON_H

#include <QObject>

#include "qthreadex.h"
#include "abscallback.h"

class ftpSenderDaemon : public QObject
{
    Q_OBJECT
public:
    explicit ftpSenderDaemon(Abscallback* cb, int chThreadID);
    QThreadEx* getThread(){return _thread;};
    int getTID() {return _tid;};
    bool startDaemon(QString& err);

private:
    QThreadEx* _thread;
    Abscallback* _syncManager;
    int _tid;
signals:

public slots:
};

#endif // FTPSENDERDAEMON_H
