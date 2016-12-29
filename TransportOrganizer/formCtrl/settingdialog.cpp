#include "settingdialog.h"
#include "ui_settingdialog.h"
#include "addmailclient.h"

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

void SettingDialog::on_actionAdd_New_Mail_Client_triggered()
{

}

void SettingDialog::on_btn_addMC_pressed()
{
    AddMailClient* mc = new AddMailClient;
    mc->show();
}
