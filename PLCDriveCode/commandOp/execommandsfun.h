#ifndef EXECOMMANDSFUN_H
#define EXECOMMANDSFUN_H
#include <funmoduleinterface.h>
#include <QObject>
#include <QPointer>
#include <serialdataframe.h>
#include <QMetaType>
class ExeCommandsFun:public QObject
{
    Q_OBJECT
public:
    ExeCommandsFun();
    void SetCommandsParam(uint8_t cmd,cmdstru stru, FunModuleInterface* pfun,int taskid);
private:
    FunModuleInterface*m_funcmd;
    cmdstru m_stru;
    int m_taskid;
    uint8_t cmdname;
public slots:
    void SlotCommandRun();
signals:
    void signalComandsEnd(uint8_t cmd,cmdstru stru,int taskid);
};

#endif // EXECOMMANDSFUN_H
