#include "commandfunop.h"
#include "ConfigfileRW/analysisdatalib.h"
#include <QApplication>
#include <QDebug>

CommandFunOp::CommandFunOp()
{
    m_pfunMap.clear();
    m_pThreadObjMap.clear();
    m_MonTimer = new QTimer(this);
    m_exetaskid = -1;
}
///
/// \brief CommandFunOp::～CommandFunOp
/// 关闭文件的时候调用所有的数据进行关闭
CommandFunOp::~CommandFunOp()
{
    if(InOutPutData::GetInstance()->outPutNum > 0)
    {
        QMutexLocker locker(&InOutPutData::GetInstance()->m_outLock);
        memset(InOutPutData::GetInstance()->outPutData,0,InOutPutData::GetInstance()->outPutNum);
    }
    m_ec.Stop();
    if(m_MonTimer)
    {
        m_MonTimer->stop();
        delete m_MonTimer;
    }
    CoreLog::QLoggerManager::getInstance()->closeLogger();
}
///
/// \brief CommandFunOp::InitAllParam
/// \return 配置文件错误 串口打开失败 执行
/// log文件接口添加打开PLC串口，读本地的文件获得初始的数据，加载指令对象，根据本地数据创建线程对象
/// 启动 ec主站start
///
bool CommandFunOp::InitAllParam()
{
    //logAPi添加
    CoreLog::QLoggerManager::getInstance()->addLogInstance(defaultLogModule, CoreLog::InfoLevel);
    //打开串口
    if(!m_pserialportob.PLCserialInit())
    {
        qDebug()<<"serial open fail...";
        CoreLog::QLog_Error("Test","初始化串口打开失败...请检查串口");
        QString error = "初始化串口打开失败...请检查串口";
        emit signalShowInfo(error);
        //return false;
    }
    //启动相关的配置文件 启动EC
    AnalysisDataLib obj;
    QString path = QCoreApplication::applicationDirPath() +"/config.xml";
    obj.ReadHWConfigFile(path);
    obj.CalcECbytePosandNum(MyShareconfig::GetInstance()->hwconfigstru);
    GetAllAxisID();
    obj.SetS_ONParam();
    obj.LoadPluginsmodule();
    if(obj.MallocECbyteSize())
    {
        if(m_othercmd.Initallaxisparam())
        {
            m_ec.Start();
            connect(m_MonTimer,&QTimer::timeout,this,&CommandFunOp::slotMonitorTimer);
            m_MonTimer->start(200);
        }
    }
    else {
        qDebug()<<"字节数目异常或者网络名字不正确...";
        CoreLog::QLog_Error("Test","please check byte info and netname is null ?");
        QString error = "please check byte info and netname is null ?";
        emit signalShowInfo(error);
        return false;
    }
    //根据硬件配置部分的内容，指令创建和拷贝 线程创建等操作
    return true;
}
///
/// \brief CommandFunOp::slotRecPlcCmdData
/// \param ID
/// \param type
/// \param funcmd
/// \param datalen
/// \param data
///接收plc主站发出指令名字进行处理
void CommandFunOp::slotRecPlcCmdData(uint8_t funcmd,cmdstru stru)
{
    m_rectaskid++;// 每次任务来都会执行任务索引进行记录
    QString cmd ="";
    uint8_t id = -1;
    switch (funcmd) {
    case cmdname::None:
        break;
    case cmdname::MOV_ORG:
        cmd = "MOV_ORG";
        break;
    case cmdname::MOV_ABSPP:
    {
        //存储所有的指令
        id = stru.ppstru.dataheader.badrID;
        cmd = "MOV_ABSPP";
        break;
    }
    case cmdname::MOV_RELPP:
        id = stru.ppstru.dataheader.badrID;
        cmd = "MOV_RELPP";
        break;
    case cmdname::MOV_SPEED:
        break;
    case cmdname::STOPDEC:
    {
        id = stru.stopstru.dataheader.badrID;
        cmd = "STOPDEC";
        // 执行停止动作的指令
        m_exetaskid++;
        StopRunningCmd(id);//改变在运行状态的轴
        break;
    }
    case cmdname::STOP:// 主线程调用，子线程接受
    {
        id = stru.stopstru.dataheader.badrID;
        cmd = "STOP";
        // 执行停止动作的指令
        m_exetaskid++;
        StopRunningCmd(id);//改变在运行状态的轴
        break;
    }
    case cmdname::UNDEFINE:
        break;
    case cmdname::GETABS: // 主线程
    {
        m_exetaskid++;
        QVector<int> id;
        id.append(stru.getposstru.dataheader.badrID);
        QVector<int> values = m_othercmd.GetCurValuefun(id,1);
        if(values.size() == 1)
        {
            cmdstru sendstru;
            sendstru.getposstru.dataheader = stru.getposstru.dataheader;
            sendstru.getposstru.curpos = values[0];
            m_pserialportob.SendStruDataToPLC(GETABS,sendstru);
        }
        break;
    }
    case cmdname::GETENCODER://不需要用线程
        cmd = "GETENCODER";
        m_exetaskid++;
        break;
    case cmdname::SETACCDEC:// 主线程
    {
        id = stru.setparamstru.dataheader.badrID;
        cmd = "SETACCDEC";
        m_exetaskid++;
        QVector<uint> values;
        stru.setparamstru.acc;
        values.append(stru.setparamstru.acc);
        values.append(stru.setparamstru.dec);
        values.append(stru.setparamstru.speed);
        m_othercmd.SetAxisParam(values,id);
        break;
    }
    case cmdname::SETSOFTLINMITPOS:
        break;
    default:
        break;
    }
    if((cmd =="MOV_ABSPP") || (cmd =="MOV_RELPP")|| (cmd == "MOV_ORG") || (cmd == "MOV_SPEED"))
    {
        CmdThreadRun(cmd,stru,id);
    }
  QString taskinfo =   m_tasklog.RecoderTaskCmd(m_exetaskid,funcmd,stru);
 taskinfo= taskinfo +" ;接受到的索引号："+m_rectaskid;
  emit signaltaskInfo(taskinfo);
}
///
/// \brief CommandFunOp::slotMonitorTimer
///监视通迅状态
bool statflag = false;
void CommandFunOp::slotMonitorTimer()
{
    //ec 状态监视
    if(!statflag)
    {
        if(!InOutPutData::GetInstance()->isRunning)
        {
            CoreLog::QLog_Error("Test","ECthercat主站停止了...");
            statflag = true;
            QString error = "ECthercat主站停止了...";
            emit signalShowInfo(error);
        }
        else{
            QVector<int> curposvec =  m_othercmd.GetCurValuefun(m_threadidvec,1);//绝对位置
            QVector<int> errcode  = m_othercmd.GetCurValuefun(m_threadidvec,2);//故障代码
            QVector<int> fault  = m_othercmd.GetCurValuefun(m_threadidvec,3);//故障
            QVector<int> runstate = m_othercmd.GetCurValuefun(m_threadidvec,4);//运行状态
            QVector<int> inp =  m_othercmd.GetCurValuefun(m_threadidvec,5);//到位信号检测
            QVector<int> Excite = m_othercmd.GetCurValuefun(m_threadidvec,6);//son状态检测
            m_monitorvalues.insert(1,curposvec);
            m_monitorvalues.insert(2,errcode);
            m_monitorvalues.insert(3,fault);
            m_monitorvalues.insert(4,runstate);
            m_monitorvalues.insert(5,inp);
            m_monitorvalues.insert(6,Excite);
        }
    }
  //  监视串口状态通迅
}
///
/// \brief CommandFunOp::slotRunEnd
/// \param stru
/// \param taskid
///
void CommandFunOp::slotRunEnd(uint8_t cmd,cmdstru stru, int taskid,QString msg)
{
    int id = -1;
    QString cmdstr;
    switch (cmd) {
    case cmdname::MOV_RELPP:
        id = stru.ppstru.dataheader.badrID;
        cmdstr = "MOV_RELPP";
        break;
    case  cmdname::MOV_ABSPP:
        id = stru.ppstru.dataheader.badrID;
        cmdstr = "MOV_ABSPP";
        break;
    case  cmdname::MOV_ORG:
        cmdstr = "MOV_ORG";
        break;
    case cmdname::SETACCDEC:
        id = stru.setparamstru.dataheader.badrID;
        break;
    default:
        break;
    }
    if(m_pThreadObjMap.contains(id))
    {
        m_pThreadObjMap[id].first->quit();
        m_pThreadObjMap[id].first->wait();
        if(m_runingtaskid.contains(id))
        {
            m_runingtaskid.remove(id);
        }
        if(m_runcmd.contains(id))
        {
            m_runcmd.remove(id);
        }
        MyShareconfig::GetInstance()->m_Runstate[id] = 0;// 用于检查指令运行状态问题
    }
    //运动指令完成的索引号
    QString info =  "["+QString::number(taskid) + "]" + " 结束指令信息:"+cmdstr+":"+msg;
    CoreLog::QLog_Info("Test",info);
    emit signaltaskInfo(msg);
}
///
/// \brief CommandFunOp::GetAllAxisID
///得到所有轴对应的ID
void CommandFunOp::GetAllAxisID()
{
    for(auto it = MyShareconfig::GetInstance()->hwconfigstru.hwconfigInputMap.begin();\
        it !=  MyShareconfig::GetInstance()->hwconfigstru.hwconfigInputMap.end();++it )
    {
        if(it.key() == 0)
        {
            for(auto item = MyShareconfig::GetInstance()->hwconfigstru.hwconfigInputMap[it.key()].begin(); \
                item != MyShareconfig::GetInstance()->hwconfigstru.hwconfigInputMap[it.key()].end();++item)
            {
                if(item.value().hwtype == "1")
                {
                    if(!m_threadidvec.contains(item.value().addr))
                    {
                        m_threadidvec.append(item.value().addr);
                        if(m_threadidvec.size() >= ORDERTHREADNUM)
                        {
                            return;
                        }
                    }
                }
            }
        }
    }
}
///
/// \brief CommandFunOp::SetPluginsMap
///
void CommandFunOp::SetPluginsMap()
{
    for(int i = 0; i < m_threadidvec.size(); ++i)
    {
        QMap<QString,FunModuleInterface*> threadpluginMap;
        threadpluginMap.clear();
        for(auto it = MyShareconfig::GetInstance()->pluginMap.begin(); it !=MyShareconfig::GetInstance()->pluginMap.end(); ++it){
            threadpluginMap.insert(it.key(),it.value()->Copy());
        }
        if(!m_pfunMap.contains(m_threadidvec[i]))
        {
            m_pfunMap.insert(m_threadidvec[i],threadpluginMap);
        }
    }
}
///
/// \brief CommandFunOp::CmdThreadRun
/// \param str
/// \param stru
/// \param id
///
void CommandFunOp::CmdThreadRun(QString str, cmdstru stru, uint8_t id)
{
    if(m_pfunMap.contains(id))
    {
        if(m_pfunMap[id].contains(str))
        {
            FunModuleInterface * cmdob = m_pfunMap[id][str];
            if(!m_pThreadObjMap.contains(id))
            {
                QThread * pthread = new QThread();
                ExeCommandsFun *exerunobj = new ExeCommandsFun();
                QPair<QThread*,ExeCommandsFun*> objpair;
                objpair.first = pthread;
                objpair.second = exerunobj;
                m_pThreadObjMap.insert(id,objpair);
                exerunobj->moveToThread(pthread);
                connect(pthread,&QThread::started,exerunobj,&ExeCommandsFun::SlotCommandRun,Qt::QueuedConnection);
                connect(exerunobj,&ExeCommandsFun::signalComandsEnd,this,&CommandFunOp::slotRunEnd,Qt::QueuedConnection);
                m_exetaskid++;
                exerunobj->SetCommandsParam(MOV_ABSPP,stru,cmdob,m_exetaskid);
                m_runingtaskid.append(m_exetaskid);
                m_runcmd.insert(id,str);
                MyShareconfig::GetInstance()->m_Runstate[id] = 1;
                pthread->start();
            }
            else{
                if(!m_pThreadObjMap[id].first->isRunning())
                {
                    m_exetaskid++;
                    m_pThreadObjMap[id].second->SetCommandsParam(MOV_ABSPP,stru,m_pfunMap[id][str],m_exetaskid);
                    m_runingtaskid.append(m_exetaskid);
                    m_runcmd.insert(id,str);
                    MyShareconfig::GetInstance()->m_Runstate[id] =1;
                    m_pThreadObjMap[id].first->start();
                }
            }
        }
    }
}
///
/// \brief CommandFunOp::StopRunningCmd
/// \param id
///停止当前运行的指令
void CommandFunOp::StopRunningCmd(int id)
{
    //    // 对当前电机执行停止动作
    //    QStringList listcmds;
    //    listcmds<<"MOV_ABSPP"<<"MOV_RELPP"<<"MOV_ORG"<<"MOV_SPEED";
    if(m_pThreadObjMap.contains(id))
    {
        if(m_pThreadObjMap[id].first->isRunning())
        {
            if(m_pfunMap.contains(id) && m_runcmd.contains(id))
            {
                if( m_pfunMap[id].contains(m_runcmd[id]))
                {
                    m_pfunMap[id][m_runcmd[id]]->isTerminate = true;
                }
            }
        }
    }
}


