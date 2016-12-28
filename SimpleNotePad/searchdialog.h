#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>

namespace Ui {
class SearchDialog;
}

class SearchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchDialog(QWidget *parent = 0);
    ~SearchDialog();

private slots:
    // open callbacks
    void findClicked();
    void clearClicked();
    void cancelClicked();
    void textChanged(const QString& s);

signals:
    void find(const QString &str, Qt::CaseSensitivity cs,bool searchDir);

private:
    Ui::SearchDialog *ui;
};

#endif // SEARCHDIALOG_H
