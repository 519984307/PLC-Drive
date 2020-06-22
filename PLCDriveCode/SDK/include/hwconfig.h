#ifndef HWCONFIG_H
#define HWCONFIG_H
#include <QString>
#include <QMap>
#include <QVector>

//硬件配置基本属性的结构体
typedef struct _HWCONFIGPROPERTY_STRU
{
    int addr = 0;
    int size = 0;
    QString hwtype = "";
    int curPos = 0;
}HWCONFIGPROPERTY_STRU;

typedef struct _BIT_VALUE_STRU
{
    uchar bit = 0;
    uchar value  = 0;

}BIT_VALUE_STRU;
typedef struct _SON_STRU
{
    QVector<BIT_VALUE_STRU> OutPutVec;
    QVector<BIT_VALUE_STRU> InPutVec;
    _SON_STRU()
    {
        OutPutVec.clear();
        InPutVec.clear();
    }
}SON_STRU;
//通迅配置内容
typedef struct _SERIALCONFIG_STRU{
   QString  ID ="";
  QString Portname = "";
  QString Baud_Rate = "";
  QString headendstr = "";
  QString Dataformat = "";
  QString saveVarname =  "";
}SERIALCONFIG_STRU;//串口配置内容，内容数据进行校验
//网络配置
typedef struct _SOCKETCONFIG_STRU{
   QString  ID = "";
  QString ProType = "";
  QString Sertype = "";
  QString IP = "";
  QString PortID = "";
   QString saveVarname =  "";
}SOCKETCONFIG_STRU;//网络配置内容
//总的硬件配置
typedef struct _HWCONFIG_STRU
{
    QString netName = "";
    int sumInNum = 0;
    int sumOutNum = 0;
    QMap<int,QMap<int,HWCONFIGPROPERTY_STRU>> hwconfigInputMap;
    QMap<int,QMap<int,HWCONFIGPROPERTY_STRU>> hwconfigOutputMap;
    QMap<QString,SERIALCONFIG_STRU>hwconfigserialMap;
    QMap<QString,SOCKETCONFIG_STRU>hwconfigsocketMap;
    SON_STRU hwconfigSonstru;
    _HWCONFIG_STRU(){
       hwconfigInputMap.clear();
       hwconfigOutputMap.clear();
       hwconfigserialMap.clear();
       hwconfigsocketMap.clear();
    }
}HWCONFIG_STRU;




#endif // HWCONFIG_H

