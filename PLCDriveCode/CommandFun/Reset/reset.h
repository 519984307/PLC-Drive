#ifndef RESET_H
#define RESET_H

#include <QObject>
#include <funmoduleinterface.h>
#include <QPair>
#include <QVector>
#include <MyGlobal.h>
#include <QDebug>
#include <serialdataframe.h>
#include <myshareconfig.h>
using namespace CoreGlobal;
class Reset:public QObject,public FunModuleInterface
{
public:
    Reset();
public:
    virtual void Initialization()             ;
    virtual void SetModuleParam(cmdstru orderparam ) ;
    virtual void RunModule()                  ;
    virtual int  GetExcResult(QString &strMsg) ;
    virtual void StopModule();
    virtual FunModuleInterface *Copy()         ;
public:
    virtual QString GetModuleName()
    {
        return m_moduleName;
    }
private:
    QString m_moduleName;
    uchar *tempOutputData;
    QVector<QPair<int,int>> m_tempInOP;
    QVector<QPair<int,int>> m_tempOutOP;
    QVector<QPair<int,int>> m_tempOFFSON;
    resetcmdstru m_resetstru;
    int m_InBeginBytePos;
    int m_OutBeginBytePos;
private:
    void GetAxisBeginBytePos();
};

#endif // RESET_H
