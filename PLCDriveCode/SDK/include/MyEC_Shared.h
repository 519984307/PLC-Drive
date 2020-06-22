#ifndef MYEC_SHARED_H
#define MYEC_SHARED_H
#include <QString>
#include <QMutex>
#define defaultLogModule  "Test"

class InOutPutData
{
private:
    InOutPutData()
    {
        inPutData = NULL;
        outPutData = NULL;
        inPutNum = -1;
        outPutNum = -1;
        WkcNum = -1;
        modeOperationInput = 0x0;
        ifName = NULL;
    }
public:
    static InOutPutData *GetInstance()
    {
        static InOutPutData pInstance;

        return &pInstance;
    }

public:
    //输入部分
    uchar* volatile inPutData;
    uchar* volatile SMTinPutData;
    uchar* volatile CANinPutData;
    int inPutNum;
        uchar modeOperationState;
    int WkcNum;
    bool isRunning;
    QMutex m_inLock;
        //输出部分
        uchar* volatile outPutData;
        uchar* volatile SMToutPutData;
        uchar* volatile CANoutPutData;
        int outPutNum;
        uchar modeOperationInput;
        char *ifName;
        QMutex m_outLock;

        //凸轮采集部分
        int Current_pulse;
        QMutex m_pulseMutex;
        QMutex m_pulse_Read_Mutex;
};
#endif
