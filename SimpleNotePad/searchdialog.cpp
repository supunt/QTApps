#include "searchdialog.h"
#include "ui_searchdialog.h"

#include <QTextStream>
#include <QPushButton>

SearchDialog::SearchDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SearchDialog)
{
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    this->setWindowTitle("Find");
    ui->setupUi(this);

    ui->btnFind->setEnabled(false);
    ui->btnClear->setEnabled(false);

    connect(ui->btnFind,SIGNAL(clicked()), this, SLOT(findClicked()));
    connect(ui->btnCancel,SIGNAL(clicked()), this, SLOT(cancelClicked()));
    connect(ui->btnClear,SIGNAL(clicked()), this, SLOT(clearClicked()));
    connect(ui->lineEdit,SIGNAL(textChanged(QString)), this, SLOT(textChanged(const QString&)));

}
//--------------------------------------------------------------------------------
SearchDialog::~SearchDialog()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void SearchDialog::findClicked()
{
    QString text = ui->lineEdit->text();
    Qt::CaseSensitivity cs = ui->rbCaseSensitive->isChecked()?Qt::CaseSensitive:Qt::CaseInsensitive;
    if (ui->rbSearchup->isChecked())
        emit find(text, cs,true);
    else
        emit find(text, cs,false);
}

//--------------------------------------------------------------------------------
void SearchDialog::cancelClicked()
{
    ui->lineEdit->clear();
    this->close();
}
//--------------------------------------------------------------------------------
void SearchDialog::clearClicked()
{
    ui->lineEdit->clear();
}
//--------------------------------------------------------------------------------
void SearchDialog::textChanged(const QString& s)
{
    if (s.trimmed() != "")
    {
        ui->btnFind->setEnabled(true);
        ui->btnClear->setEnabled(true);
    }
    else
    {
        ui->btnFind->setEnabled(false);
        ui->btnClear->setEnabled(false);
    }
}

//--------------------------------------------------------------------------------
