#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void on_actionAdd_New_Vehicle_triggered();

    void on_actionSettings_triggered();

    void on_actionAdd_New_Driver_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
