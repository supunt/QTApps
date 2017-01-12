#ifndef ENTITY_H
#define ENTITY_H

#include "db/dbtable.h"
#include "entities/entityinstance.h"

class entity
{
public:
    entity();
    DBTable* _dbTable;
    std::map<QString,entityInstance*> _mapEntityInst;
};

#endif // ENTITY_H
