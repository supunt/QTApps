#ifndef ADDNEWVEHICLE_H
#define ADDNEWVEHICLE_H

#include <QDialog>

namespace Ui {
class AddNewVehicle;
}

class AddNewVehicle : public QDialog
{
    Q_OBJECT

public:
    explicit AddNewVehicle(QWidget *parent = 0, bool edit = false);
    ~AddNewVehicle();

private:
    Ui::AddNewVehicle *ui;

    void SetupUi(bool edit);
};

#endif // ADDNEWVEHICLE_H
