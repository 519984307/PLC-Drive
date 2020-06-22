#include "serialportcom.h"
#include <QDebug>
using namespace SerialDataByteSW;
QQueue<DataFramestru> m_cmdqueue;
QMutex  cmdEditMutex;//队列锁
SerialPortCom::SerialPortCom(QObject *parent): QObject(parent)
{
    m_count=0;
    m_datalen=0;
    m_devicenum = 0;
}

SerialPortCom::~SerialPortCom()
{
    m_plcserialob.close();
}
///
/// \brief SerialPortCom::PLCserialInit
/// \return
///
bool SerialPortCom::PLCserialInit()
{
    m_devicenum = 0;
    //设置串口名
    m_plcserialob.setPortName("myplctest");
    if(m_plcserialob.isOpen())
    {
        m_plcserialob.close();
    }
    //打开串口 读写模式
    if(m_plcserialob.open(QIODevice::ReadWrite))
    {
        qDebug()<<"与plc硬件串口打开成功"<<endl;
        //设置波特率 9600
        m_plcserialob.setBaudRate(QSerialPort::Baud9600);
        //设置数据位 8
        m_plcserialob.setDataBits(QSerialPort::Data8);
        //设置奇偶校验 无
        m_plcserialob.setParity(QSerialPort::NoParity);
        //设置停止位 1
        m_plcserialob.setStopBits(QSerialPort::OneStop);
        //设置流控制
        m_plcserialob.setFlowControl(QSerialPort::NoFlowControl);
        m_plcserialob.setReadBufferSize(4096);
        connect(&m_plcserialob, SIGNAL(readyRead()), this, SLOT(RevSerialDataFromPLC()));
        //        m_devicenum = FindDevice(); //还未确定发什么东西
        //         qDebug()<<"查找设备站数量..."<< m_devicenum<<  endl;
        //          if(m_devicenum <= 0 )
        //          {
        //              qDebug()<<"查找设备的总数为0"<<endl;
        //              return false;
        //          }

        return true;
    }else
    {
        qDebug()<<"串口打开失败...."<<endl;

        return false;
    }
}
///
/// \brief SerialPortCom::RevSerialDataFromPLC
///接受处理plc硬件发过来的数据指令数据
/// 接受数据包的格式，0X55 0XAA 数据长度 数据  0x5A
void SerialPortCom::RevSerialDataFromPLC()
{
    QByteArray arry;
    arry = m_plcserialob .readAll();
    for(int i = 0; i < arry.length(); ++i )
    {
        switch(m_count)
        {
        case 0:
            if( arry.at(i)==0x55)
            {
                m_count++;
            }
            break;
        case 1:
            if( arry.at(i)==(char)(0xAA))
            {
                m_count++;
            }else
            {
                m_count=0;
            }
            break;
        case 2:
            m_datalen = arry.at(i);
            m_RevData[0]= m_datalen;
            m_count++;
            break;
        default:
            if(m_count == m_datalen+4)
            {
                if( arry.at(i)==(char)(0x5A))
                {
                    //数据正确接受
                    qDebug()<<" 数组接受格式："<<m_RevData;
                    Dataprocessingfun(m_RevData,m_datalen);
                }
                m_count=0;
            }else
            {
                if(m_count >=2)
                {
                    m_RevData[m_count-2]= arry.at(i);
                }
                m_count++;
                if(m_count==m_datalen+4)
                {
                    if( arry.at(i)==(char)(0x5A))
                    {
                        //数据正确接受
                        Dataprocessingfun(m_RevData,m_datalen);
                    }
                    m_count=0;
                }
            }
            break;
        }
    }
}
///
/// \brief SerialPortCom::Dataprocessingfun
/// \param data
/// \param len
///处理串口接受的数据进行指令分析
void SerialPortCom::Dataprocessingfun(uint8_t *data, uint8_t len)
{
    if(len < 2)
        return;
    uint8_t tempdata[255] = {0};
    memcpy(tempdata,&data[1],len); //去除头和尾的长度大小
    int alen = 0; //第一位是数据的总长度，不需要传数据
    for(alen = 0; alen < len ; )
    {
        //解析数据头指令
        memcpy((uint8_t*)(&m_boardData.dataheader),&tempdata[alen],2);
        uint8_t  lenvalue = m_boardData.dataheader.bdatalen;
        //解析类型 指令名字 站号ID  数据长度
        uint8_t cmd = m_boardData.dataheader.bcmd;
        //数据指令发到外部执行
        cmdstru  scmdstru;
        if(lenvalue != BaseCommands::GetCmdDatalenValue(cmd))
        {
            continue;
        }
        switch (cmd) {
        case cmdname::None:
            break;
        case cmdname::MOV_ABSPP:
            scmdstru.ppstru.dataheader = m_boardData.dataheader;
            atoi32 pos;
            memcpy(m_boardData.data,&tempdata[alen+2],lenvalue);
            memcpy(pos.a,&tempdata[alen+2],lenvalue);
            scmdstru.ppstru.pos =pos.x;
            break;
        case cmdname::STOPDEC:
            scmdstru.stopstru.dataheader =  m_boardData.dataheader;
            break;
        case cmdname::SETACCDEC:
            scmdstru.setparamstru.dataheader = m_boardData.dataheader;
            atou32  param;
            memcpy(param.a,&tempdata[alen+2],4);
            scmdstru.setparamstru.acc = param.x;
            memcpy(param.a,&tempdata[alen+2+4],4);
            scmdstru.setparamstru.dec = param.x;
            memcpy(param.a,&tempdata[alen+2+4+4],4);
            scmdstru.setparamstru.speed =  param.x;
            break;
        default:
            break;
        }
        emit SignalsendDataToCmd(cmd,scmdstru);
        alen = alen + lenvalue + 2;
    }
}
///
/// \brief SerialPortCom::FindDevice
/// \return
///
uint8_t SerialPortCom::FindDevice()
{
    uint8_t device = 0;
    return  device;

}
///
/// \brief SerialPortCom::SendDatatoPLC
/// \param ID
/// \param type
/// \param funcmd
/// \param datalen
/// \param data
///
void SerialPortCom::SendDatatoPLC(uint8_t ID, uint8_t type, uint8_t funcmd, uint8_t datalen, uint8_t data[32])
{
    DataFramestru datastru;
    memcpy(datastru.data,data,datalen);
    datastru.dataheader.bcmd = funcmd;
    datastru.dataheader.bType = type;
    datastru.dataheader.badrID = ID;
    datastru.dataheader.bdatalen = datalen;
    uint8_t alen = datalen+2; // 把数据自定义长度  + header二个字节
    m_plcserialob.putChar(0x55);
    m_plcserialob.putChar(0xAA);
    m_plcserialob.putChar(alen);//要发送的长度不包含头尾
    m_plcserialob.write((char*)&datastru,datalen+2);
    m_plcserialob.putChar(0x5A);
}
///
/// \brief SerialPortCom::SendStruDataToPLC
/// \param funcmd
/// \param stru
///封装一层方便数据传输
void SerialPortCom::SendStruDataToPLC(uint8_t funcmd, cmdstru stru)
{
    uint8_t id;
    uint8_t type;
    uint8_t datalen;
    uint8_t data[32] = {0};
    switch (funcmd) {
    case cmdname::GETABS:
        atoi32 value;
        value.x = stru.getposstru.curpos;
        memcpy(data,value.a,4);
        id = stru.getposstru.dataheader.badrID;
        type = stru.getposstru.dataheader.bType;
        datalen = stru.getposstru.dataheader.bdatalen;
        funcmd = stru.getposstru.dataheader.bcmd;
        break;
    case cmdname::GETENCODER:
        break;
    default:
        break;
    }
    SendDatatoPLC(id,type,funcmd,datalen,data);
}
