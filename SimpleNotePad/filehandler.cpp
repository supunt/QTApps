#include "filehandler.h"


#include <QMessageBox>
#include <QFont>
#include <QTextStream>
#include <QDir>
#include <mainwindow.h>


using namespace std;

#define DEFAULT_DIR_PATH "myNotepad"
#define DEFAULT_FILE_PATH "myNotepad/temp.txt"
#define TIMER_DURATION_MS 30000 // 30 second auto save timer

//---------------------------------------------------
fileHandler::fileHandler()
{
    filePath = DEFAULT_DIR_PATH;
    lastknownDirPath = DEFAULT_DIR_PATH;
}
//---------------------------------------------------
QString fileHandler::Init(QWidget* parentWindow)
{
    _parentWindow = parentWindow;
    QDir dir(filePath);

    if(!dir.exists())
        dir.mkpath(filePath);

    filePath = DEFAULT_FILE_PATH;
    InitAutoSave();

    return "";
}
//---------------------------------------------------
QString fileHandler::appendOpenFileandGetData()
{
    filePath = QFileDialog::getOpenFileName(_parentWindow,"Open a file..",lastknownDirPath,
                                                "Text files (*.txt *.log);; All files (*.*)");


    if(!filePath.isEmpty())
    {
        QFile curentFile(filePath);
        if(!curentFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            filePath = DEFAULT_FILE_PATH;
            return "";
        }
        else
        {
            QTextStream in (&curentFile);
            QString text = in.readAll();
            curentFile.close();
            return text;
        }
    }
    return "";
}
//---------------------------------------------------
bool fileHandler::getFilePath2Save()
{
    filePath = QFileDialog::getSaveFileName(_parentWindow,"Save file",lastknownDirPath,
                                                "Text files (*.txt *.log);; All files (*.*)");

    if (filePath.isEmpty())
    {
        filePath = DEFAULT_FILE_PATH;
        return false;
    }

    return true;
}
//---------------------------------------------------
bool fileHandler::saveAs(QString data, QString& sErr)
{
    if(!getFilePath2Save())
        return false;

    save(data,sErr);
    return true;
}
//---------------------------------------------------
bool fileHandler::save(QString data, QString& sErr)
{
    if (filePath == DEFAULT_FILE_PATH)
    {
        return saveAs(data,sErr);
    }
    else
    {
        if(!filePath.isEmpty())
        {
            QFile curentFile(filePath);
            if(!curentFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
            {
                filePath = DEFAULT_FILE_PATH;
                return false;
            }
            QTextStream out(&curentFile);
            out << data;
            curentFile.close();
            return true;

        }
        else
        {
            filePath = DEFAULT_FILE_PATH;
            return false;
        }
    }
}
//---------------------------------------------------
QString fileHandler:: GetCurrentFilepath()
{
    return filePath;
}
//---------------------------------------------------
void fileHandler::InitAutoSave()
{
    autoSaveWriteTimer = new QTimer(_parentWindow);
    QObject::connect(autoSaveWriteTimer, SIGNAL(timeout()), _parentWindow/*file handler is not QObject*/, SLOT(onAutoSaveTimer()));
    autoSaveWriteTimer->start(TIMER_DURATION_MS);
}
//---------------------------------------------------
void fileHandler::onAutoSaveTimer(QString data)
{
    QFile curentFile(filePath);
    if(!curentFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
    {
        filePath = DEFAULT_FILE_PATH;
        return;
    }
    QTextStream out(&curentFile);
    out << data;
    curentFile.close();
}
