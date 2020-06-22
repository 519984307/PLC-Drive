#include "taskmangerclass.h"

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

void TaskMangerClass::RecoderTaskCmd(uint8_t funcmd, cmdstru stru)
{

}
