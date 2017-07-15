#ifndef FTPSENDERDAEMON_H
#define FTPSENDERDAEMON_H

#include <QObject>
#include <QFile>

#include "qt4_legacy/qftp.h"
#include "qthreadex.h"
#include "abscallback.h"
#include <QtNetwork/QNetworkSession>
#include <QProgressBar>

class syncManager;

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
    syncManager* _syncManager;
    int _tid;

    QFtp *_ftp;
    QFile *_file;
    QNetworkSession* _ntwkSesstion;
    QString _host;
    QString _user;
    QString _pass;

private slots:
    void thread_start();
    void ftpCommandFinished(int comId,bool error);
};

#endif // FTPSENDERDAEMON_H
