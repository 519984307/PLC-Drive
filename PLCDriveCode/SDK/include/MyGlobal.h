#ifndef MYGLOBAL_H
#define MYGLOBAL_H
#include <QThread>
#include <QString>
#include <QMutexLocker>
#include <ctime>
#include <time.h>
#include <sys/time.h>
#include <QMutex>
#include <QtCore/qmath.h>
#include <MyEC_Shared.h>
#include <QPair>
#include <QVector>
//#include <MyConfig_Shared>

#define errorCode11 -9999999

namespace CoreGlobal {
    union atoi32
    {
        uchar a[4];
        int x;
    };
    union atou32
    {
        uchar a[4];
        uint x;
    };
    union atoi16
    {
        uchar a[2];
        short x;
    };
    union atou16
    {
        uchar a[2];
        ushort x;
    };
    class BaseCalcFun
    {
    public:
        static void MemCopyOutputData(uchar* TempOutputData)
        {
            QMutexLocker locker(&InOutPutData::GetInstance()->m_outLock);
            memcpy(InOutPutData::GetInstance()->outPutData,TempOutputData,InOutPutData::GetInstance()->outPutNum);
        }
        static void MemCopyOutputData(uchar* TempOutputData,int startindex,int index)
        {
            QMutexLocker locker(&InOutPutData::GetInstance()->m_outLock);
            memcpy(InOutPutData::GetInstance()->outPutData + startindex,TempOutputData + startindex,index);
        }
        static void MemCopyOutputData(uchar* TempOutputData,QVector<QPair<int,int>> temp)
        {
            QMutexLocker locker(&InOutPutData::GetInstance()->m_outLock);
            for(int i = 0;i < temp.size();++i)
            {
                memcpy(InOutPutData::GetInstance()->outPutData+temp[i].first,TempOutputData+temp[i].first,temp[i].second);
            }
        }
        static void MemCopyTempData(uchar* TempOutputData)
        {
            QMutexLocker locker(&InOutPutData::GetInstance()->m_outLock);
            memcpy(TempOutputData,InOutPutData::GetInstance()->outPutData,InOutPutData::GetInstance()->outPutNum);
        }
        ///
        /// \brief mDelay
        /// \param mscount  延时的毫秒计数
        ///该函数延时精确，但是会独占CPU
        static void mDelay(int mscount)
        {
            struct timeval tpStart,tpEnd;
            float timeUse = 0;
            gettimeofday(&tpStart,NULL);
           do
            {
                gettimeofday(&tpEnd,NULL);
                timeUse = 1000 *(tpEnd.tv_sec - tpStart.tv_sec) + 0.001*(tpEnd.tv_usec - tpStart.tv_usec);
            }while(timeUse < mscount);
        }
        ///
        /// \brief GetBitValue
        /// \param iBitNum   bit的位数
        /// \param iByteNum  占的字节数
        /// \param iBytePos  字节的起始位置
        /// \return 该字节的值 0/1
        ///获取Inputdata[]里面的值
        static char GetBitValue(int iBitNum,int iByteNum,int iBytePos)
        {
            QMutexLocker locker(&InOutPutData::GetInstance()->m_inLock);
            char iresult = -1;
            if(iBitNum > iByteNum * 8)
            {
                return iresult;
            }
            if(iByteNum + iBytePos > InOutPutData::GetInstance()->inPutNum)
            {
                return iresult;
            }
            int ibyte = iBitNum /8 + iBytePos;
            int ibit = iBitNum % 8;
            iresult = (InOutPutData::GetInstance()->inPutData[ibyte] >> ibit) & 0x01;
            return iresult;
        }
        ///
        /// \brief GetOutBitValue
        /// \param iBitNum   bit的位数
        /// \param iByteNum  占的字节数
        /// \param iBytePos  字节的起始位置
        /// \return 该字节的值 0/1
        ///获取Outputdata[]里面的值
        static uchar GetOutBitValue(int iBitNum,int iByteNum,int iBytePos)
        {
            uchar *TempOutputData = new uchar[InOutPutData::GetInstance()->outPutNum];
            MemCopyTempData(TempOutputData);
            uchar iresult = 0;
            if(iBitNum > iByteNum * 8)
            {
                return iresult;
            }
            if(iByteNum + iBytePos > InOutPutData::GetInstance()->outPutNum)
            {
                return iresult;
            }
            int ibyte = iBitNum /8 + iBytePos;
            int ibit = iBitNum % 8;
            iresult = (TempOutputData[ibyte] >> ibit) & 0x01;
            delete TempOutputData;
            TempOutputData = NULL;
            return iresult;
        }
    ///
    /// \brief GetOutIOQBitValue
    /// \param strQ0.3 Q0.0
    /// \param iValue 获取对应bit位的值
    /// \return -1输入字符串有误 ,0字节数或者输入字符串不符合格式,1返回成功 iValue返回输出字节值
    ///
//    static int  GetOutIOQBitValue(QString str,uchar &iValue){
//        int iresult = 0;
//        QStringList strList = str.split(".");
//        if(strList.count() > 1)
//        {
//            QString strKey = strList[0];
//            int bitPos = QString(strList[1]).toInt();
//            if(ConfigData::GetInstance()->hwIOMap.contains(strKey))
//            {
//                int iByteNum = ConfigData::GetInstance()->hwIOMap[strKey].iByteNum;
//                int iBytePos = ConfigData::GetInstance()->hwIOMap[strKey].iBytePos;
//                if(iByteNum * 8 > bitPos)
//                {
//                    int ibyte = bitPos /8;
//                    iBytePos += ibyte;
//                    bitPos = bitPos % 8;
//                    if(strKey.contains("Q"))
//                    {
//                        uchar *TempOutputData = new uchar[InOutPutData::GetInstance()->outPutNum];
//                        BaseCalcFun::MemCopyTempData(TempOutputData);
//                        QMutexLocker locker(&InOutPutData::GetInstance()->m_outLock);
//                        if(iBytePos < InOutPutData::GetInstance()->outPutNum)
//                        {
//                            iValue = (TempOutputData[iBytePos] >> bitPos)&1;
//                            iresult = 1;
//                        }
//                    }
//                    else
//                    {
//                        iresult = -1;
//                    }
//                }
//            }
//        }
//        return iresult;
//    }
        ///
        /// \brief GetByteValue
        /// \param str          数值的类型
        /// \param iByteNum     占的字节数
        /// \param iBytePos     字节的起始位置
        /// \return 该字节数以小端方式拼接的十进制值
        ///
        static int64_t GetByteValue(QString str,int iByteNum,int iBytePos)
        {
            QMutexLocker locker(&InOutPutData::GetInstance()->m_inLock);
            int64_t iresult = 0;
            if(iByteNum + iBytePos > InOutPutData::GetInstance()->inPutNum)
            {
                return errorCode11;
            }
            if(str == "I8")
            {
                char c = InOutPutData::GetInstance()->inPutData[iBytePos];
                iresult = c;
            }
            else if(str == "U8")
            {
                uchar c = InOutPutData::GetInstance()->inPutData[iBytePos];
                iresult = c;
            }
            else if(str == "I16")
            {
                atoi16 ai;
                memcpy(ai.a,InOutPutData::GetInstance()->inPutData+iBytePos,iByteNum);
                iresult = ai.x;
            }
            else if(str == "U16")
            {
                atou16 ai;
                memcpy(ai.a,InOutPutData::GetInstance()->inPutData+iBytePos,iByteNum);
                iresult = ai.x;
            }
            else if(str == "I32")
            {
                atoi32 ai;
                memcpy(ai.a,InOutPutData::GetInstance()->inPutData+iBytePos,iByteNum);
                iresult = ai.x;
            }
            else if(str == "U32")
            {
                atou32 ai;
                memcpy(ai.a,InOutPutData::GetInstance()->inPutData+iBytePos,iByteNum);
                iresult = ai.x;
            }
            else
            {
                iresult = errorCode11;
            }
            return iresult;
        }
        ///
        /// \brief SetByteValue
        /// \param str          数值的类型
        /// \param iByteNum     占的字节数
        /// \param iBytePos     字节的起始位置
        /// \param ivalue       设置OutputData[]的数值
        /// 十进制值以小端的方式分成uchar的方式填充到OutputData[]
        static void SetByteValue(QString str,int iByteNum,int iBytePos,int64_t ivalue)
        {
            uchar *TempOutputData = new uchar[InOutPutData::GetInstance()->outPutNum];
            MemCopyTempData(TempOutputData);
            if(str == "I8")
            {
                char c = ivalue;
                TempOutputData[iBytePos] = c;
            }
            else if(str == "U8")
            {
                uchar c = ivalue;
                TempOutputData[iBytePos] = c;
            }
            else if(str == "I16")
            {
                atoi16 ai;
                ai.x = ivalue;
                memcpy(TempOutputData+iBytePos,ai.a,iByteNum);
            }
            else if(str == "U16")
            {
                atou16 ai;
                ai.x = ivalue;
                memcpy(TempOutputData+iBytePos,ai.a,iByteNum);
            }
            else if(str == "I32")
            {
                atoi32 ai;
                ai.x = ivalue;
                memcpy(TempOutputData+iBytePos,ai.a,iByteNum);
            }
            else if(str == "U32")
            {
                atou32 ai;
                ai.x = ivalue;
                memcpy(TempOutputData+iBytePos,ai.a,iByteNum);
            }
            MemCopyOutputData(TempOutputData,iBytePos,iByteNum);
            delete TempOutputData;
            TempOutputData = NULL;
        }
        static void  SetByteValue(QString str,int iByteNum,int iBytePos,int64_t ivalue,uchar* TempOutputData)
        {
            if(str == "I8")
            {
                char c = ivalue;
                TempOutputData[iBytePos] = c;
            }
            else if(str == "U8")
            {
                uchar c = ivalue;
                TempOutputData[iBytePos] = c;
            }
            else if(str == "I16")
            {
                atoi16 ai;
                ai.x = ivalue;
                memcpy(TempOutputData+iBytePos,ai.a,iByteNum);
            }
            else if(str == "U16")
            {
                atou16 ai;
                ai.x = ivalue;
                memcpy(TempOutputData+iBytePos,ai.a,iByteNum);
            }
            else if(str == "I32")
            {
                atoi32 ai;
                ai.x = ivalue;
                memcpy(TempOutputData+iBytePos,ai.a,iByteNum);
            }
            else if(str == "U32")
            {
                atou32 ai;
                ai.x = ivalue;
                memcpy(TempOutputData+iBytePos,ai.a,iByteNum);
            }
        }
        ///
        /// \brief SetBitValue
        /// \param iBitNum      字节中占的bit位
        /// \param iBytePos     字节的起始位置
        /// \param iValue       设置bit位的值  0/1
        ///
        static void SetBitValue(int iBitNum,int iBytePos,char iValue)
        {
            QMutexLocker locker(&InOutPutData::GetInstance()->m_outLock);
            int ibyte = iBitNum /8 + iBytePos;
            int ibit = iBitNum % 8;
            if(iValue == 1)
            {
                iValue = iValue << ibit;
                InOutPutData::GetInstance()->outPutData[ibyte] = InOutPutData::GetInstance()->outPutData[ibyte] | iValue;
            }
            else
            {
                iValue = ~(1 << ibit);
                InOutPutData::GetInstance()->outPutData[ibyte] = InOutPutData::GetInstance()->outPutData[ibyte] & iValue;
            }
        }
        static void SetBitValue(int iBitNum,int iBytePos,char iValue,uchar* TempOutputData)
        {
            int ibyte = iBitNum /8 + iBytePos;
            int ibit = iBitNum % 8;
            if(iValue == 1)
            {
                iValue = iValue << ibit;
                TempOutputData[ibyte] =  TempOutputData[ibyte] | iValue;
            }
            else
            {
                iValue = ~(1 << ibit);
                TempOutputData[ibyte] =  TempOutputData[ibyte] & iValue;
            }
        }
        ///
        /// \brief AnalysisIOString
        /// \param str          传入的类型（E0.1/Q0.1)
        /// \param iValue       设置值(O)或者获取值(I)
        /// \return -1=失败  0=Out成功  1=获取成功
        ///该函数设置、或者IO值
//        static int AnalysisIOString(QString str,uchar &iValue)
//        {
//            int iresult = -1;
//            QStringList strList = str.split(".");
//            if(strList.count() > 1)
//            {
//                QString strKey = strList[0];
//                int bitPos = QString(strList[1]).toInt();
//                if(ConfigData::GetInstance()->hwIOMap.contains(strKey))
//                {
//                    int iByteNum = ConfigData::GetInstance()->hwIOMap[strKey].iByteNum;
//                    int iBytePos = ConfigData::GetInstance()->hwIOMap[strKey].iBytePos;
//                    if(iByteNum * 8 > bitPos)
//                    {
//                        int ibyte = bitPos /8;
//                        iBytePos += ibyte;
//                        bitPos = bitPos % 8;
//                        if(strKey.contains("Q"))
//                        {
//                            QMutexLocker locker(&InOutPutData::GetInstance()->m_outLock);
//                            if(iBytePos < InOutPutData::GetInstance()->outPutNum)
//                            {
//                                iresult = 0;
//                                if(iValue == 1)
//                                {
//                                    iValue = iValue << bitPos;
//                                    InOutPutData::GetInstance()->outPutData[iBytePos] = InOutPutData::GetInstance()->outPutData[iBytePos] | iValue;
//                                }
//                                else
//                                {
//                                    iValue = ~(1 << bitPos);
//                                    InOutPutData::GetInstance()->outPutData[iBytePos] = InOutPutData::GetInstance()->outPutData[iBytePos] & iValue;
//                                }

//                            }
//                        }
//                        else if(strKey.contains("E"))
//                        {
//                            QMutexLocker locker(&InOutPutData::GetInstance()->m_inLock);
//                            if(iBytePos < InOutPutData::GetInstance()->inPutNum)
//                            {
//                                iresult = 1;
//                                iValue = (InOutPutData::GetInstance()->inPutData[iBytePos] >> bitPos)&1;
//                            }
//                        }
//                    }
//                }
//            }
//            return iresult;
//        }
//        static int AnalysisIOStringS(QMap<QString,uchar> &ValueMap)
//        {
//            int iresult = -1;
//            for(auto itor = ValueMap.begin() ;itor != ValueMap.end();++itor)
//            {
//                QStringList strList = itor.key().split(".");
//                if(strList.count() > 1)
//                {
//                    QString strKey = strList[0];
//                    int bitPos = QString(strList[1]).toInt();
//                    if(ConfigData::GetInstance()->hwIOMap.contains(strKey))
//                    {
//                        int iByteNum = ConfigData::GetInstance()->hwIOMap[strKey].iByteNum;
//                        int iBytePos = ConfigData::GetInstance()->hwIOMap[strKey].iBytePos;
//                        if(iByteNum * 8 > bitPos)
//                        {
//                            int ibyte = bitPos /8;
//                            iBytePos += ibyte;
//                            bitPos = bitPos % 8;
//                            if(strKey.contains("Q"))
//                            {
//                                QMutexLocker locker(&InOutPutData::GetInstance()->m_outLock);
//                                if(iBytePos < InOutPutData::GetInstance()->outPutNum)
//                                {
//                                    iresult = 0;
//                                    if(itor.value() == 1)
//                                    {
//                                        uchar iValue = 1;
//                                        iValue = iValue << bitPos;
//                                        InOutPutData::GetInstance()->outPutData[iBytePos] = InOutPutData::GetInstance()->outPutData[iBytePos] | iValue;
//                                    }
//                                    else
//                                    {
//                                        uchar iValue = 0;
//                                        iValue = ~(1 << bitPos);
//                                        InOutPutData::GetInstance()->outPutData[iBytePos] = InOutPutData::GetInstance()->outPutData[iBytePos] & iValue;
//                                    }

//                                }
//                            }
//                            else if(strKey.contains("E"))
//                            {
//                                QMutexLocker locker(&InOutPutData::GetInstance()->m_inLock);
//                                if(iBytePos < InOutPutData::GetInstance()->inPutNum)
//                                {
//                                    iresult = 1;
//                                    itor.value() = (InOutPutData::GetInstance()->inPutData[iBytePos] >> bitPos)&1;
//                                }
//                            }
//                        }
//                    }
//                }
//            }
//            return iresult;
//        }
	 ///
        /// \brief Analysis_L
        /// \param str   传入的类型（Lin0.1 /Lout 0.0）
        /// \param Value 设置out的值 还是 获取in的值
        /// \return -1 失败 0 = out成功 1= 获取成功
        ///

//        static int Analysis_L(QString str,uchar &Value)
//        {
//            int result = -1;
//            QStringList strlist = str.split(".");
//            if(strlist.count() > 1)
//            {
//                QString Strkey = strlist[0];
//                int bitpos = QString(strlist[1]).toInt();
//                QString key = Strkey.mid(2);
//                if(ConfigData::GetInstance()->m_LStateMap.contains(key) && bitpos < 16)
//                {
//                    if(Strkey.contains("LQ"))
//                    {
//                        QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
//                        result = 0;
//                        if(Value == 1)
//                        {
//                            ushort iValue = Value << bitpos;
//                            ConfigData::GetInstance()->m_LStateMap[key] = ConfigData::GetInstance()->m_LStateMap[key] | iValue;
//                        }
//                        else
//                        {
//                            ushort iValue = ~(1 << bitpos);
//                            ConfigData::GetInstance()->m_LStateMap[key] = ConfigData::GetInstance()->m_LStateMap[key] & iValue;
//                        }

//                    }
//                    else if(Strkey.contains("LE"))
//                    {
//                        QMutexLocker locker(&ConfigData::GetInstance()->m_mutex);
//                        result = 1;
//                        Value = (ConfigData::GetInstance()->m_LStateMap[key] >> bitpos) & 1;

//                    }
//                }
//            }
//           return result;
//        }
   };
    class BaseAxisOperate
    {
    public:
        ///
        /// \brief ChangeAxisModel
        /// \param strType  模式的数值类型
        /// \param iByteNum 占的字节数
        /// \param iBytePos 字节占的起始位置
        /// \param ivalue   更改模式对应的值
        /// 更改轴的模式函数
        static void ChangeAxisModel(QString strType,uchar iByteNum,int iBytePos,int ivalue)
        {
            BaseCalcFun::SetByteValue(strType,iByteNum,iBytePos,ivalue);
            QThread::msleep(20);
        }
        static void ChangeAxisModel(QString strType,uchar iByteNum,int iBytePos,int ivalue,uchar* TempOutputData)
        {
            BaseCalcFun::SetByteValue(strType,iByteNum,iBytePos,ivalue,TempOutputData);
        }
        ///
        /// \brief SetAxisSpeed
        /// \param strType  速度的数值类型
        /// \param iByteNum 占的字节数
        /// \param iBytePos 字节占的起始位置
        /// \param ivalue   更改速度对应的值
        ///设置轴的速度
        static void SetAxisSpeed(QString strType,uchar iByteNum,int iBytePos,uint ivalue)
        {
            BaseCalcFun::SetByteValue(strType,iByteNum,iBytePos,ivalue);
            QThread::msleep(20);
        }
        static void SetAxisSpeed(QString strType,uchar iByteNum,int iBytePos,uint ivalue,uchar* TempOutputData)
        {
            BaseCalcFun::SetByteValue(strType,iByteNum,iBytePos,ivalue,TempOutputData);
        }
        ///
        /// \brief SetAxisPos
        /// \param strType  位置的数值类型
        /// \param iByteNum 占的字节数
        /// \param iBytePos 字节占的起始位置
        /// \param ivalue   更改位置对应的值
        ///设置轴的位置
        static void SetAxisPos(QString strType,uchar iByteNum,int iBytePos,int ivalue)
        {
            BaseCalcFun::SetByteValue(strType,iByteNum,iBytePos,ivalue);
            QThread::msleep(20);
        }
        static void SetAxisPos(QString strType,uchar iByteNum,int iBytePos,int ivalue,uchar* TempOutputData)
        {
            BaseCalcFun::SetByteValue(strType,iByteNum,iBytePos,ivalue,TempOutputData);
        }
        ///
        /// \brief SetAxisDCC
        /// \param strType  减速的数值类型
        /// \param iByteNum 占的字节数
        /// \param iBytePos 字节占的起始位置
        /// \param ivalue   更改减速对应的值
        ///设置轴的减速
        static void SetAxisDCC(QString strType,uchar iByteNum,int iBytePos,int ivalue)
        {
            BaseCalcFun::SetByteValue(strType,iByteNum,iBytePos,ivalue);
            QThread::msleep(20);
        }
        static void SetAxisDCC(QString strType,uchar iByteNum,int iBytePos,int ivalue,uchar* TempOutputData)
        {
            BaseCalcFun::SetByteValue(strType,iByteNum,iBytePos,ivalue,TempOutputData);
        }
        ///
        /// \brief SetAxisACC
        /// \param strType  加速的数值类型
        /// \param iByteNum 占的字节数
        /// \param iBytePos 字节占的起始位置
        /// \param ivalue   更改加速对应的值
        ///设置轴的加速
        static void SetAxisACC(QString strType,uchar iByteNum,int iBytePos,int ivalue)
        {
            BaseCalcFun::SetByteValue(strType,iByteNum,iBytePos,ivalue);
            QThread::msleep(20);
        }
        static void SetAxisACC(QString strType,uchar iByteNum,int iBytePos,int ivalue,uchar* TempOutputData)
        {
            BaseCalcFun::SetByteValue(strType,iByteNum,iBytePos,ivalue,TempOutputData);
        }
        ///
        /// \brief SetAxisRun
        /// \param ibitNum      控制字中对应运行的bit位置
        /// \param iBytePos     控制字占的起始位置
        ///设置轴启动运行，该函数设置控制字的bit
        static void SetAxisRun(uchar ibitNum,int iBytePos)
        {
            BaseCalcFun::SetBitValue(ibitNum,iBytePos,1);
            QThread::msleep(20);
            BaseCalcFun::SetBitValue(ibitNum,iBytePos,0);
            QThread::msleep(20);
        }
        static void SetAxisRun(uchar ibitNum,int iBytePos,uchar* TempOutputData)
        {
            BaseCalcFun::SetBitValue(ibitNum,iBytePos,1,TempOutputData);
        }
        static void ReAxisRunBit(uchar ibitNum,int iBytePos,uchar* TempOutputData)
        {
            BaseCalcFun::SetBitValue(ibitNum,iBytePos,0,TempOutputData);
        }
        ///
        /// \brief SetAxisReset
        /// \param ibitNum      控制字中对应复位的bit位置
        /// \param iBytePos     控制字占的起始位置
        ///设置轴复位，该函数设置控制字的bit
        static void SetAxisReset(uchar ibitNum,int iBytePos)
        {
            BaseCalcFun::SetBitValue(ibitNum,iBytePos,1);
            QThread::msleep(20);
            BaseCalcFun::SetBitValue(ibitNum,iBytePos,0);
            QThread::msleep(20);
        }
        static void SetAxisReset(uchar ibitNum,int iBytePos,uchar* TempOutputData)
        {
            BaseCalcFun::SetBitValue(ibitNum,iBytePos,1,TempOutputData);
        }
        static void ReAxisResetBit(uchar ibitNum,int iBytePos,uchar* TempOutputData)
        {
            BaseCalcFun::SetBitValue(ibitNum,iBytePos,0,TempOutputData);
        }
        ///
        /// \brief SetAxisStop
        /// \param ibitNum      控制字中对应停止的bit位置
        /// \param iBytePos     控制字占的起始位置
        ///设置轴停止，该函数设置控制字的bit
        static void SetAxisStop(uchar ibitNum,int iBytePos)
        {
            BaseCalcFun::SetBitValue(ibitNum,iBytePos,0);
            QThread::msleep(20);
            BaseCalcFun::SetBitValue(ibitNum,iBytePos,1);
            QThread::msleep(20);
        }
        static void SetAxisStop(uchar ibitNum,int iBytePos,uchar* TempOutputData)
        {
            BaseCalcFun::SetBitValue(ibitNum,iBytePos,0,TempOutputData);
        }
        static void ReAxisStopBit(uchar ibitNum,int iBytePos,uchar* TempOutputData)
        {
            BaseCalcFun::SetBitValue(ibitNum,iBytePos,1,TempOutputData);
        }
        ///
        /// \brief SetAxisExcite
        /// \param iByteNum     控制字占的字节数
        /// \param iBytePos     控制字占的起始位置
        ///设置轴励磁，该函数设置控制字中，bit1=1,bit2=1,bit0=1,bit3=1
        static void SetAxisExcite(QVector<QPair<int,int>> temp,int iBytePos)
        {
            for(auto it = temp.begin();it != temp.end();++it)
            {
                BaseCalcFun::SetBitValue(it->first,iBytePos,it->second);
                QThread::msleep(20);
            }
        }
        static void SetAxisExcite(QVector<QPair<int,int>> temp,QVector<int> bytePosVec,uchar* TempOutputData)
        {
            BaseCalcFun::MemCopyTempData(TempOutputData);
            QVector<QPair<int,int>> tempBytePosNum;
            for(auto it = temp.begin();it != temp.end();++it)
            {
                for(int i = 0 ; i < bytePosVec.count(); ++i)
                {
                    BaseCalcFun::SetBitValue(it->first,bytePosVec[i],it->second,TempOutputData);
                    QPair<int,int> pair = QPair<int,int>(bytePosVec[i],1);
                    tempBytePosNum.append(pair);
                }
                BaseCalcFun::MemCopyOutputData(TempOutputData,tempBytePosNum);
                QThread::msleep(20);
            }
        }
        ///
        /// \brief SetAxisExcite
        /// \param iByteNum     控制字占的字节数
        /// \param iBytePos     控制字占的起始位置
        ///关闭轴励磁，该函数设置控制字中，bit1=1,bit2=1,bit0=1,bit3=1
        static void SetAxisExciteOff(QVector<QPair<int,int>> temp,int iBytePos)
        {
            for(auto it = temp.begin();it != temp.end();++it)
            {
                BaseCalcFun::SetBitValue(it->first,iBytePos,it->second);
                QThread::msleep(20);
            }
        }
        static void SetAxisExciteOff(QVector<QPair<int,int>> temp,QVector<int> bytePosVec,uchar* TempOutputData)
        {
            BaseCalcFun::MemCopyTempData(TempOutputData);
            QVector<QPair<int,int>> tempBytePosNum;
            for(auto it = temp.begin();it != temp.end();++it)
            {
                for(int i = 0 ; i < bytePosVec.count(); ++i)
                {
                    BaseCalcFun::SetBitValue(it->first,bytePosVec[i],it->second,TempOutputData);
                    QPair<int,int> pair = QPair<int,int>(bytePosVec[i],1);
                    tempBytePosNum.append(pair);
                }
                BaseCalcFun::MemCopyOutputData(TempOutputData,tempBytePosNum);
                QThread::msleep(20);
            }
        }
//        static void SetAllAxisExciteOff(QString strWorld)
//        {
//            QVector<QPair<int,int>> temp;
//            foreach (auto it, ConfigData::GetInstance()->xmlMap.swConfigXml.swXml_SER_Map.son_Stru.OutPutVec) {
//                int second = 0;
//                if(0 == it.value)
//                {
//                    second = 1;
//                }

//                QPair<int,int> pair = QPair<int,int>(it.bit,second);
//                temp.append(pair);
//            }
//            QVector<int> tempPos;
//            for(auto item = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap.begin();
//                item != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap.end();
//                ++item)
//            {
//                int iBytePos = item.value().iBytePos;
//                QString strtype = item.value().modelType;
//                if(ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap.contains(strtype))
//                {
//                    auto hwModel = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[strtype]["OUTPUT"];
//                    for(auto ior = hwModel.byteInfoVec.begin();ior != hwModel.byteInfoVec.end();++ior)
//                    {
//                        int ibyteNum = ior->byteNum;
//                        if(ior->key.compare(strWorld) == 0)
//                        {
//                            tempPos.append(iBytePos);
//                            break;
//                        }
//                        iBytePos += ibyteNum;
//                    }
//                }
//            }
//            if(InOutPutData::GetInstance()->outPutNum < 0)
//                return;
//            uchar* TempOutputData = new uchar[InOutPutData::GetInstance()->outPutNum];
//            SetAxisExciteOff(temp,tempPos,TempOutputData);
//        }
//        static void SetAllAxisReset(QString strWorld)
//        {
//            if(InOutPutData::GetInstance()->outPutNum < 0)
//                return;
//            QVector<QPair<int,int>> temp;
//            for(auto item = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap.begin();
//                item != ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlOutputMap.hwSerOutputMap.end();
//                ++item)
//            {
//                int iBytePos = item.value().iBytePos;
//                QString strtype = item.value().modelType;
//                if(ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap.contains(strtype))
//                {
//                    auto hwModel = ConfigData::GetInstance()->xmlMap.hwConfigXml.hwConfigXmlModelMap.hwModelMap[strtype]["OUTPUT"];
//                    for(auto ior = hwModel.byteInfoVec.begin();ior != hwModel.byteInfoVec.end();++ior)
//                    {
//                        int ibyteNum = ior->byteNum;
//                        if(ior->key.compare("Contrlworld") == 0)
//                        {
//                            QMap<QString,BITINFO_STRU> cwbitMap;
//                            cwbitMap = ior->bitInfoMap;
//                            auto ir = cwbitMap.find(strWorld);
//                            QPair<int,int> pair = QPair<int,int>(ir.value().bit,iBytePos);
//                            temp.append(pair);
//                            break;
//                        }
//                        iBytePos += ibyteNum;
//                    }
//                }
//            }
//            uchar* TempOutputData = new uchar[InOutPutData::GetInstance()->outPutNum];
//            BaseCalcFun::MemCopyTempData(TempOutputData);
//            for(auto it = temp.begin();it != temp.end();++it)
//            {
//                SetAxisReset(it->first,it->second,TempOutputData);
//            }
//            BaseCalcFun::MemCopyOutputData(TempOutputData);
//            QThread::msleep(150);
//            for(auto it = temp.begin();it != temp.end();++it)
//            {
//                ReAxisResetBit(it->first,it->second,TempOutputData);
//            }
//            BaseCalcFun::MemCopyOutputData(TempOutputData);
//            QThread::msleep(20);
//            delete TempOutputData;
//            TempOutputData = NULL;
//        }

        ///
        /// \brief CheckAxisExcited
        /// \param iByteNum     状态字占的字节数
        /// \param iBytePos     状态字占的起始位置
        /// \return
        ///校验当前轴是否已经励磁，该函数检查状态字中，bit0=1,bit1=1,bit2=1,bit6=0
        static bool CheckAxisExcited( QVector<QPair<int,int>> temp,uchar iByteNum,int iBytePos)
        {
            bool isok = true;
            for(auto it = temp.begin();it != temp.end();++it)
            {
                if(BaseCalcFun::GetBitValue(it->first,iByteNum,iBytePos) != it->second)
                    isok = false;
            }
            return isok;
        }
        ///
        /// \brief GetAxisCurPos
        /// \param strType     当前位置的数值类型
        /// \param iByteNum    占字节数
        /// \param iBytePos    字节的起始位置
        /// \return 轴当前位置
        ///
        static int GetAxisCurPos(QString strType,uchar iByteNum,int iBytePos)
        {
            return BaseCalcFun::GetByteValue(strType,iByteNum,iBytePos);
        }
        ///
        /// \brief GetAxisErrorCode
        /// \param strType     故障代码的数值类型
        /// \param iByteNum    占字节数
        /// \param iBytePos    字节的起始位置
        /// \return 轴的故障代码
        ///
        static int GetAxisErrorCode(QString strType,uchar iByteNum,int iBytePos)
        {
            return BaseCalcFun::GetByteValue(strType,iByteNum,iBytePos);
        }
        ///
        /// \brief CheckAxisFault
        /// \param ibitNum      故障信号的bit位数
        /// \param iByteNum     状态字占的字节数
        /// \param iBytePos     字节的起始位置
        /// \return 校验的结果 true=有故障信号;false=无故障信号
        ///状态字中故障bit位的检查，通常在整个轴运行过程中都需要校验
        static bool CheckAxisFault(uchar ibitNum,uchar iByteNum,int iBytePos)
        {
            uchar uc = BaseCalcFun::GetBitValue(ibitNum,iByteNum,iBytePos);
            if(uc != 0)
                return true;
            return false;
        }
        ///
        /// \brief CheckAxisINP
        /// \param ibitNum      到位信号的bit位数
        /// \param iByteNum     状态字占的字节数
        /// \param iBytePos     字节的起始位置
        /// \return true=有到位信号;false=无到位信号
        ///轴运行的到位信号
        static bool CheckAxisINP(uchar ibitNum,uchar iByteNum,int iBytePos)
        {
            uchar uc = BaseCalcFun::GetBitValue(ibitNum,iByteNum,iBytePos);
            if(uc != 0)
                return true;
            return false;
        }
    };
}
#endif
