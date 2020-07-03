#include "soff.h"

SOFF::SOFF()
{
    m_moduleName = "SETOFF";
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

void SOFF::Initialization()
{
    //todo Initialization variable
    isTerminate = false;
    BaseCalcFun::MemCopyTempData(tempOutputData);
    m_InBeginBytePos = -1;
    m_OutBeginBytePos = -1;
}

void SOFF::SetModuleParam(cmdstru orderparam)
{
    m_soffstru = orderparam.soffstru;
}

void SOFF::RunModule()
{
    GetAxisBeginBytePos();
}

int SOFF::GetExcResult(QString &strMsg)
{
    int iresult = 0;
    if(m_InBeginBytePos<0 || m_OutBeginBytePos<0)
    {
        iresult = -2;
        strMsg = "根据传入的参数ID，不能找到对应的字节位置，请确认！";
    }
    else{
        if(SetAxisoff())
        {
            iresult =1;
            strMsg = "soff 成功";
        }
        else{
            iresult =-1;
            strMsg = "soff 失败";
        }
    }
    return iresult;
}

void SOFF::StopModule()
{
    isTerminate = false;
}

FunModuleInterface *SOFF::Copy()
{
    SOFF *soff = new SOFF;
    return soff;
}

void SOFF::GetAxisBeginBytePos()
{
    uint8_t id = m_soffstru.dataheader.badrID;
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

bool SOFF::SetAxisoff()
{
    QVector<int> tempVec;
    tempVec.append(m_OutBeginBytePos);
    BaseAxisOperate::SetAxisExciteOff(m_tempOFFSON,tempVec,tempOutputData);
    QThread::msleep(50);//延时时间原来50ms
    return true;
}
