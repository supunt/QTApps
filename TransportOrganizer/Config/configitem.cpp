#include "configitem.h"

configItem::configItem(CFG_ITEM_DATA_TYPE type, QString key)
{
    _key = key;
    _configItemDataType = type;
}