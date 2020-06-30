#ifndef SERIALDATAFRAME_H
#define SERIALDATAFRAME_H
#include <QObject>
#include <QQueue>
#include<QMutex>
namespace SerialDataByteSW {
union atoi32
{
    uint8_t a[4];
    int x;
};
union atou32
{
    uint8_t a[4];
    uint x;
};
union atoi16
{
    uint8_t a[2];
    short x;
};
union atou16
{
    uint8_t a[2];
    ushort x;
};
union atoi8
{
    uint8_t a;
    int8_t x;
};
}
#pragma pack(1)
typedef union
{
    struct {
        uint16_t
        bdatalen:5, // 0-4 取值范围0-31 代表数据长度
            bcmd:5, // 5-9 取值范围0-31   代表命令功能
            badrID:5,//10-14 取值范围0-31  代表电机或者IO的站点号 功能ID
            bType:1; // 15 0代表IO 1代表电机
    };
    uint16_t dataheader;
}DataFramehead;//接受数据帧头定义
// 数据长度
//单个站点数据
typedef struct {
    DataFramehead  dataheader;
    uint8_t data[32]; //最大32个字节 根据取值范围是32个
}DataFramestru;
//总的数据包格式
typedef struct {
    uint8_t datalen;
    DataFramestru alldatastru[255];//最大数据长度255
}BoardFramestru;
#pragma pack()
extern QQueue<DataFramestru> m_cmdqueue;
extern QMutex  cmdEditMutex;//队列锁
//0X55 0XAA 数据总长度 数据 0X5A //串口包的格式 数据总长度包含只是数据内容
typedef enum
{
    PLC_IO_ID        = 0,//  IO
    PLC_AXIS_ID        =1,  // axis name
} PLC_TYPE;
//指令功能类型枚举
typedef enum cmdname{
    None  = 0,
    MOV_ORG  = 1,
    MOV_ABSPP = 2,
    MOV_RELPP = 3,
    MOV_SPEED = 4,
    STOPDEC = 5,
    STOP = 6,
    RUNSTATE = 7,
    GETABS = 8,
    GETENCODER = 9,
    GETERRORCODE = 10,
    SETACCDEC = 11,
    SETSOFTLINMITPOS=12,
}cmdname;
//MOV_ABSPP || //MOV_RELPP 绝对运动或者相对运动信息结构体
typedef struct {
    DataFramehead  dataheader;
    int pos;
}ppcmdstru;
//STOPDEC 减速停止指令
typedef struct {
    DataFramehead  dataheader;
}stopdeccmdstru;
//SETACC DECSPEED
typedef struct {
    DataFramehead  dataheader;
    uint acc;
    uint dec;
    uint speed;
}setadscmdstru;
//GETABS
typedef struct {
    DataFramehead  dataheader;
    int curpos;
}getabsposcmdstru;
//runstate
typedef struct {
    DataFramehead  dataheader;
   char state;
}getrunsatecmdstru;
//故障代码
typedef struct {
    DataFramehead  dataheader;
   uint16_t errcode;
}geterrorcodecmdstru;
///各个命令结构体总
typedef struct{
    ppcmdstru ppstru;
    stopdeccmdstru stopstru;
    setadscmdstru setparamstru;
    getabsposcmdstru getposstru;
    getrunsatecmdstru  runstatestru;
    geterrorcodecmdstru errorcodestru;
}cmdstru;
//基本数据
class BaseCommands
{
public:
    ///
    /// \brief GetCmdDatalenValue
    /// \param cmd
    /// \return
    ///每种指令数据长度的定义
    static uint8_t GetCmdDatalenValue(uint8_t cmd){
        int lenvalue = 0;
        switch (cmd) {
        case STOPDEC:
            lenvalue = 0;
            break;
        case MOV_RELPP:
        case MOV_ABSPP:
        case GETABS:
            lenvalue = 4;
            break;
        case SETACCDEC:
            lenvalue = 12;
            break;
        default:
            break;
        }
        return lenvalue;
    }
};


#endif // SERIALDATAFRAME_H
