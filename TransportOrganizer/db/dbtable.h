#ifndef DBTABLE_H
#define DBTABLE_H

#include "db/dbcon.h"
#include "db/dbcolumn.h"
#include <QtSql/QSqlQuery>
#include <map>

class DBTable
{
public:
    DBTable(QSqlDatabase* dbCon, QString tableName);
    bool Init(bool defaultCur = true,
              bool filteredCur = true,
              bool insertCur = true,
              bool updateCur = false,
              bool deleteCur = true,
              bool countCur = true);

    bool Fetch();
    bool Insert();
    bool Update();
    bool Delete();

private:
    QSqlDatabase* _dbCon;
    QSqlQuery* _defaultSelectCursor;
    QSqlQuery* _FilteredSelectCursor;
    QSqlQuery* _InsertCursor;
    QSqlQuery* _UpdateCursor;
    QSqlQuery* _DeleteCursor;
    QSqlQuery* _CountCursor;

    int columnCount;
    QString _tableName;
    std::map<QString,DBColumn*> map_selectColumns;
    std::map<QString,DBColumn*> map_insertUpdateColumns;
    std::map<QString,int> map_pks;

    bool LoadSchema();
    void AddColumn(QString colName, QString dataType);

    bool initCursors(bool defaultCur,
                     bool filteredCur,
                     bool insertCur,
                     bool updateCur,
                     bool deleteCur,
                     bool countCur);
};

#endif // DBTABLE_H
