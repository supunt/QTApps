#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include "QTextStream"
#include "utils.h"
//-----------------------------------------------------------------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    _file_path = "";
    ui->setupUi(this);
    connect(ui->btnSave, SIGNAL(clicked()), this, SLOT(on_action_save2File()));
    connect(ui->btnCompile, SIGNAL(clicked()), this, SLOT(on_action_compile()));
    connect(ui->btnOpenFile, SIGNAL(clicked()), this, SLOT(on_action_openFile()));
}
//-----------------------------------------------------------------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    delete ui;
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void MainWindow::on_action_save2File()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Save file", "/home/",
        "Text files (*.txt *.log);; All files (*.*)");

    if (!filePath.isEmpty())
    {
        QFile curentFile(filePath);
        if (!curentFile.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
            return;

        QTextStream out(&curentFile);
        out << ui->outputText->toPlainText();
        curentFile.close();
    }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void MainWindow::on_action_openFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Save file", "/home/",
        "Text files (*.txt *.log);; All files (*.*)");

    if (!filePath.isEmpty())
    {
        QFile curentFile(filePath);
        if (!curentFile.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        _file_path = filePath;
        QTextStream in(&curentFile);
        QString text = in.readAll();
        curentFile.close();
        ui->inputText->setText(text);
    }
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void MainWindow::on_action_compile()
{
    QString Data = ui->inputText->toPlainText();

    if (Data.size() == 0)
    {
        return;
    }

    string line;
    int linenumber = 0;
    fstream file(_file_path.toStdString().c_str());

    if (file.is_open())
    { //remove all multi line comments
        string content((istreambuf_iterator<char>(file)), (istreambuf_iterator<char>()));
        regex mult_COMMENT("/\\*([^*]|[\r\n]|(\\*+([^\\*\\/]|[\r\n])))*\\*+\\/");
        regex_iterator<string::iterator> itd(content.begin(), content.end(), mult_COMMENT);
        regex_iterator<string::iterator> end;

        QString data = "";
        for (; itd != end; ++itd)
        {
            data += QString(itd->str().c_str());
            data += ", multiline_COMMENT\n";
            data = "";
        }
        content = regex_replace(content, mult_COMMENT, " ");


        istringstream f(content);

        while (getline(f, line)) {
            linenumber++;
            COMMENT(line, linenumber,ui->outputText);
            KEYWORD(line, linenumber,ui->outputText);
            FLOAT_NUM(line, linenumber,ui->outputText);
            INT_NUM(line, linenumber,ui->outputText);
            SPECIAL_SYMBOL(line, linenumber,ui->outputText);
            ID(line, linenumber,ui->outputText);
            letter(line, linenumber,ui->outputText);
            Digit(line, linenumber,ui->outputText);

            int size = line.length();
            QString data = "";
            for (int i = 0; i < size; i++) {
                if (line[i] != '\n' && line[i] != ' ')
                {
                    data += "There is a LEXICAL error in this tokent ";
                    data +=  QString(line.c_str());
                    data += "  line number:  ";
                    data += QString::number(linenumber);
                    ui->outputText->append(data);
                    data = "";
                }
            }
        }
        file.close();
    }

    else
        cout << "Unable to open file" << endl;

}
//-----------------------------------------------------------------------------------------------------------------------------------------
