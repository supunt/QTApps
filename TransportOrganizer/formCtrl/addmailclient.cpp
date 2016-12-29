#include "addmailclient.h"
#include "ui_addmailclient.h"

AddMailClient::AddMailClient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddMailClient)
{
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);
}

AddMailClient::~AddMailClient()
{
    delete ui;
}

void AddMailClient::on_AddClient_pressed()
{

}

void AddMailClient::on_Discard_pressed()
{
    this->close();
}
