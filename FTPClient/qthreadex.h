#ifndef QTHREADEX_H
#define QTHREADEX_H

#include <QThread>

class QThreadEx : public QThread
{
public:
    void run();
    QThreadEx(int tid);
private:
    int _tid;
};

#endif // QTHREADEX_H
