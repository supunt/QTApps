#ifndef ADDMAILCLIENT_H
#define ADDMAILCLIENT_H

#include <QDialog>

namespace Ui {
class AddMailClient;
}

class AddMailClient : public QDialog
{
    Q_OBJECT

public:
    explicit AddMailClient(QWidget *parent = 0);
    ~AddMailClient();

private slots:
    void on_AddClient_pressed();
    void on_Discard_pressed();

private:
    Ui::AddMailClient *ui;
};

#endif // ADDMAILCLIENT_H
