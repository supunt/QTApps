#ifndef LOGGER_H
#define LOGGER_H

#include <mutex>
#include <QString>

class QMainWindow;

class logger
{
public:
    static void log(QString log);
    static  void logDebug(QString log);
    static  void logError(QString log);

private:
    logger();
    logger(const logger&) = delete;
    logger& operator=(logger&) = delete;

    //mutex for _logWriter_, file_desc, etc
    std::mutex _logger_mtx_;
    std::unique_lock<std::mutex> _logFileLock;
    static logger* _logWriter_;
};

#endif // LOGGER_H
