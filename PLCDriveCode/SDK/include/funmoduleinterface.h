#ifndef FUNMODULEINTERFACE_H
#define FUNMODULEINTERFACE_H

#include <QtCore/QtPlugin>
#include <QString>
#include <QPoint>
#include<serialdataframe.h>


class FunModuleInterface
{
public:
    virtual void Initialization()                     = 0 ;
    virtual void SetModuleParam(cmdstru orderparam)  = 0 ;
    virtual void RunModule()                          = 0 ;
    virtual int  GetExcResult(QString &strMsg)        = 0 ;
    virtual void StopModule()                         = 0 ;
    virtual FunModuleInterface* Copy()                = 0 ;
public:
    virtual QString GetModuleName(){return "";}
    virtual bool ReadEvent(uchar*){return true;}
public:
    bool isTerminate;
};
//QT_BEGIN_NAMESPACE

//#define IBasicModule_iid "com.twsz.tc.classicladder.FunModuleInterface/1.0"

////Q_DECLARE_INTERFACE(FunModuleInterface, IBasicModule_iid)

//QT_END_NAMESPACE
#endif // FUNMODULEINTERFACE_H
