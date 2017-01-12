#ifndef DATALOAD_H
#define DATALOAD_H

#include "db/dbcon.h"
#include "entities/entity.h"

class DataLoad
{
public:
    DataLoad();
    DBCon* mdbCon;
    bool Loadup();
    bool LoadEntities();

    std::map<int,entity*> mapEntities;
};

#endif // DATALOAD_H
