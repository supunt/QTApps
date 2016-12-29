#ifndef REGISTERNEWDRIVER_H
#define REGISTERNEWDRIVER_H

#include <QDialog>

namespace Ui {
class RegisterNewDriver;
}

class RegisterNewDriver : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterNewDriver(QWidget *parent = 0, bool edit = false);
    ~RegisterNewDriver();

private slots:
    void on_date_IssueDate_dateChanged(const QDate &date);

    void on_date_dob_dateChanged(const QDate &date);

private:
    Ui::RegisterNewDriver *ui;

    void SetupUi(bool edit);
};

#endif // REGISTERNEWDRIVER_H
