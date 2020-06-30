#include "sendcommandsclass.h"

SendCommandsClass::SendCommandsClass()
{
    m_tempInOP.clear();
}

SendCommandsClass::~SendCommandsClass()
{

}

bool SendCommandsClass::Initallaxisparam()
{
    //检查励磁
    foreach (BIT_VALUE_STRU item, MyShareconfig::GetInstance()->hwconfigstru.hwconfigSonstru.InPutVec) {
        QPair<int,int> pair = QPair<int,int>(item.bit,item.value);
        m_tempInOP.append(pair);
    }
    if(m_tempInOP.size() <=0)
        return false;
    return true;
}
///
/// \brief SendCommandsClass::StopComandsfun
///直接作用于轴停止
void SendCommandsClass::StopComandsfun(QVector<int>axisidvec)
{
    QVector<int> CWPosVec;
    for(int i = 0 ; i < axisidvec.size(); ++i)
    {
        int inpos= -1;
        int outpos = -1;
        GetAxisBeginBytePos(axisidvec[i],inpos,outpos);
        if(inpos!=-1 &&  outpos!=-1)
        {
            CWPosVec.append(outpos);
        }
    }
    if(InOutPutData::GetInstance()->outPutNum <= 0)
        return;
      QMutexLocker locker(&MyShareconfig::GetInstance()->m_mutex);
    uchar *  tempOutputData = new uchar[InOutPutData::GetInstance()->outPutNum];
    BaseCalcFun::MemCopyTempData(tempOutputData);
    QVector<QPair<int,int>> tempBytePosNum;
    for(int i = 0;i < CWPosVec.size();++i)
    {
        BaseAxisOperate::SetAxisStop(2,CWPosVec[i],tempOutputData);
        QPair<int,int> pair = QPair<int,int>(CWPosVec[i],1);
        tempBytePosNum.append(pair);
    }
    BaseCalcFun::MemCopyOutputData(tempOutputData,tempBytePosNum);
    QThread::msleep(30);
    tempBytePosNum.clear();
    for(int i = 0;i < CWPosVec.size();++i)
    {
        BaseAxisOperate::ReAxisStopBit(2,CWPosVec[i],tempOutputData);
        QPair<int,int> pair = QPair<int,int>(CWPosVec[i],1);
        tempBytePosNum.append(pair);
    }
    BaseCalcFun::MemCopyOutputData(tempOutputData,tempBytePosNum);
    QThread::msleep(30);
    delete tempOutputData;
}
///
/// \brief SendCommandsClass::GetCurPosfun
/// \param axisidvec
/// \return
//////获得所有的当前的状态
QVector<int> SendCommandsClass::GetCurValuefun(QVector<int> axisidvec,int role)
{
    QVector<int> values;
    for(int i = 0; i < axisidvec.size(); ++i)
    {
        int value = -1;
        int axisid = axisidvec[i];
        int axiscurpos= -1;
        int outpos = -1;
        GetAxisBeginBytePos(axisid,axiscurpos,outpos);
        if((axiscurpos==-1 )|| (outpos==-1))
        {
            continue;
        }
        switch (role) {
        case curpos:
            //当前位置
            value =  BaseAxisOperate::GetAxisCurPos("I32",4,axiscurpos+5);
            values.append(value);
            break;
        case ErrorCode:
            value= BaseAxisOperate::GetAxisErrorCode("U16",2,axiscurpos);
            values.append(value);
            break;
        case Fault:   //轴故障位
            value = BaseAxisOperate::CheckAxisFault(3,2,axiscurpos+2);
            values.append(value);
            break;
        case Runstate:   //轴是否在停止状态
            if(MyShareconfig::GetInstance()->m_Runstate[i] /*&&(!BaseCalcFun::GetB.`itValue(5,2,axiscurpos+2))*/)
            {
                value = 1;
            }
            else{
                value = 0;
            }
            values.append(value);
            break;
        case AxisINP: //轴到位信号
            value= BaseAxisOperate::CheckAxisINP(10,2,axiscurpos+2);
            values.append(value);
            break;
        case Excited:
            //励磁状态监视
            value = BaseAxisOperate::CheckAxisExcited(m_tempInOP,2,axiscurpos+2);
            values.append(value);
        case curmodel:  //轴模式检测
            value = BaseCalcFun::GetByteValue("I8",1,axiscurpos+4);//记住修改点模式
            values.append(value);
            break;
        default:
            break;
        }
    }
    return values;
}

///
/// \brief SendCommandsClass::GetAxisBeginBytePos
/// \param axisid
/// 得到控制字的位置
void SendCommandsClass::GetAxisBeginBytePos(int axisid,int &inbeginbytepos,int &outbeginbytepos)
{
    if(MyShareconfig::GetInstance()->hwconfigstru.hwconfigInputMap.contains(0))//have ec type
    {
        if(MyShareconfig::GetInstance()->hwconfigstru.hwconfigInputMap[0].contains(axisid))//addr
        {
            if(MyShareconfig::GetInstance()->hwconfigstru.hwconfigInputMap[0][axisid].hwtype == "1")
            {
                inbeginbytepos = MyShareconfig::GetInstance()->hwconfigstru.hwconfigInputMap[0][axisid].curPos; //
            }
        }
    }
    if(MyShareconfig::GetInstance()->hwconfigstru.hwconfigOutputMap.contains(0))//have ec type
    {
        if(MyShareconfig::GetInstance()->hwconfigstru.hwconfigOutputMap[0].contains(axisid)){//
            if(MyShareconfig::GetInstance()->hwconfigstru.hwconfigOutputMap[0][axisid].hwtype == "1")
            {
                outbeginbytepos = MyShareconfig::GetInstance()->hwconfigstru.hwconfigOutputMap[0][axisid].curPos; //
            }
        }
    }
}
///
/// \brief SendCommandsClass::SetAxisParam
/// \param axisidvec
///设置轴参数
void SendCommandsClass::SetAxisParam(QVector<uint> values,int id)
{
      QMutexLocker locker(&MyShareconfig::GetInstance()->m_mutex);
    int axiscurpos= -1;
    int OutBeginBytePos = -1;
    GetAxisBeginBytePos(id,axiscurpos,OutBeginBytePos);
    if(values.size() != 3)
        return;
    if((axiscurpos==-1 )|| (OutBeginBytePos==-1))
    {
        return;
    }
    if(InOutPutData::GetInstance()->outPutNum <= 0)
        return;
    uchar *  tempOutputData = new uchar[InOutPutData::GetInstance()->outPutNum];
    BaseCalcFun::MemCopyTempData(tempOutputData);
    QVector<QPair<int,int>> tempBytePosNum;
    QPair<int,int> pair;
    BaseAxisOperate::SetAxisSpeed("U32",4,OutBeginBytePos+7,values[2],tempOutputData);
    pair = QPair<int,int>(OutBeginBytePos+7,4);
    tempBytePosNum.append(pair);

    BaseAxisOperate::SetAxisACC("U32",4,OutBeginBytePos+11,values[0],tempOutputData);
    pair = QPair<int,int>(OutBeginBytePos+11,4);
    tempBytePosNum.append(pair);

    BaseAxisOperate::SetAxisDCC("U32",4,OutBeginBytePos+15,values[1],tempOutputData);
    pair= QPair<int,int>(OutBeginBytePos+15,4);
    tempBytePosNum.append(pair);

    BaseCalcFun::MemCopyOutputData(tempOutputData,tempBytePosNum);
    QThread::msleep(10);
}
///
/// \brief SendCommandsClass::SetAxisReset
/// \param id
///  轴复位指令
void SendCommandsClass::SetAxisReset(int id)
{
   QMutexLocker locker(&MyShareconfig::GetInstance()->m_mutex);
    int axiscurpos= -1;
    int OutBeginBytePos = -1;
    GetAxisBeginBytePos(id,axiscurpos,OutBeginBytePos);
    if((axiscurpos==-1 )|| (OutBeginBytePos==-1))
    {
        return;
    }
    if(InOutPutData::GetInstance()->outPutNum <= 0)
        return;
    BaseAxisOperate::SetAxisReset(7,OutBeginBytePos);//設置軸復位
}
///
/// \brief SendCommandsClass::SetAxisSon
/// \param id
///设置son指令
void SendCommandsClass::SetAxisSon(int id)
{
    int axiscurpos= -1;
    int OutBeginBytePos = -1;
    GetAxisBeginBytePos(id,axiscurpos,OutBeginBytePos);
    if((axiscurpos==-1 )|| (OutBeginBytePos==-1))
    {
        return;
    }
//    QMutexLocker locker(&MyShareconfig::GetInstance()->m_mutex);
//    if(!BaseAxisOperate::CheckAxisExcited(m_tempInOP,2,axiscurpos+2))
//        return ;
//    BaseAxisOperate::SetAxisExcite(m_tempOutOP,tempVec,tempOutputData);
//    QThread::msleep(10);//延时时原来50ms

}
///
/// \brief SendCommandsClass::SetAxisSoff
/// \param id
///设置soff指令
void SendCommandsClass::SetAxisSoff(int id)
{
      QVector<int> tempVec;
      int axiscurpos= -1;
      int OutBeginBytePos = -1;
      GetAxisBeginBytePos(id,axiscurpos,OutBeginBytePos);
      if((axiscurpos==-1 )|| (OutBeginBytePos==-1))
      {
          return;
      }
      tempVec.append(OutBeginBytePos);
//       QMutexLocker locker(&MyShareconfig::GetInstance()->m_mutex);
//      BaseAxisOperate::SetAxisExciteOff(m_tempOFFSON,tempVec,tempOutputData);
//      QThread::msleep(10);//延时时间原来50ms
}


