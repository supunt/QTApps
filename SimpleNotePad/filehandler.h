#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <QString>
#include <QMessageBox>
#include <QWidget>
#include <QFileDialog>
#include <QTimer>


class fileHandler
{
public:
    fileHandler();
    bool save(QString data, QString& sErr);
    bool saveAs(QString data, QString& sErr);
    QString Init(QWidget* parentWindow);
    QString GetCurrentFilepath();
    QString appendOpenFileandGetData();

    void    onAutoSaveTimer(QString data);

private:
    QString filePath = "";   
    QString lastknownDirPath = "/";
    QTimer* autoSaveWriteTimer;
    QWidget* _parentWindow;

    bool getFilePath2Save();
    void InitAutoSave();
};

#endif // FILEHANDLER_H
