#ifndef DBCON_H
#define DBCON_H

#include <QtSql/QSqlDatabase>

class DBCon
{
public:
    DBCon();
    virtual bool connectToDB();
    QSqlDatabase* getSQLDBCon() {return &m_db;};

private:
    QSqlDatabase m_db;
    QString dbName;
    QString dbPath;
};

#endif // DBCON_H
