#ifndef FTPSENDERDAEMON_H
#define FTPSENDERDAEMON_H

#include <QObject>
#include <QFile>

#include "qt4_legacy/qftp.h"
#include "qthreadex.h"
#include "abscallback.h"
#include <QtNetwork/QNetworkSession>
#include <QProgressBar>

class ftpSenderDaemon : public QObject
{
    Q_OBJECT
public:
    explicit ftpSenderDaemon(Abscallback* cb, int chThreadID,QNetworkSession* session);
    QThreadEx* getThread(){return _thread;};
    bool sendFile(QString filePath) {};
    int getTID() {return _tid;};
    bool startDaemon(QString& err);

private:
    QThreadEx* _thread;
    Abscallback* _syncManager;
    int _tid;

    QFtp *_ftp;
    QFile *_file;
    QNetworkSession* _ntwkSesstion;

signals:

public slots:
};

#endif // FTPSENDERDAEMON_H
