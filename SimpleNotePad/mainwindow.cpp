#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QPainter>
#include <QDialog>
#include <QtPrintSupport/QPrintPreviewDialog>
#include <QDateTime>
#include <QFontDialog>
#include <QFrame>

#define DEFAULT_FILE_PATH "myNotepad/temp.txt"

//-------------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->textEdit);
    setWindowTitle(QCoreApplication::applicationName());
    ui->textEdit->setPlainText(ofileHandler.Init(this));
    setWindowTitle("New text file");

    searchDlg = new SearchDialog(this);

    // create status bar timer
    statusBarTimer = new QTimer(this);
    connect(statusBarTimer,SIGNAL(timeout()),this,SLOT(onStatusBarTimer()));
    statusBarTimer->start(5000);

    // Finder callbacks
    connect(searchDlg,SIGNAL(find(const QString, Qt::CaseSensitivity,bool)),this,SLOT(findAndMarkMatch(const QString&, Qt::CaseSensitivity, bool)));

    // Two widgets for stats
    count = new QLabel;
    count->setFrameShadow(QFrame::Sunken);
    ui->statusBar->addWidget(count);
    count->setText("Word count : 0 \t");

    timeStamp = new QLabel;
    timeStamp->setFrameShadow(QFrame::Sunken);
    ui->statusBar->addWidget(timeStamp);
    timeStamp->setText("Last saved : \t");
}
//-------------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    delete searchDlg;
    delete statusBarTimer;
    delete timeStamp;
    delete count;
    delete ui;
}
//-------------------------------------------------------------------------------------
void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
    ui->statusBar->showMessage("Copy the contents into the clipboard ", 1400);
}
//-------------------------------------------------------------------------------------
void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
}
//-------------------------------------------------------------------------------------
void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}
//-------------------------------------------------------------------------------------
void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}
//-------------------------------------------------------------------------------------
void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}
//-------------------------------------------------------------------------------------
void MainWindow::on_actionFont_triggered()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, this);
    if (ok) {
        ui->textEdit->setFont(font);
    } else return;
}
//-------------------------------------------------------------------------------------
void MainWindow::on_actionAbout_myNotepad_triggered()
{
    QMessageBox::about(this, tr("About myNotepad"),
             tr("v.1.0.1"));

}
//-------------------------------------------------------------------------------------
void MainWindow::on_actionPrint_triggered()
{
    QPrinter printer;
    QPrintDialog *printDialog = new QPrintDialog(&printer, this);
    printDialog->setWindowTitle("Print Document");
    if (printDialog->exec() == QDialog::Accepted)
        ui->textEdit->print(&printer);
}

//-------------------------------------------------------------------------------------
void MainWindow::onStatusBarTimer()
{
    // reason for timer is to reduce CPU load..
    QString s = "Word count : ";
    QString sData =  ui->textEdit->toPlainText();

    int wordCount = countWords(sData);
    s += (QString::number(wordCount) + " \t");
    count->setText(s);
}

//-------------------------------------------------------------------------------------
void MainWindow::onAutoSaveTimer()
{
    ofileHandler.onAutoSaveTimer(ui->textEdit->toPlainText());
    lastSavedContent = ui->textEdit->toPlainText();
    updateLastSaveTime();
}
//-------------------------------------------------------------------------------------
void MainWindow::updateLastSaveTime()
{
    QString strTimeStamp = QDateTime::currentDateTime().toString();
    QString ls = "Last saved : " + strTimeStamp + " \t";
    timeStamp->setText(ls);
}
//-------------------------------------------------------------------------------------
int MainWindow::countWords(QString s)
{
    s = s.simplified();
    s = s.trimmed();

    int wc = 0;
    if (s.size() == 0)
        wc = 0;
    else
        wc = 1;

    for (QChar c : s)
    {
        if (c == ' ')
            wc++;
    }
    return wc;
}
//-------------------------------------------------------------------------------------
void MainWindow::on_actionFind_triggered()
{
    ui->textEdit->moveCursor(QTextCursor::Start);
    searchDlg->show();
}
//-------------------------------------------------------------------------------------
void MainWindow::findAndMarkMatch(const QString &str, Qt::CaseSensitivity cs, bool bwd)
{
    if (bwd)
        if (cs == Qt::CaseSensitive)
            ui->textEdit->find(str,(QTextDocument::FindBackward | QTextDocument::FindCaseSensitively));
        else
            ui->textEdit->find(str,QTextDocument::FindBackward);
    else
        if (cs == Qt::CaseSensitive)
            ui->textEdit->find(str, QTextDocument::FindCaseSensitively);
        else
            ui->textEdit->find(str);

}
//-------------------------------------------------------------------------------------
// File operations --------------------------------------------------------------------
//-------------------------------------------------------------------------------------
void MainWindow::on_actionNew_triggered()
{
    if(isFileChangedFromLastSave(ui->textEdit->toPlainText()))
    {
        QMessageBox::StandardButton opt = QMessageBox::question(this, "Save", "Do you want to save your changes?",
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        QString sErr = "";

        switch (opt)
        {
        case QMessageBox::Save:
            if(ofileHandler.save(ui->textEdit->toPlainText(),sErr))
            {
                ui->textEdit->clear();
                this->setWindowTitle("New text file");
            }
            break;
        case QMessageBox::Discard:
            ui->textEdit->clear();
            break;
        default:
            break;
        }
    }
    else
      ui->textEdit->clear();
}
//-------------------------------------------------------------------------------------
void MainWindow::on_actionOpen_triggered()
{
    if(isFileChangedFromLastSave(ui->textEdit->toPlainText()))
    {
        QMessageBox::StandardButton opt = QMessageBox::question(this, "Save", "Unsave changes exist do you want to save ?",
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        QString sErr = "";
        switch (opt)
        {
        case QMessageBox::Save:
        {
            if(!ofileHandler.save(ui->textEdit->toPlainText(),sErr))
                return;

            ui->textEdit->clear();
            ui->textEdit->setPlainText(ofileHandler.appendOpenFileandGetData());
            this->setWindowTitle(ofileHandler.GetCurrentFilepath());
            break;
        }
        case QMessageBox::Discard:
            ui->textEdit->clear();
            ui->textEdit->setPlainText(ofileHandler.appendOpenFileandGetData());
            this->setWindowTitle(ofileHandler.GetCurrentFilepath());
            break;
        default:
            break;
        }
    }
    else
    {
        ui->textEdit->clear();
        ui->textEdit->setPlainText(ofileHandler.appendOpenFileandGetData());
        this->setWindowTitle(ofileHandler.GetCurrentFilepath());
    }
}
//-------------------------------------------------------------------------------------
void MainWindow::on_actionSave_triggered()
{
    QString sErr = "";
    ofileHandler.save(ui->textEdit->toPlainText(),sErr);
    this->setWindowTitle(ofileHandler.GetCurrentFilepath());
    lastSavedContent = ui->textEdit->toPlainText();
    updateLastSaveTime();
}
//-------------------------------------------------------------------------------------
void MainWindow::on_actionSave_As_triggered()
{
    QString sErr = "";
    ofileHandler.saveAs(ui->textEdit->toPlainText(),sErr);
    this->setWindowTitle(ofileHandler.GetCurrentFilepath());
    lastSavedContent = ui->textEdit->toPlainText();
    updateLastSaveTime();
}
//-------------------------------------------------------------------------------------
bool MainWindow::isFileChangedFromLastSave(QString newData)
{
    if ((ofileHandler.GetCurrentFilepath() == DEFAULT_FILE_PATH) &&
        (newData.length() != 0))
        return true;

    if (newData.length() != lastSavedContent.length())
        return true;
    else
    {
        if (newData != lastSavedContent)
            return true;
        else
            return false;
    }
}
//-------------------------------------------------------------------------------------
