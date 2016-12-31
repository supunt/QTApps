#include "dbcon.h"
#include <QMessageBox>

// -----------------------------------------------------------------------------------------------------------------------------------------
DBCon::DBCon()
{
    dbName = "QSQLITE"; // get from cfg
    dbPath = "transportOrgMainDBx"; // get from cfg
}
// -----------------------------------------------------------------------------------------------------------------------------------------
bool DBCon::connectToDB()
{
   m_db = QSqlDatabase::addDatabase(dbName);
   m_db.setDatabaseName(dbPath);

   if (!m_db.open())
      return false;

   return true;
}
// -----------------------------------------------------------------------------------------------------------------------------------------
