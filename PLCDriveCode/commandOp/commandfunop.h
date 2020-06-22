#ifndef COMMANDFUNOP_H
#define COMMANDFUNOP_H

#include <QObject>
#include <myshareconfig.h>
#include "serialportcom.h"
#include <ec_control.h>
#include <logmanager.h>
#include <QTimer>
#include <QQueue>
#include <QPointer>
#include "execommandsfun.h"
#include <QMutex>
#define ORDERTHREADNUM 4
///
/// \brief The CommandFunOp class
///核心调用框架部分，加载启动，指令调用，接口关闭的任务，线程为主要的任务
class CommandFunOp:public QObject
{
    Q_OBJECT
public:
    CommandFunOp();
    ~CommandFunOp();
    bool InitAllParam();
public slots:
    void slotRecPlcCmdData(uint8_t funcmd,cmdstru stru);
private:
    SerialPortCom m_pserialportob;
    Ec_control  m_ec;
    QPointer<QTimer> m_MonTimer;

    //指令线程
    QVector<int> m_threadidvec;
    QMap<int,QMap<QString,FunModuleInterface*>> m_pfunMap;//一个轴对应的 所有的指令 绝对运动 相对运动 等其他指令
    QMap<int,QPair<QThread*,ExeCommandsFun*>> m_pThreadObjMap;
private slots:
    void slotMonitorTimer();
    void slotRunEnd(uint8_t cmd,cmdstru stru,int taskid);
private:
    void GetAllAxisID();
    void SetPluginsMap();
    void CmdThreadRun(QString str,cmdstru stru,uint8_t id);
private:
    int m_exetaskid;
    int m_rectaskid;
    QVector<int> m_runingtaskid;


};

#endif // COMMANDFUNOP_H
