#include "relmovcommand.h"
#include <MyEC_Shared.h>
#include <myshareconfig.h>
#include <hwconfig.h>
#include <QMutexLocker>
#include <QApplication>
#include <QDateTime>

RelMovCommand::RelMovCommand()
{
    m_moduleName = "MOV_RELPP";
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

void RelMovCommand::Initialization()
{
    BaseCalcFun::MemCopyTempData(tempOutputData);
    isTerminate = false;
    m_isInitStatOk = true;
    m_InBeginBytePos = -1;
    m_OutBeginBytePos = -1;
}

void RelMovCommand::SetModuleParam(cmdstru orderparam)
{
    m_relstru = orderparam.ppstru;
    m_axisid = m_relstru.dataheader.badrID;
}

void RelMovCommand::RunModule()
{
    GetAxisBeginBytePos();
    if(m_InBeginBytePos < 0|| m_OutBeginBytePos < 0)
    {
        m_isInitStatOk = false;
        return;
    }
    QVector<int> tempVec;
    tempVec.append(m_OutBeginBytePos);
    RelativeChange();
    //todo init ，send speed,pos
    SetAxisParm();
    ResetAllAxis(tempVec);
    if(!SetAxisInit(tempVec))
    {
        m_isInitStatOk = false;
        return;
    }
    RunAxis();
}

int RelMovCommand::GetExcResult(QString &strMsg)
{
    int iresult = -1;
    if(!m_isInitStatOk)
    {
        iresult = -2;
        strMsg = "初始化參數設置錯誤,son失败有故障，参数配置错误";
        qDebug()<<QDateTime::currentDateTime()<<"MOV_RELPP:"<<iresult<<strMsg;
        return iresult;
    }
    bool isInp = false;
    while (true)
    {
        if(isTerminate)//主循环发出退出标志
        {
            iresult = -99;
            StopAxis();
            strMsg = QString("%1 %2 Terminate!").arg(m_moduleName).arg(m_axisid);
             qDebug()<<QDateTime::currentDateTime()<<"MOV_RELPP::"<<iresult<<strMsg;
            return iresult;
        }
        //dosomething
        //解析输入数据获取当前位置，到位信号，故障信号
        int curPos = 0;
        int ireturn = GetInputData(curPos);
        if(ireturn == 0) //无故障信号
        {
//            if(qAbs(m_absstru.pos- curPos) > 200)
//            {
//                isInp = false;
//            }
//            else
//                isInp = true;
        }
        else if(ireturn == 1)
        {
            iresult = 1;
             strMsg = QString("指令名字：%1，指令ID：%2,指令位置：%3 axis run ok").arg(m_moduleName).arg(m_axisid).arg(m_relstru.pos);
             qDebug()<<QDateTime::currentDateTime()<<"MOV_RELPP::"<<iresult<<strMsg;
            break;
        }
        else //故障信号
        {
            iresult = -4;
            StopAxis();
            strMsg = QString("%1 axis %2 return error").arg(m_moduleName).arg(m_axisid);
            qDebug()<<QDateTime::currentDateTime()<<"MOV_RELPP::"<<iresult<<strMsg;
            return iresult;
        }
        //所有的轴ok
        if(isInp)
        {
            iresult = 1;
            strMsg = QString("指令名字：%1，指令ID：%2,指令位置：%3 axis run ok").arg(m_moduleName).arg(m_axisid).arg(m_relstru.pos);
             qDebug()<<QDateTime::currentDateTime()<<"MOV_RELPP::"<<iresult<<strMsg;
            break;
        }
        QThread::msleep(5);
        QApplication::processEvents(QEventLoop::AllEvents,100);
    }
    return iresult;
}

void RelMovCommand::StopModule()
{
  isTerminate = false;
}

FunModuleInterface *RelMovCommand::Copy()
{
    RelMovCommand *relcmd = new RelMovCommand;
    return relcmd;
}

void RelMovCommand::GetAxisBeginBytePos()
{
    if(MyShareconfig::GetInstance()->hwconfigstru.hwconfigInputMap.contains(0))//have ec type
    {
        if(MyShareconfig::GetInstance()->hwconfigstru.hwconfigInputMap[0].contains(m_axisid))//addr
        {
            if(MyShareconfig::GetInstance()->hwconfigstru.hwconfigInputMap[0][m_axisid].hwtype == "1")
            {
                m_InBeginBytePos = MyShareconfig::GetInstance()->hwconfigstru.hwconfigInputMap[0][m_axisid].curPos; //故障代碼在有兩個
            }
        }
    }
    if(MyShareconfig::GetInstance()->hwconfigstru.hwconfigOutputMap.contains(0))//have ec type
    {
        if(MyShareconfig::GetInstance()->hwconfigstru.hwconfigOutputMap[0].contains(m_axisid)){//
            if(MyShareconfig::GetInstance()->hwconfigstru.hwconfigOutputMap[0][m_axisid].hwtype == "1")
            {
                m_OutBeginBytePos = MyShareconfig::GetInstance()->hwconfigstru.hwconfigOutputMap[0][m_axisid].curPos; //故障代碼在有兩個
            }
        }
    }
}

void RelMovCommand::RelativeChange()
{
    BaseCalcFun::SetBitValue(6,m_OutBeginBytePos,1);
    QThread::msleep(50);
}

void RelMovCommand::SetAxisParm()
{
    int pos = m_relstru.pos;
    BaseCalcFun::MemCopyTempData(tempOutputData);
    QVector<QPair<int,int>> tempBytePosNum;
    QPair<int,int> pair;
    QMutexLocker locker(&MyShareconfig::GetInstance()->m_mutex);

    BaseAxisOperate::ChangeAxisModel("I8",1,m_OutBeginBytePos+2,1,tempOutputData);
    pair = QPair<int,int>(m_OutBeginBytePos+2,1);
    tempBytePosNum.append(pair);

    BaseAxisOperate::SetAxisPos("I32",4,m_OutBeginBytePos+3,pos,tempOutputData);
    pair = QPair<int,int>(m_OutBeginBytePos+3,4);
    tempBytePosNum.append(pair);

    //    BaseAxisOperate::SetAxisSpeed("U32",4,m_OutBeginBytePos+7,m_axisparamvec[3],tempOutputData);
    //    pair = QPair<int,int>(m_OutBeginBytePos+7,4);
    //    tempBytePosNum.append(pair);

    //    BaseAxisOperate::SetAxisACC("U32",4,m_OutBeginBytePos+11,m_axisparamvec[1],tempOutputData);
    //    pair = QPair<int,int>(m_OutBeginBytePos+11,4);
    //    tempBytePosNum.append(pair);

    //    BaseAxisOperate::SetAxisDCC("U32",4,m_OutBeginBytePos+15,m_axisparamvec[2],tempOutputData);
    //    pair= QPair<int,int>(m_OutBeginBytePos+15,4);
    //    tempBytePosNum.append(pair);

    BaseCalcFun::MemCopyOutputData(tempOutputData,tempBytePosNum);
    QThread::msleep(10);
}

void RelMovCommand::SetAxisModel()
{
    QMutexLocker locker(&MyShareconfig::GetInstance()->m_mutex);
    BaseAxisOperate::ChangeAxisModel("I8",1,m_OutBeginBytePos+2,1);
    for(int i = 0 ; i < 10 ;++i)
    {
        if(isTerminate)
        {
            qDebug()<<QDateTime::currentDateTime()<<"SetAxisModel MOV_RELPP: isTerminate";
            break;
        }
        if(1 == BaseCalcFun::GetByteValue("I8",1,m_InBeginBytePos+4))//记住修改点模式检测的map
        {
            break;
        }
        else
        {
            BaseAxisOperate::ChangeAxisModel("I8",1,m_OutBeginBytePos+2,1);
            QThread::msleep(20);
        }
        QThread::msleep(1);
    }
}

void RelMovCommand::ResetAllAxis(const QVector<int> &tempVec)
{
    BaseCalcFun::MemCopyTempData(tempOutputData);
    QVector<QPair<int,int>> tempBytePosNum;
    for(int i = 0;i < tempVec.size();++i)
    {
        BaseAxisOperate::SetAxisReset(7,tempVec[i],tempOutputData);
        QPair<int,int> pair = QPair<int,int>(tempVec[i],1);
        tempBytePosNum.append(pair);
    }
    BaseCalcFun::MemCopyOutputData(tempOutputData,tempBytePosNum);
    QThread::msleep(20);
    tempBytePosNum.clear();
    for(int i = 0;i < tempVec.size();++i)
    {
        BaseAxisOperate::ReAxisResetBit(7,tempVec[i],tempOutputData);
        QPair<int,int> pair = QPair<int,int>(tempVec[i],1);
        tempBytePosNum.append(pair);
    }
    BaseCalcFun::MemCopyOutputData(tempOutputData,tempBytePosNum);
    QThread::msleep(20);
}

bool RelMovCommand::SetAxisInit(const QVector<int> &tempVec)
{
    if(CheckAxisInit())
        return true;
    BaseAxisOperate::SetAxisExciteOff(m_tempOFFSON,tempVec,tempOutputData);
    QThread::msleep(50);
    BaseAxisOperate::SetAxisExcite(m_tempOutOP,tempVec,tempOutputData);
    QThread::msleep(50);
    for(int i = 0;i < 10;++i)
    {
        if(CheckAxisInit())
        {
            return true;
        }
        QThread::msleep(50);
    }
    return false;
}

bool RelMovCommand::CheckAxisInit()
{
    //检查是否有故障
    if(BaseAxisOperate::CheckAxisFault(3,2,m_InBeginBytePos+2))
        return false;
    //检查轴是否励磁成功
    if(!BaseAxisOperate::CheckAxisExcited(m_tempInOP,2,m_InBeginBytePos+2))
        return false;
    return true;
}

void RelMovCommand::RunAxis()
{

    BaseCalcFun::MemCopyTempData(tempOutputData);
    QVector<QPair<int,int>> tempBytePosNum;
    QVector<int> CWPosVec;
    //dosomething
    int ibytePos = m_OutBeginBytePos;
    CWPosVec.append(ibytePos);
    BaseAxisOperate::SetAxisRun(4,ibytePos,tempOutputData);
    QPair<int,int> pair0 = QPair<int,int>(ibytePos,1);
    tempBytePosNum.append(pair0);

    BaseCalcFun::MemCopyOutputData(tempOutputData,tempBytePosNum);
    QThread::msleep(20);
    tempBytePosNum.clear();
    for(int i = 0;i < CWPosVec.size();++i)
    {
        BaseAxisOperate::ReAxisRunBit(4,CWPosVec[i],tempOutputData);
        QPair<int,int> pair = QPair<int,int>(CWPosVec[i],1);
        tempBytePosNum.append(pair);
    }
    BaseCalcFun::MemCopyOutputData(tempOutputData,tempBytePosNum);
    QThread::msleep(20);
}

void RelMovCommand::StopAxis()
{
    BaseCalcFun::MemCopyTempData(tempOutputData);
    QVector<QPair<int,int>> tempBytePosNum;

    BaseAxisOperate::SetAxisStop(2,m_OutBeginBytePos,tempOutputData);
    QPair<int,int> pair = QPair<int,int>(m_OutBeginBytePos,1);
    tempBytePosNum.append(pair);

    BaseCalcFun::MemCopyOutputData(tempOutputData,tempBytePosNum);
    QThread::msleep(30);
    tempBytePosNum.clear();

    BaseAxisOperate::ReAxisStopBit(2,m_OutBeginBytePos,tempOutputData);
    pair = QPair<int,int>(m_OutBeginBytePos,1);
    tempBytePosNum.append(pair);
    BaseCalcFun::MemCopyOutputData(tempOutputData,tempBytePosNum);
    QThread::msleep(30);
}

int RelMovCommand::GetInputData(int &curPos)
{
    QMutexLocker locker(&MyShareconfig::GetInstance()->m_mutex);
    int iResult = 0;
    if(BaseAxisOperate::CheckAxisFault(3,2,m_InBeginBytePos+2))
    {
        iResult = -1;
        qDebug()<<"指令故障:"<<iResult;
        return iResult;
    }
    if(BaseAxisOperate::CheckAxisINP(10,2,m_InBeginBytePos+2))
    {
        iResult = 1;
        qDebug()<<"MOV_RELPP到位信号:"<<iResult;
        return iResult;
    }
    curPos = BaseAxisOperate::GetAxisCurPos("I32",4,m_InBeginBytePos+5);
    return iResult;
}
