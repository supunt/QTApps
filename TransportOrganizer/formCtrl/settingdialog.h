#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = 0);
    ~SettingDialog();

private slots:
    void on_actionAdd_New_Mail_Client_triggered();

    void on_btn_addMC_pressed();

private:
    Ui::SettingDialog *ui;
};

#endif // SETTINGDIALOG_H
