#include "reset.h"

Reset::Reset()
{
    m_moduleName = "SETRESET";
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

void Reset::Initialization()
{
    //todo Initialization variable
    isTerminate = false;
    BaseCalcFun::MemCopyTempData(tempOutputData);
    m_InBeginBytePos = -1;
    m_OutBeginBytePos = -1;
}

void Reset::SetModuleParam(cmdstru orderparam)
{
    m_resetstru = orderparam.resetstru;
}

void Reset::RunModule()
{
    GetAxisBeginBytePos();
}

int Reset::GetExcResult(QString &strMsg)
{
    int iresult = 0;
    if(m_InBeginBytePos<0 || m_OutBeginBytePos<0)
    {
        iresult = -2;
        strMsg = "根据传入的参数ID，不能找到对应的字节位置，请确认！";
    }
    //检查是否有故障
    if(BaseAxisOperate::CheckAxisFault(3,2,m_InBeginBytePos+2))
    {
        BaseAxisOperate::SetAxisReset(7,m_OutBeginBytePos);//設置軸復位

        if(BaseAxisOperate::CheckAxisFault(3,2,m_InBeginBytePos+2))
        {
            iresult = -1;
          strMsg = "故障清除失败";
        }
        else{
            iresult = 1;
          strMsg = "故障清除 成功";
        }
    }
    return iresult;
}

void Reset::StopModule()
{
    isTerminate = false;
}

FunModuleInterface *Reset::Copy()
{
    Reset *reset = new Reset;
    return reset;
}

void Reset::GetAxisBeginBytePos()
{
    uint8_t id = m_resetstru.dataheader.badrID;
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
