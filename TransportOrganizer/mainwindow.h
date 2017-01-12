#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "db/dbcon.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    virtual bool initDBCons();

private slots:
    void on_actionAdd_New_Vehicle_triggered();

    void on_actionSettings_triggered();

    void on_actionAdd_New_Driver_triggered();

    void on_actionRegistered_Drivers_triggered();

private:
    Ui::MainWindow *ui;
    DBCon dbCon;
};

#endif // MAINWINDOW_H
