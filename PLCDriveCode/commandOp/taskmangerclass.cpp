#include "taskmangerclass.h"
#include <logmanager.h>

TaskMangerClass::TaskMangerClass()
{

}
///
/// \brief TaskMangerClass::AddHandleTask
/// \param stru
///
void TaskMangerClass::AddHandleTask(TaskMangerClass::Task_queueStru stru)
{

}
///
/// \brief TaskMangerClass::GetCurHandleTask
/// \return
///
TaskMangerClass::Task_queueStru TaskMangerClass::GetCurHandleTask()
{

}

//void TaskMangerClass::AddHandleTask(Task_queueStru stru)
//{
//    Task_queueStru  curstru= m_taskqueue.last();
//    int curid =  curstru.taskid;
//    stru.taskid = curid+1;
//    m_taskqueue.push_back(stru);
//}

//TaskMangerClass::Task_queueStru TaskMangerClass::GetCurHandleTask()
//{
//    Task_queueStru first ;
//    if(!m_taskqueue.isEmpty())
//    {
//        first =   m_taskqueue.dequeue();
//    }
//    return first;
//}

QString TaskMangerClass::RecoderTaskCmd(int index,uint8_t funcmd, cmdstru stru)
{
    QString name;
    QString info;
    switch (funcmd) {
    case cmdname::MOV_ORG:
        name = "MOV_ORG";
        info = QString("设置轴id:%1回原点，method:%2,search swspeed:%3,search zerospeed:%4,ACC/DEC:%5;").\
                arg(stru.orgstru.dataheader.badrID).arg(stru.orgstru.method).arg(stru.orgstru.searchswspeed).\
                arg(stru.orgstru.searchzerospeed).arg(stru.orgstru.acc);
        break;
    case cmdname::MOV_ABSPP:
    {
        name = "MOV_ABSPP";
        info = QString("设置轴id:%1的绝对位置为:%2;").arg(stru.ppstru.dataheader.badrID).arg(stru.ppstru.pos);
        break;
    }
    case cmdname::MOV_RELPP:
    {
        name = "MOV_RELPP";
        info = QString("设置轴id:%1的相对位置为:%2;").arg(stru.ppstru.dataheader.badrID).arg(stru.ppstru.pos);
        break;
    }
    case  cmdname::STOP:
    case  cmdname::STOPDEC:
    {     name = "STOP";
        info = QString("设置轴id ：%1停止运行;").arg(stru.ppstru.dataheader.badrID);
        break;
    }
    case cmdname::SETACCDEC:
    {
        name = "SETACCDEC";
        info = QString("设置轴id :%1的acc:%2,dec:%3,speed:%4;").arg(stru.setparamstru.dataheader.badrID).arg(stru.setparamstru.acc)\
                .arg(stru.setparamstru.dec).arg(stru.setparamstru.speed);
        break;
    }
    case cmdname::GETABS:
    {
        name = "GETABS";
        info = QString("获取轴id:%1的绝对位置,发送位置:pos%2;").arg(stru.getposstru.dataheader.badrID).arg(stru.getposstru.curpos);
        break;
    }
    case cmdname::SETSON:
    {
        name = "SETSON";
        info = QString("设置轴id ：%,励磁动作;").arg(stru.sonstru.dataheader.badrID);
        break;
    }
    case cmdname::SETOFF:
    {
        name = "SETOFF";
        info = QString("设置轴id ：%,松励磁动作;").arg(stru.soffstru.dataheader.badrID);
        break;
    }
    case cmdname::SETRESET:
    {
        name = "SETRESET";
        info = QString("设置轴id ：%,故障清除动作;").arg(stru.resetstru.dataheader.badrID);
        break;
    }
    default:
        break;
    }
    QString msg;
    if(info != "")
    {
        msg=  "["+QString::number(index) + "]" + " 执行指令:"+name+":"+info;
        CoreLog::QLog_Info("Test",msg);
    }
    return msg;
}
