#ifndef CONFIG_H
#define CONFIG_H

#include "configitem.h"
#include <map>

enum CFG_TYPE
{
    CFG_TYPE_MAIL_CLIENT_SETTINGS=0,
    CFG_TYPE_PREFERENCES,
};

class config
{
public:
    config(CFG_TYPE cftype, QString key);
    void AddConfigItem(CFG_ITEM_DATA_TYPE cfidt, QString key, QString sData){};

private:
    CFG_TYPE _cfgType;
    std::map<QString,configItem*> _mapCfgItems;
    QString _key;
};

#endif // CONFIG_H
