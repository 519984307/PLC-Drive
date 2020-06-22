#ifndef SERIALPORTCOM_H
#define SERIALPORTCOM_H

#include <QObject>
#include<QSerialPort>
#include "serialdataframe.h"
#include <QTimer>

class SerialPortCom:public QObject
{
    Q_OBJECT
public:
    explicit SerialPortCom(QObject *parent = 0);
    ~SerialPortCom();
    bool PLCserialInit();
    uint8_t FindDevice();
    void PLCsendDataFrame(uint8_t datalen,DataFramestru idframe);
    void SendDatatoPLC(uint8_t ID,uint8_t type,uint8_t funcmd,uint8_t datalen,uint8_t data[32]);
    void SendStruDataToPLC(uint8_t funcmd,cmdstru stru);
private:
    QSerialPort m_plcserialob;
    int   m_count;
    uint8_t   m_datalen;//接受总的长度 当前数据包接收的总长度
    uint8_t   m_RevData[255];
    uint8_t m_devicenum;
    QTimer  m_ConnectTimer;//串口通讯是否正常
    DataFramestru m_boardData;
public slots:
    void RevSerialDataFromPLC();
signals:
    void SignalsendDataToCmd(uint8_t funcmd,cmdstru cmdstru);
private:
    void Dataprocessingfun(uint8_t *data,uint8_t len);

};

#endif // SERIALPORTCOM_H
