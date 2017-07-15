#include "qtablewidgetex.h"
#include "QDateTime"
#include "mainwindow.h"

//-----------------------------------------------------------------------------------------------------------------------------------------
QTableWidgetEx::QTableWidgetEx(QWidget *parent):QTableWidget(parent)
{
    _mian_tbl_color[0].setRgb(255,255,255);
    _mian_tbl_color[1].setRgb(255,240,230);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void QTableWidgetEx::Insert_Row(QFileInfo* qfi, int& rownum)
{
    QColor qc = _mian_tbl_color[(int)MainWindow::g_mainTblClr];
    rownum  = rowCount();
    insertRow(rownum);
    qint64 temp = 0;
    setCellData(rownum,0,&qfi->fileName(),&qc);
    temp = qfi->size();
    setCellData(rownum,1,&temp,&qc);
    setCellData(rownum,2,&qfi->created(),&qc);
    setCellData(rownum,3,new QProgressBar(),&qc);
    setCellData(rownum,4,&qfi->absoluteFilePath(),&qc);

    resizeRowsToContents();
    resizeColumnsToContents();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void QTableWidgetEx::Insert_Row(fe_error* err, int& rownum)
{
    rownum  = rowCount();
    insertRow(rownum);
    QColor qc;

    switch (err->_type)
    {
        case WARNING: qc.setRgb(255,255,157);break;
        case ERROR: qc.setRgb(255,94,99);break;
        case SUCCESS:qc.setRgb(111,255,111);break;
        default : qc.setRgb(255,255,255);break;
    }
    QString status = getTypeStr(err->_type);

    setCellData(rownum,0,&err->_source,&qc);
    setCellData(rownum,1,&err->_err, &qc);
    setCellData(rownum,2,&err->_dt,&qc);
    setCellData(rownum,3,&err->_count,&qc);
    setCellData(rownum,4,&status,&qc);

    resizeRowsToContents();
    resizeColumnsToContents();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void QTableWidgetEx::updateCellValue(int row,int column, QString val)
{
    QTableWidgetItem* wi = item(row,column);
    if (wi)
    {
         wi->setText(val);
         update();
    }
    resizeRowsToContents();
    resizeColumnsToContents();
}
//-----------------------------------------------------------------------------------------------------------------------------------------
template <typename T>
void QTableWidgetEx::setCellData(int row, int column, T* data, QColor* cellColor)
{
    QTableWidgetItem* twi =  nullptr;
    size_t tid_hash = typeid(*data).hash_code();

     if (tid_hash == _dt_int_hash)
            twi = new QTableWidgetItem(QString::number(*((int*)data)));
     else if (tid_hash == _dt_double_hash)
            twi = new QTableWidgetItem(QString::number(*((double*)data)));
     else if (tid_hash == _dt_qint64_hash)
         twi = new QTableWidgetItem(QString::number(*((qint64*)data)));
    else if (tid_hash == _dt_qdatetime_hash)
            twi = new QTableWidgetItem((*((QDateTime*)data)).toString());
    else if (tid_hash == _dt_qstring_hash )
            twi = new QTableWidgetItem(*((QString*)data));
    else if (tid_hash == _dt_qprogressbar_hash)
    {
         ((QProgressBar*)data)->setRange(0,100);
         setCellWidget(row,column,(QProgressBar*)data);
         return; // do not continue as this is a widget
    }
     else
     {
         return;
    }
     if (cellColor)
        twi->setBackgroundColor(*cellColor);

     if (!twi)
         qDebug() << "NULL";
     setItem(row,column,twi);
}
