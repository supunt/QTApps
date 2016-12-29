#ifndef CONFIGITEM_H
#define CONFIGITEM_H

#include <QString>

enum CFG_ITEM_DATA_TYPE
{
    CFG_ITEM_DATA_TYPE_INT=0,
    CFG_ITEM_DATA_TYPE_STRING,
    CFG_ITEM_DATA_TYPE_DOUBLE
};
//------------------------------------------------------------------------
class configItem
{
public:
    configItem(CFG_ITEM_DATA_TYPE type, QString key);

    QString getKey() {return _key;};
    CFG_ITEM_DATA_TYPE getCfgType(){return _configItemDataType;};

private:
    CFG_ITEM_DATA_TYPE _configItemDataType;
    QString _key;
};
//------------------------------------------------------------------------
class stringConfigItem : public configItem
{
public:
    stringConfigItem(CFG_ITEM_DATA_TYPE type, QString key, QString sData):configItem(type,key)
    {
        _data = sData;
    };
    QString getCGFData(){return _data;};

private:
    QString _data;
};
//------------------------------------------------------------------------
class intConfigItem : public configItem
{
public:
    intConfigItem(CFG_ITEM_DATA_TYPE type, QString key, int iData):configItem(type,key)
    {
        _data = iData;
    };
    int getCGFData(){return _data;};

private:
    int _data;
};
//------------------------------------------------------------------------
class doubleConfigItem : public configItem
{
public:
    doubleConfigItem(CFG_ITEM_DATA_TYPE type, QString key, double dData):configItem(type,key)
    {
        _data = dData;
    };
    double getCGFData(){return _data;};

private:
    double _data;
};
//------------------------------------------------------------------------
#endif // CONFIGITEM_H
