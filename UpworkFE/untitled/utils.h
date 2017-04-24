#include<iostream>
#include <regex>
#include <string>
#include<fstream>
#include <sstream>
#include "QTextEdit"
#include "QString"

using namespace std;

void KEYWORD(string & str, int linenum, QTextEdit* textEdit) {
    regex keywords("(void|int|bool|float|if|else|while|for|return|break)");
    regex_iterator<string::iterator> itd(str.begin(), str.end(), keywords);
    regex_iterator<string::iterator> end;

    QString data = "";
    for (; itd != end; ++itd)
    {
        data  += QString::number(linenum);
        data  +=",";
        data  += QString(itd->str().c_str());
        data  +=",";
        data  +=" KEYWORD";
        textEdit->append(data);
        data = "";
    }
    str = regex_replace(str, keywords, " ");

}

void SPECIAL_SYMBOL(string & str, int linenum, QTextEdit* textEdit) {
    regex special_symbol("(\\&&|\\<=|\\>=|\\==|\\<|\\>|\\!=|\\=|\\(|\\)|\\{|\\}|\\;|\\,|\\.|\\+|\\-|\\*)");
    regex_iterator<string::iterator> itd(str.begin(), str.end(), special_symbol);
    regex_iterator<string::iterator> end;
    QString data = "";
    for (; itd != end; ++itd)
    {
        data  += QString::number(linenum);
        data  +=",";
        data  += QString(itd->str().c_str());
        data  +=",";
        data  +=" SPECIAL_SYMBOL";
        textEdit->append(data);
        data = "";
    }
    str = regex_replace(str, special_symbol, " ");
}

void INT_NUM(string & str, int linenum, QTextEdit* textEdit) {

    regex ints("[-|+]+[0-9]{1,10}");
    regex_iterator<string::iterator> itd(str.begin(), str.end(), ints);
    regex_iterator<string::iterator> end;

    QString data = "";
    for (; itd != end; ++itd)
    {
        data  += QString::number(linenum);
        data  +=",";
        data  += QString(itd->str().c_str());
        data  +=",";
        data  +=" INT_NUM";
        textEdit->append(data);
        data = "";
    }
    str = regex_replace(str, ints, " ");
}


void Digit(string  & str, int linenum, QTextEdit* textEdit) {

    regex Digit("0|1|2|3|4|5|6|7|8|9");
    regex_iterator<string::iterator> itd(str.begin(), str.end(), Digit);
    regex_iterator<string::iterator> end;

    QString data = "";
    for (; itd != end; ++itd)
    {
        data  += QString::number(linenum);
        data  +=",";
        data  += QString(itd->str().c_str());
        data  +=",";
        data  +=" Digit";
        textEdit->append(data);
        data = "";
    }

    str = regex_replace(str, Digit, " ");

}
void letter(string  & str, int linenum, QTextEdit* textEdit) {

    regex letter("[a-zA-Z]");
    regex_iterator<string::iterator> itd(str.begin(), str.end(), letter);
    regex_iterator<string::iterator> end;

    QString data = "";
    for (; itd != end; ++itd)
    {
        data  += QString::number(linenum);
        data  +=",";
        data  += QString(itd->str().c_str());
        data  +=",";
        data  +=" letter";
        textEdit->append(data);
        data = "";
    }
    str = regex_replace(str, letter, " ");

}

void FLOAT_NUM(string  & str, int linenum, QTextEdit* textEdit) {

    regex floats("[0-9]*(\\.[0-9]+)");
    regex_iterator<string::iterator> itd(str.begin(), str.end(), floats);
    regex_iterator<string::iterator> end;

    QString data = "";
    for (; itd != end; ++itd)
    {
        data  += QString::number(linenum);
        data  +=",";
        data  += QString(itd->str().c_str());
        data  +=",";
        data  +=" FLOAT_NUM";
        textEdit->append(data);
        data = "";
    }

    str = regex_replace(str, floats, " ");

}

void ID(string & str, int linenum, QTextEdit* textEdit) {

    regex identifier("_[a-zA-Z][_a-zA-Z0-9]{0,30}"); //identifier could start with underscore or have under score at the middle
    regex_iterator<string::iterator> itd(str.begin(), str.end(), identifier);
    regex_iterator<string::iterator> end;

    QString data = "";
    for (; itd != end; ++itd)
    {
        data  += QString::number(linenum);
        data  +=",";
        data  += QString(itd->str().c_str());
        data  +=",";
        data  +=" ID";
        textEdit->append(data);
        data = "";
    }

    str = regex_replace(str, identifier, " ");
}

void COMMENT(string & str, int linenum, QTextEdit* textEdit) {

    regex comments("//.*");
    regex_iterator<string::iterator> itd(str.begin(), str.end(), comments);
    regex_iterator<string::iterator> end;

   QString data = "";
    for (; itd != end; ++itd)
    {
        data  += QString::number(linenum);
        data  +=",";
        data  += QString(itd->str().c_str());
        data  +=",";
        data  +=" COMMENT";
        textEdit->append(data);
        data = "";
    }
    str = regex_replace(str, comments, " ");

}
