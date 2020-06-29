#include "setaccdecspeedcmd.h"

SetAccDecSpeedcmd::SetAccDecSpeedcmd()
{
    m_moduleName = "SETACCDECSPEED";
    if(InOutPutData::GetInstance()->outPutNum <= 0)
        return;
    tempOutputData = new uchar[InOutPutData::GetInstance()->outPutNum];
    //获取检查励磁是否成功的参数（bit，vaule）
    foreach (BIT_VALUE_STRU item, MyShareconfig::GetInstance()->hwconfigstru.hwconfigSonstru.InPutVec) {
        QPair<int,int> pair = QPair<int,int>(item.bit,item.value);
        m_tempInOP.append(pair);
    }
    //获取励磁的参数（bit，vaule）
    foreach (BIT_VALUE_STRU item, MyShareconfig::GetInstance()->hwconfigstru.hwconfigSonstru.OutPutVec) {
        QPair<int,int> pair = QPair<int,int>(item.bit,item.value);
        m_tempOutOP.append(pair);
        int offValue = 0;
        if(item.value == 0)
        {
            offValue = 1;
        }
        QPair<int,int> pairoff = QPair<int,int>(item.bit,offValue);
        m_tempOFFSON.append(pairoff);
    }
}

void SetAccDecSpeedcmd::Initialization()
{
    //todo Initialization variable
    isTerminate = false;
    BaseCalcFun::MemCopyTempData(tempOutputData);
    m_InBeginBytePos = -1;
    m_OutBeginBytePos = -1;
}

void SetAccDecSpeedcmd::SetModuleParam(cmdstru orderparam)
{
  m_paramstru = orderparam.setparamstru;
}

void SetAccDecSpeedcmd::RunModule()
{
    GetAxisBeginBytePos();
}

int SetAccDecSpeedcmd::GetExcResult(QString &strMsg)
{
    int iresult = 0;
    if(m_InBeginBytePos<0 || m_OutBeginBytePos<0)
    {
        iresult = -2;
        strMsg = "根据传入的参数ID，不能找到对应的字节位置，请确认！";
    }
   else{
        SetCommandsParam();
        iresult =1;
        strMsg = "参数设置成功";
    }
    return iresult;
}
///
/// \brief SetAccDecSpeedcmd::StopModule
///
void SetAccDecSpeedcmd::StopModule()
{
    isTerminate = false;
}

FunModuleInterface *SetAccDecSpeedcmd::Copy()
{
    SetAccDecSpeedcmd *Setparamcmd = new SetAccDecSpeedcmd();
    return Setparamcmd;
}
///
/// \brief SetAccDecSpeedcmd::SetCommandsParam
///根据进行赋值
void SetAccDecSpeedcmd::SetCommandsParam()
{
    BaseCalcFun::MemCopyTempData(tempOutputData);
    QVector<QPair<int,int>> tempBytePosNum;
    QPair<int,int> pair;
    QMutexLocker locker(&MyShareconfig::GetInstance()->m_mutex);
    BaseAxisOperate::SetAxisSpeed("U32",4,m_OutBeginBytePos+7,m_paramstru.speed,tempOutputData);
    pair = QPair<int,int>(m_OutBeginBytePos+7,4);
    tempBytePosNum.append(pair);

    BaseAxisOperate::SetAxisACC("U32",4,m_OutBeginBytePos+11,m_paramstru.acc,tempOutputData);
    pair = QPair<int,int>(m_OutBeginBytePos+11,4);
    tempBytePosNum.append(pair);

    BaseAxisOperate::SetAxisDCC("U32",4,m_OutBeginBytePos+15,m_paramstru.dec,tempOutputData);
    pair= QPair<int,int>(m_OutBeginBytePos+15,4);
    tempBytePosNum.append(pair);

    BaseCalcFun::MemCopyOutputData(tempOutputData,tempBytePosNum);
    QThread::msleep(10);
}

void SetAccDecSpeedcmd::GetAxisBeginBytePos()
{
    uint8_t id = m_paramstru.dataheader.badrID;
    if(MyShareconfig::GetInstance()->hwconfigstru.hwconfigInputMap.contains(0))//have ec type
    {
        if(MyShareconfig::GetInstance()->hwconfigstru.hwconfigInputMap[0].contains(id))//addr
        {
            if(MyShareconfig::GetInstance()->hwconfigstru.hwconfigInputMap[0][id].hwtype == "1")
            {
                m_InBeginBytePos = MyShareconfig::GetInstance()->hwconfigstru.hwconfigInputMap[0][id].curPos; //
            }
        }
    }
    if(MyShareconfig::GetInstance()->hwconfigstru.hwconfigOutputMap.contains(0))//have ec type
    {
        if(MyShareconfig::GetInstance()->hwconfigstru.hwconfigOutputMap[0].contains(id)){//
            if(MyShareconfig::GetInstance()->hwconfigstru.hwconfigOutputMap[0][id].hwtype == "1")
            {
                m_OutBeginBytePos = MyShareconfig::GetInstance()->hwconfigstru.hwconfigOutputMap[0][id].curPos; //
            }
        }
    }
}
