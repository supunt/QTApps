#include "dataload.h"

DataLoad::DataLoad()
{
    mdbCon = new DBCon;
}
bool DataLoad::Loadup()
{
    if (!mdbCon->connectToDB())
        return false;

    //LoadEntities();

}
bool DataLoad::LoadEntities()
{
    DBTable* entityTable = new DBTable(mdbCon->getSQLDBCon(),"entities");
    entityTable->Init(true,false,false,false,false,true);
}
