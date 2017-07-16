#ifndef FTPSENDERDAEMON_H
#define FTPSENDERDAEMON_H

#include <QObject>
#include <QFile>

#include "qt4_legacy/qftp.h"
#include "qthreadex.h"
#include "abscallback.h"
#include <QtNetwork/QNetworkSession>
#include <QProgressBar>
#include <QTimer>


class syncManager;
class QFileInfo;
typedef std::pair<QFileInfo*,int> PAIR_FI_I;

class ftpSenderDaemon : public QObject
{
    Q_OBJECT
public:
    explicit ftpSenderDaemon(Abscallback* cb, int chThreadID,QNetworkSession* session);
    QThreadEx* getThread(){return _thread;};
    void sendFile(PAIR_FI_I* fileInfo);
    int getTID() {return _tid;};
    bool startDaemon(QString& err);
    QFtp* getClient(){return _ftp;};

private:
    QThreadEx* _thread;
    syncManager* _syncManager;
    int _tid;

    QFtp *_ftp = nullptr;
    QFile *_file = nullptr;
    QNetworkSession* _ntwkSesstion = nullptr;
    QString _host;
    QString _user;
    QString _pass;
    PAIR_FI_I _currentFileInfo;
    QTimer* _reconnectTimer = nullptr;

private slots:
    void init();
    void ftpCommandFinished(int comId,bool error);
    void onFtpDataTransferProgress(qint64 now,qint64 total);
    void onReconnectTimer();
    void onFtpStateChanged(int state);
};

#endif // FTPSENDERDAEMON_H
