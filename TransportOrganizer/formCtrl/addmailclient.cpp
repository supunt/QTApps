#include "addmailclient.h"
#include "ui_addmailclient.h"

// -----------------------------------------------------------------------------------------------------------------------------------------
AddMailClient::AddMailClient(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddMailClient)
{
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->setupUi(this);
    //gmail port 993 and SSL
}

// -----------------------------------------------------------------------------------------------------------------------------------------
AddMailClient::~AddMailClient()
{
    delete ui;
}

// -----------------------------------------------------------------------------------------------------------------------------------------
void AddMailClient::on_AddClient_pressed()
{

}
