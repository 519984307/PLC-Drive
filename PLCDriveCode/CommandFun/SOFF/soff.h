#ifndef SOFF_H
#define SOFF_H

#include <QObject>
#include <funmoduleinterface.h>
#include <QPair>
#include <QVector>
#include <MyGlobal.h>
#include <QDebug>
#include <serialdataframe.h>
#include <myshareconfig.h>
using namespace CoreGlobal;
class SOFF:public QObject,public FunModuleInterface
{
public:
    SOFF();
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
    soffcmdstru m_soffstru;
    int m_InBeginBytePos;
    int m_OutBeginBytePos;
private:
    void GetAxisBeginBytePos();
    bool SetAxisoff();
};

#endif // SOFF_H
