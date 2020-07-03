#include "son.h"

SON::SON()
{
    m_moduleName = "SETSON";
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

void SON::Initialization()
{
    //todo Initialization variable
    isTerminate = false;
    BaseCalcFun::MemCopyTempData(tempOutputData);
    m_InBeginBytePos = -1;
    m_OutBeginBytePos = -1;
}

void SON::SetModuleParam(cmdstru orderparam)
{
    m_sonstru = orderparam.sonstru;
}

void SON::RunModule()
{
    GetAxisBeginBytePos();
}

int SON::GetExcResult(QString &strMsg)
{
    int iresult = 0;
    if(m_InBeginBytePos<0 || m_OutBeginBytePos<0)
    {
        iresult = -2;
        strMsg = "根据传入的参数ID，不能找到对应的字节位置，请确认！";
    }
    else{
        if(SetAxisInit())
        {
            iresult =1;
            strMsg = "son 成功";
        }
        else{
            iresult =-1;
            strMsg = "son 失败";
        }
    }
    return iresult;
}

void SON::StopModule()
{
    isTerminate = false;
}

FunModuleInterface *SON::Copy()
{
    SON *son = new SON;
    return son;
}

void SON::GetAxisBeginBytePos()
{
    uint8_t id = m_sonstru.dataheader.badrID;
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

bool SON::SetAxisInit()
{
    if(CheckAxisInit())
        return true;
    QVector<int> tempVec;
    tempVec.append(m_OutBeginBytePos);

    BaseAxisOperate::SetAxisExciteOff(m_tempOFFSON,tempVec,tempOutputData);
    QThread::msleep(50);//延时时间原来50ms
    BaseAxisOperate::SetAxisExcite(m_tempOutOP,tempVec,tempOutputData);
    QThread::msleep(50);//延时时原来50ms
    if(CheckAxisInit())
    {
        return true;
    }
    return false;
}

bool SON::CheckAxisInit()
{
    //检查是否有故障
    if(BaseAxisOperate::CheckAxisFault(3,2,m_InBeginBytePos+2))
    {
        BaseAxisOperate::SetAxisReset(7,m_OutBeginBytePos);//設置軸復位
        if(BaseAxisOperate::CheckAxisFault(3,2,m_InBeginBytePos+2))
        {
            return false;
        }
    }
    //检查轴是否励磁成功
    if(!BaseAxisOperate::CheckAxisExcited(m_tempInOP,2,m_InBeginBytePos+2))
        return false;
    return true;
}
