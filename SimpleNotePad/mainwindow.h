#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QTimer>
#include <QtCore>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QTextStream>

#include "fileHandler.h"
#include "searchdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionNew_triggered();
    void on_actionCopy_triggered();
    void on_actionCut_triggered();
    void on_actionPaste_triggered();
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSave_As_triggered();
    void on_actionFont_triggered();    
    void on_actionPrint_triggered();
    void on_actionFind_triggered();
    void on_actionAbout_myNotepad_triggered();

    // Custom slots
    void onAutoSaveTimer();
    void onStatusBarTimer();
    void findAndMarkMatch(const QString &str, Qt::CaseSensitivity cs, bool bwd);

private:
    Ui::MainWindow *ui;   
    QTimer* statusBarTimer;

    QLabel *count;
    QLabel *timeStamp;
    QLabel *information;

    QString lastSavedContent = "";

    SearchDialog* searchDlg;
    fileHandler ofileHandler;

    void updateLastSaveTime();
    int  countWords(QString s);
    bool isFileChangedFromLastSave(QString newData);
};

#endif // MAINWINDOW_H
