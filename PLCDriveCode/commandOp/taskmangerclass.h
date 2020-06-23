#ifndef TASKMANGERCLASS_H
#define TASKMANGERCLASS_H
#include <serialdataframe.h>
#include <QObject>
#include <QString>

class TaskMangerClass:public QObject
{
    Q_OBJECT
    typedef struct  _Task_queueStru{
        int taskid = 0;
        DataFramehead headerdata;
        QString  msg;
    }Task_queueStru;
public:
    TaskMangerClass();
    void  AddHandleTask(Task_queueStru stru);
    Task_queueStru  GetCurHandleTask();
    int m_exetaskid;
    int m_rectaskid;
    //存储任务ID
    QQueue<Task_queueStru> m_taskqueue;
   QString RecoderTaskCmd(int index,uint8_t funcmd,cmdstru stru);

};

#endif // TASKMANGERCLASS_H
