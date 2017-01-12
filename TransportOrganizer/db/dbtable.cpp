#include "dbtable.h"

// -----------------------------------------------------------------------------------------------------------------------------------------
DBTable::DBTable(QSqlDatabase* dbCon, QString tableName)
{
    _tableName = tableName;
    _dbCon = dbCon;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
bool DBTable::Init(bool defaultCur, bool filteredCur, bool insertCur, bool updateCur, bool deleteCur, bool countCur)
{
   if(!LoadSchema())
       return false;

   if(!initCursors(defaultCur, filteredCur, insertCur, updateCur, deleteCur, countCur))
       return false;

   return true;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
bool DBTable::LoadSchema()
{
    return true;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
bool DBTable::initCursors(bool defaultCur, bool filteredCur, bool insertCur, bool updateCur, bool deleteCur,
                          bool countCur)
{
    if (defaultCur)
        _defaultSelectCursor    = new QSqlQuery(*_dbCon);
    if (filteredCur)
        _FilteredSelectCursor   = new QSqlQuery(*_dbCon);
    if (insertCur)
        _InsertCursor              = new QSqlQuery(*_dbCon);
    if (updateCur)
        _UpdateCursor            = new QSqlQuery(*_dbCon);
    if (deleteCur)
        _DeleteCursor              = new QSqlQuery(*_dbCon);
    if (countCur)
        _CountCursor              = new QSqlQuery(*_dbCon);
    return true;
}

