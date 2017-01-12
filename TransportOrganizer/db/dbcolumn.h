#ifndef DBCOLUMN_H
#define DBCOLUMN_H

#include <QString>

class DBColumn
{
public:
    DBColumn(QString cName);
    bool getWriteFlag(){return _writeFlag;};
    bool isNull(){return _isNull;};

    void setWriteFlag(bool wFlag){ _writeFlag = wFlag;};
    void setNullFlag(bool nFlag){ _isNull = nFlag;};

private:
    QString _colName;
    bool _isNull;
    bool _writeFlag;
};

#endif // DBCOLUMN_H
