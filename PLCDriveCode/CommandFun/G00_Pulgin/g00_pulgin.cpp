#include "g00_pulgin.h"
#include <sys/time.h>
#include <MyEC_Shared.h>
#include <myshareconfig.h>
#include <hwconfig.h>
#include <QMutexLocker>
#include <QApplication>
#include <QDateTime>
G00_Pulgin::G00_Pulgin()
{
    m_moduleName = "MOV_ABSPP";
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
void G00_Pulgin::Initialization()
{
    //todo Initialization variable
    m_InitOk = true;
    isTerminate = false;
    BaseCalcFun::MemCopyTempData(tempOutputData);
    m_axisparamvec.clear();
    m_InBeginBytePos = -1;
    m_OutBeginBytePos = -1;
}

void G00_Pulgin::SetModuleParam(cmdstru stru)
{
   // GetAxisVarParamValue(orderparam);//Var str spilt
    m_absstru = stru.ppstru;
    m_axisid = stru.ppstru.dataheader.badrID;

}

void G00_Pulgin::RunModule()
{
    GetAxisBeginBytePos();
    if(m_InBeginBytePos < 0|| m_OutBeginBytePos < 0)
    {
        m_InitOk = false;
        return;
    }
    //todo init set speed pos acc dec s_on
    SetAxisCWBit6();//bit 設置爲0
    SetAxisModel();
    SetAxisParam();
    if(!SetAxisInit())
    {
        m_InitOk = false;
         qDebug()<<"SetAxisInit函数出现故障错误";
        return;
    }
}

int  G00_Pulgin::GetExcResult(QString &strMsg)
{
    int iresult = -1;
    if(!m_InitOk)
    {
        iresult = -2;
        strMsg = "G00 初始化參數設置錯誤";
        qDebug()<<QDateTime::currentDateTime()<<"G00:"<<iresult<<strMsg;
        return iresult;
    }
    bool isInp = false;
     RunAxis();
    while (true)
    {
        if(isTerminate)//主循环发出退出标志
        {
            iresult = -99;
            StopAxis();
            strMsg = QString("%1 %2 Terminate!").arg(m_moduleName).arg(m_axisid);
             qDebug()<<QDateTime::currentDateTime()<<"G00::"<<iresult<<strMsg;
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
            strMsg = QString("%1 axis run ok").arg(m_axisid);
             qDebug()<<QDateTime::currentDateTime()<<"G00::"<<iresult<<strMsg;
            break;
        }
        else //故障信号
        {
            iresult = -4;
            StopAxis();
            strMsg = QString("%1 axis %2 return error").arg(m_moduleName).arg(m_axisid);
            qDebug()<<QDateTime::currentDateTime()<<"G00::"<<iresult<<strMsg;
            return iresult;
        }
        //所有的轴ok
        if(isInp)
        {
           //  WriteVar(200,9,curPos);
            iresult = 1;
            strMsg = QString("%1 axis run ok").arg(m_axisid);
             qDebug()<<QDateTime::currentDateTime()<<"G00::"<<iresult<<strMsg;
            break;
        }
        QThread::msleep(5);
        QApplication::processEvents(QEventLoop::AllEvents,100);
    }
    return iresult;
}
void G00_Pulgin::StopModule()
{
    isTerminate = false;
}

FunModuleInterface *G00_Pulgin::Copy()
{
    G00_Pulgin *G00 = new G00_Pulgin();
    return G00;
}

void G00_Pulgin::GetAxisVarParamValue(QString str)
{
    m_axisparamvec.clear();
    QStringList varlist = str.split(" ");
    if(varlist.size() != 4)
    {
        return;
    }
    if(varlist[0] != "G00")
    {
        return;
    }
    QString axisname = varlist[1];
    int VarStartindex = -1;
   if(axisname.startsWith("@290/")&&axisname.endsWith('@'))
   {
     QString X = axisname.mid(5);
     X.chop(1);
     VarStartindex = X.toUInt();   
   }
   if(VarStartindex < 0)
   {
       return;
   }
   int id /*= ReadVar(290,VarStartindex);*/;
   m_axisparamvec.append(id);
    QString accstr =  varlist[2];
    int accindex= -1;
    if(accstr.startsWith("@200/")&&accstr.endsWith('@'))
    {
      QString X = accstr.mid(5);
      X.chop(1);
      accindex = X.toUInt();
    }
    for(int i = accindex; i < accindex +3; ++i)
    {
       int value/*  =  ReadVar(200,i)*/;
        m_axisparamvec.append(value);
    }
    int valuepos = 0;
    QString posvar  =  varlist[3];
    if(posvar.startsWith("#"))
    {
        QString value = posvar.mid(1);
        valuepos = value.toInt();
    }
    else if (posvar.startsWith("@200/")&&posvar.endsWith('@'))
    {
        QString Xstr = posvar.mid(5);
                  Xstr.chop(1);
                int   Varindex = Xstr.toUInt();
       // valuepos = ReadVar(200,Varindex);
    }
    m_axisparamvec.append(valuepos);
    qDebug()<<QDateTime::currentDateTime()<<"G00插件参数的值"<<m_axisparamvec;
}

void G00_Pulgin::GetAxisBeginBytePos()
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
void G00_Pulgin::SetAxisCWBit6()
{
    QMutexLocker locker(&MyShareconfig::GetInstance()->m_mutex);
    BaseCalcFun::MemCopyTempData(tempOutputData);
    QVector<QPair<int,int>> tempBytePosNum;
    BaseCalcFun::SetBitValue(6,m_OutBeginBytePos,0,tempOutputData);
    QPair<int,int> pair = QPair<int,int>(m_OutBeginBytePos,1);
    tempBytePosNum.append(pair);
    BaseCalcFun::MemCopyOutputData(tempOutputData,tempBytePosNum);
}
void G00_Pulgin::SetAxisModel()
{
    QMutexLocker locker(&MyShareconfig::GetInstance()->m_mutex);
    BaseAxisOperate::ChangeAxisModel("I8",1,m_OutBeginBytePos+2,1);
    for(int i = 0 ; i < 10 ;++i)
    {
        if(isTerminate)
        {
            qDebug()<<QDateTime::currentDateTime()<<"SetAxisModel G00: isTerminate";
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

void G00_Pulgin::SetAxisParam()
{
    int pos = m_absstru.pos;
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

bool G00_Pulgin::SetAxisInit()
{
    if(CheckAxisInit())
        return true;
    QVector<int> tempVec;
    tempVec.append(m_OutBeginBytePos);
    BaseAxisOperate::SetAxisExciteOff(m_tempOFFSON,tempVec,tempOutputData);
    QThread::msleep(10);//延时时间原来50ms
    BaseAxisOperate::SetAxisExcite(m_tempOutOP,tempVec,tempOutputData);
    QThread::msleep(10);//延时时原来50ms
    if(CheckAxisInit())
    {
        return true;
    }
     return false;
}

bool G00_Pulgin::CheckAxisInit()
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
void G00_Pulgin::RunAxis()
{
    BaseCalcFun::MemCopyTempData(tempOutputData);
    QVector<QPair<int,int>> tempBytePosNum;
    QPair<int,int> pair;
    BaseAxisOperate::SetAxisRun(4,m_OutBeginBytePos,tempOutputData);
    pair  = QPair<int,int>(m_OutBeginBytePos,1);
    tempBytePosNum.append(pair);

    BaseCalcFun::MemCopyOutputData(tempOutputData,tempBytePosNum);
    QThread::msleep(30);
    tempBytePosNum.clear();

    BaseAxisOperate::ReAxisRunBit(4,m_OutBeginBytePos,tempOutputData);
    pair = QPair<int,int>(m_OutBeginBytePos,1);
    tempBytePosNum.append(pair);

    BaseCalcFun::MemCopyOutputData(tempOutputData,tempBytePosNum);
    QThread::msleep(20);
}

void G00_Pulgin::StopAxis()
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

int G00_Pulgin::GetInputData(int &curPos)
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
          qDebug()<<"G00到位信号:"<<iResult;
       return iResult;
    }
    curPos = BaseAxisOperate::GetAxisCurPos("I32",4,m_InBeginBytePos+5);
    return iResult;
}
