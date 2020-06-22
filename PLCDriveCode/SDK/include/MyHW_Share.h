#ifndef MY_HW_SHARED_H
#define MY_HW_SHARED_H

#include <QObject>
#include <QMap>
#include <MyConfig_Shared>
#include <DycLibBase/PackLib/packlib.h>
#include <MyGlobal.h>
using namespace CoreGlobal;
typedef struct _BYTEINFO
{
    QString strType = "";
    uchar byteNum = 0;
    int   bytePos = -1;

}BYTEINFO;

class MyHW_Share
{
private:
    MyHW_Share()
    {
        m_AxisORG.clear();
        m_INSON.clear();
        m_OUTSON.clear();
        m_OFFSON.clear();
        foreach (auto it, ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.son_Stru.InPutVec) {
            QPair<int,int> pair = QPair<int,int>(it.bit,it.value);
            m_INSON.append(pair);
        }

        foreach (auto it, ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.son_Stru.OutPutVec) {
            QPair<int,int> pair = QPair<int,int>(it.bit,it.value);
            m_OUTSON.append(pair);
            int offValue = 0;
            if(it.value == 0)
            {
                offValue = 1;
            }
            QPair<int,int> pairoff = QPair<int,int>(it.bit,offValue);
            m_OFFSON.append(pairoff);
        }
    }

public:
    static MyHW_Share* GetInstance()
    {
        static MyHW_Share pInstance;
        return &pInstance;
    }
    int AnalysisOutPutBytes(const QString &Key,QMap<QString,BITINFO_STRU> &bitInfoMap,QMap<QString,BYTEINFO> &byteInfoMap)
    {
        int cwBytePos = -1;
        bitInfoMap.clear();
        byteInfoMap.clear();
        QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
        if(ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap.contains(Key))
        {
            auto item = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap[Key];
            int ibytePos = item.iBytePos;
            if(ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap.contains(item.modelType))
            {
                auto it = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[item.modelType]["OUTPUT"].byteInfoVec.begin();
                for(; it != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[item.modelType]["OUTPUT"].byteInfoVec.end();++it)
                {
                    int ibyteNum = it->byteNum;

                    if(it->key == "Contrlworld")
                    {
                        bitInfoMap = it->bitInfoMap;
                        cwBytePos = ibytePos;
                    }
                    else if(it->key == "Model")
                    {
                        BYTEINFO  byteInfo;
                        byteInfo.strType = it->type;
                        byteInfo.byteNum = ibyteNum;
                        byteInfo.bytePos = ibytePos;
                        byteInfoMap.insert(it->key,byteInfo);
                        BaseAxisOperate::ChangeAxisModel(it->type,ibyteNum,ibytePos,item.modelType.toInt());
                    }
                    else if(it->key == "Trapos")
                    {
                        BYTEINFO  byteInfo;
                        byteInfo.strType = it->type;
                        byteInfo.byteNum = ibyteNum;
                        byteInfo.bytePos = ibytePos;
                        byteInfoMap.insert(it->key,byteInfo);
                    }
                    else if(it->key == "Tratq")
                    {
                        BYTEINFO  byteInfo;
                        byteInfo.strType = it->type;
                        byteInfo.byteNum = ibyteNum;
                        byteInfo.bytePos = ibytePos;
                        byteInfoMap.insert(it->key,byteInfo);
                    }
                    else if(it->key == "Traspeed")
                    {
                        BYTEINFO  byteInfo;
                        byteInfo.strType = it->type;
                        byteInfo.byteNum = ibyteNum;
                        byteInfo.bytePos = ibytePos;
                        byteInfoMap.insert(it->key,byteInfo);
                    }
                    else if(it->key == "Accel")
                    {
                        BYTEINFO  byteInfo;
                        byteInfo.strType = it->type;
                        byteInfo.byteNum = ibyteNum;
                        byteInfo.bytePos = ibytePos;
                        byteInfoMap.insert(it->key,byteInfo);
                    }
                    else if(it->key == "Decel")
                    {
                        BYTEINFO  byteInfo;
                        byteInfo.strType = it->type;
                        byteInfo.byteNum = ibyteNum;
                        byteInfo.bytePos = ibytePos;
                        byteInfoMap.insert(it->key,byteInfo);
                    }
                    ibytePos += ibyteNum;
                }
            }
        }
        return cwBytePos;
    }
    int AnalysisInPutBytes(const QString &strKey, QMap<QString, BITINFO_STRU> &bitInfoMap,QMap<QString,BYTEINFO> &byteInfoMap)
    {
        int swBytePos = -1;
        bitInfoMap.clear();
        byteInfoMap.clear();
        QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
        if(ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap.contains(strKey))
        {
            int ibytePos = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap[strKey].iBytePos;
            QString ModelType = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlInputMap.hwSerInputMap[strKey].modelType;
            if(ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap.contains(ModelType))
            {
                auto it = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.begin();
                for(;it != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[ModelType]["INPUT"].byteInfoVec.end();++it)
                {
                    int ibyteNum = it->byteNum;
                    if(it->key == "Statusworld")
                    {
                        bitInfoMap = it->bitInfoMap;
                        swBytePos = ibytePos;
                    }
                    else if(it->key == "Errorcode")
                    {
                        BYTEINFO  byteInfo;
                        byteInfo.strType = it->type;
                        byteInfo.byteNum = ibyteNum;
                        byteInfo.bytePos = ibytePos;
                        byteInfoMap.insert(it->key,byteInfo);
                    }
                    else if(it->key == "Curpos")
                    {
                        BYTEINFO  byteInfo;
                        byteInfo.strType = it->type;
                        byteInfo.byteNum = ibyteNum;
                        byteInfo.bytePos = ibytePos;
                        byteInfoMap.insert(it->key,byteInfo);
                    }
                    else if(it->key == "Curtq")
                    {
                        BYTEINFO  byteInfo;
                        byteInfo.strType = it->type;
                        byteInfo.byteNum = ibyteNum;
                        byteInfo.bytePos = ibytePos;
                        byteInfoMap.insert(it->key,byteInfo);
                    }
                    ibytePos += ibyteNum;
                }
            }
        }
        return swBytePos;
    }
    void InsertORGStatus(const QString axis,QThread* OrgThread)
    {
        if(OrgThread == NULL)
            return;
        if(m_AxisORG.contains(axis))
            m_AxisORG[axis] = OrgThread;
        else
        {
            m_AxisORG.insert(axis,OrgThread);
        }
    }

public:
    QVector<QPair<int,int>> m_INSON;
    QVector<QPair<int,int>> m_OUTSON;
    QVector<QPair<int,int>> m_OFFSON;
    QMap<QString,QThread*> m_AxisORG;


};

#endif // MY_HW_SHARED_H
