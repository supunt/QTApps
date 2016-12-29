#include "addnewvehicle.h"
#include "ui_addnewvehicle.h"

AddNewVehicle::AddNewVehicle(QWidget *parent, bool edit) :
    QDialog(parent),
    ui(new Ui::AddNewVehicle)
{
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);
}

void AddNewVehicle::SetupUi(bool edit)
{
    if (edit)
    {
        setWindowTitle("Edit vehicle information");
        //LoadDriverDetails();
    }
    else
    {
        setWindowTitle("Add New Vehicle");
    }


}
AddNewVehicle::~AddNewVehicle()
{
    delete ui;
}
