#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

#include "formCtrl/settingdialog.h"
#include "formCtrl/addnewvehicle.h"
#include "formCtrl/registernewdriver.h"

#include <QStandardItemModel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /*QStandardItemModel *model = new QStandardItemModel(2,3,this); //2 Rows and 3 Columns
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("Column1 Header")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Column2 Header")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("Column3 Header")));

    ui->tableView->setModel(model);*/
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAdd_New_Vehicle_triggered()
{
    AddNewVehicle* av = new AddNewVehicle;
    av->setWindowTitle("Add new");
    av->show();
}

void MainWindow::on_actionSettings_triggered()
{
    SettingDialog* sd =  new SettingDialog;

    sd->setWindowTitle("Settings");
    sd->show();
}

void MainWindow::on_actionAdd_New_Driver_triggered()
{
    RegisterNewDriver* rd = new RegisterNewDriver;
    rd->setWindowTitle("Add new Driver");
    rd->show();
}
