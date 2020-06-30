#include "analysisdatalib.h"
#include <QDir>
#include <QCoreApplication>
#include <QPluginLoader>
#include <QDebug>
#include "ConfigfileRW/readhwconfigfile.h"
#include <MyEC_Shared.h>
#include "CommandFun/G00_Pulgin/g00_pulgin.h"
#include "CommandFun/RelMov/relmovcommand.h"
#include "CommandFun/SetADS/setaccdecspeedcmd.h"

AnalysisDataLib::AnalysisDataLib()
{
}

void AnalysisDataLib::LoadPluginsmodule()
{
   // QDir plugindir = QDir(QCoreApplication::applicationDirPath()+"/plugins");
    MyShareconfig::GetInstance()->pluginMap.clear();
//    foreach(QString filename,plugindir.entryList(QDir::Files))
//    {
//        QPluginLoader loader(plugindir.absoluteFilePath(filename));

 //       if (FunModuleInterface * base_moudle = qobject_cast<FunModuleInterface *>(loader.instance()))
//        {
 //          if(base_moudle)
//           {
//               qDebug() << base_moudle->GetModuleName();
//             ClassicladderConfigData::GetInstance()->pluginMap.insert(base_moudle->GetModuleName(),base_moudle);
//            }
//        }
//    }
    G00_Pulgin * base_moudle = new G00_Pulgin();
    qDebug() <<"LoadPluginsmodule"<< base_moudle->GetModuleName();
    if(base_moudle)
    {
        MyShareconfig::GetInstance()->pluginMap.insert(base_moudle->GetModuleName(),base_moudle);
    }
    SetAccDecSpeedcmd *ads = new SetAccDecSpeedcmd();
    qDebug() <<"LoadPluginsmodule"<< ads->GetModuleName();
    if(ads)
    {
        MyShareconfig::GetInstance()->pluginMap.insert(ads->GetModuleName(),ads);
    }
    RelMovCommand *relmov= new RelMovCommand();
    qDebug() <<"LoadPluginsmodule"<< relmov->GetModuleName();
    if(relmov)
    {
        MyShareconfig::GetInstance()->pluginMap.insert(relmov->GetModuleName(),relmov);
    }
}

void AnalysisDataLib::ReadHWConfigFile(QString path)
{
    ReadHWconfigFile hwobj;
    hwobj.ReadHWconfigType(MyShareconfig::GetInstance()->hwconfigstru,path);
}
///
/// \brief AnalysisDataLib::SetS_ONParam
///获取固定的SON相关参数
void AnalysisDataLib::SetS_ONParam()
{
    //励磁参数设置,设置为固定的值
    MyShareconfig::GetInstance()->hwconfigstru.hwconfigSonstru.InPutVec.clear();
    MyShareconfig::GetInstance()->hwconfigstru.hwconfigSonstru.OutPutVec.clear();
    BIT_VALUE_STRU instru;
    instru.bit = 0;
    instru.value = 1;
    MyShareconfig::GetInstance()->hwconfigstru.hwconfigSonstru.InPutVec.append(instru);
    instru.bit = 1;
    instru.value = 1;
    MyShareconfig::GetInstance()->hwconfigstru.hwconfigSonstru.InPutVec.append(instru);
    instru.bit = 2;
    instru.value = 1;
    MyShareconfig::GetInstance()->hwconfigstru.hwconfigSonstru.InPutVec.append(instru);
    instru.bit = 6;
    instru.value = 0;
    MyShareconfig::GetInstance()->hwconfigstru.hwconfigSonstru.InPutVec.append(instru);
    instru.bit = 1;
    instru.value = 1;
    MyShareconfig::GetInstance()->hwconfigstru.hwconfigSonstru.OutPutVec.append(instru);
    instru.bit = 2;
    instru.value = 1;
    MyShareconfig::GetInstance()->hwconfigstru.hwconfigSonstru.OutPutVec.append(instru);
    instru.bit = 0;
    instru.value = 1;
    MyShareconfig::GetInstance()->hwconfigstru.hwconfigSonstru.OutPutVec.append(instru);
    instru.bit = 3;
    instru.value = 1;
    MyShareconfig::GetInstance()->hwconfigstru.hwconfigSonstru.OutPutVec.append(instru);
}
////
/// \brief AnalysisDataLib::CalcECbytePosandNum
/// \param stru
/// 根据ethercat配置的位置进行排序
void AnalysisDataLib::CalcECbytePosandNum(HWCONFIG_STRU &stru)
{
    //实现INPUT硬件排序的操作 can类型排序位置是和ec类型的的硬件排序问题
    for(auto it = stru.hwconfigInputMap.begin(); it != stru.hwconfigInputMap.end(); ++it)
    {
        if(it.key() == 0)
        {
            int inECbytepos = 0;
            for(auto item =  it.value().begin(); item != it.value().end(); ++ item)
            {
                item.value().curPos = inECbytepos;
                inECbytepos += item.value().size;
            }
            stru.sumInNum = inECbytepos;
        }
        else if(it.key() == 1){
            int inCANbitpos = 0;
            for(auto item =  it.value().begin(); item != it.value().end(); ++ item)
            {
                item.value().curPos = inCANbitpos;
                inCANbitpos += item.value().size;
            }
        }
    }
    //实现OUTPUT硬件排序的操作 can类型排序位置是和ec类型的的硬件排序问题
    for(auto it = stru.hwconfigOutputMap.begin(); it != stru.hwconfigOutputMap.end(); ++it)
    {
        if(it.key() == 0)
        {
            int OutECbytepos = 0;
            for(auto item =  it.value().begin(); item != it.value().end(); ++ item)
            {
                item.value().curPos = OutECbytepos;
                OutECbytepos += item.value().size;
            }
            stru.sumOutNum = OutECbytepos;
        }
        else if(it.key() == 1){
            int OutCANbitpos = 0;
            for(auto item =  it.value().begin(); item != it.value().end(); ++ item)
            {
                item.value().curPos = OutCANbitpos;
                OutCANbitpos += item.value().size;
            }
        }
    }
}

bool AnalysisDataLib::MallocECbyteSize()
{
    if(MyShareconfig::GetInstance()->hwconfigstru.sumInNum > 0 && MyShareconfig::GetInstance()->hwconfigstru.sumOutNum > 0)
    {
        InOutPutData::GetInstance()->inPutNum = MyShareconfig::GetInstance()->hwconfigstru.sumInNum;
        InOutPutData::GetInstance()->outPutNum = MyShareconfig::GetInstance()->hwconfigstru.sumOutNum;
        if(InOutPutData::GetInstance()->inPutData)
        {
            InOutPutData::GetInstance()->inPutData = NULL;
        }
        if( InOutPutData::GetInstance()->outPutData)
        {
             InOutPutData::GetInstance()->inPutData = NULL;
        }
        InOutPutData::GetInstance()->ifName = NULL;
        InOutPutData::GetInstance()->inPutData = new uchar[InOutPutData::GetInstance()->inPutNum] ;
        InOutPutData::GetInstance()->outPutData = new uchar[InOutPutData::GetInstance()->outPutNum];
        memset(InOutPutData::GetInstance()->inPutData, 0, sizeof(uchar) * InOutPutData::GetInstance()->inPutNum);
        memset(InOutPutData::GetInstance()->outPutData, 0, sizeof(uchar) * InOutPutData::GetInstance()->outPutNum);

        QByteArray array = MyShareconfig::GetInstance()->hwconfigstru.netName.toLatin1();
            int size = strlen (array.data());
        if(size == 0)
        {

            return false;
        }
       // InOutPutData::GetInstance()->ifName = array.data();
        InOutPutData::GetInstance()->ifName = new char[size+1];
        memcpy(InOutPutData::GetInstance()->ifName, array.data(),size+1);
        for(int i = 0; i < size; ++i)
        {
            qDebug()<<"i:"<<i << "value:"<<InOutPutData::GetInstance()->ifName[i];
        }
        return true;
     qDebug()<<"ifname:"<<QString(InOutPutData::GetInstance()->ifName);
    }
    else{
     return false;
    }
}
