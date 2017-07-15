#ifndef QTABLEWIDGETEX_H
#define QTABLEWIDGETEX_H
#pragma once

#include "QTableWidget.h"
#include "QDebug"
#include "QFileInfo"
#include "QDateTime"
#include "QProgressBar"
#include "defs.h"
//-----------------------------------------------------------------------------------------------------------------------------------------
struct fe_error
{
    QString _err;
    QString _source;
    QDateTime _dt;
    TWE  _type;
    int _count;
    int _index;
};
//-----------------------------------------------------------------------------------------------------------------------------------------
class QTableWidgetEx : public QTableWidget
{
public:
    QTableWidgetEx(QWidget *parent);
    void Insert_Row(QFileInfo* qfi, int& rownum);
    void Insert_Row(fe_error* err, int& rownum);
    void updateCellValue(int row,int column, QString val);
private:
    template <typename T>
    void setCellData(int row,
                            int column,
                             T* data,
                            QColor* cellColor = nullptr);

    //---------- general hash codes we are going to use
    size_t const _dt_int_hash = typeid(int).hash_code();
    size_t const _dt_double_hash= typeid(double).hash_code();
    size_t const _dt_qint64_hash= typeid(qint64).hash_code();
    size_t const _dt_qdatetime_hash = typeid(QDateTime).hash_code();
    size_t const _dt_qstring_hash = typeid(QString).hash_code();
    size_t const _dt_qprogressbar_hash = typeid(QProgressBar).hash_code();

     QColor _mian_tbl_color[2];

     QString getTypeStr(TWE type)
     {
         switch(type)
         {
             case ERROR: return "ERROR"; break;
             case WARNING: return "WARNING"; break;
             case SUCCESS: return "INFO"; break;
             default : return "TEXT"; break;
         }
     }
};

#endif // QTABLEWIDGETEX_H
