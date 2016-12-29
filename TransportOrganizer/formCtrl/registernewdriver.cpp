#include "registernewdriver.h"
#include "ui_registernewdriver.h"

RegisterNewDriver::RegisterNewDriver(QWidget *parent, bool edit) :
    QDialog(parent),
    ui(new Ui::RegisterNewDriver)
{
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);

    SetupUi(edit);
}

RegisterNewDriver::~RegisterNewDriver()
{
    delete ui;
}

void RegisterNewDriver::on_date_IssueDate_dateChanged(const QDate &date)
{
    ui->date_expDate->setDate(date);
}

void RegisterNewDriver::on_date_dob_dateChanged(const QDate &date)
{
    ui->le_age->setText("10");
}

void RegisterNewDriver::SetupUi(bool edit)
{
    if (edit)
    {
        setWindowTitle("Edit Driver Details");
        //LoadDriverDetails();
    }
    else
    {
        QDate date = QDate::currentDate();
        ui->date_IssueDate->setDate(date);
        setWindowTitle("Add New Driver");
    }


}
