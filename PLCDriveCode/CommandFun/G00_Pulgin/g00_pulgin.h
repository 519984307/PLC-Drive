
#ifndef G00_PULGIN_H
#define G00_PULGIN_H
#include <QObject>
#include <QtPlugin>
#include <funmoduleinterface.h>
#include <QPair>
#include <QVector>
#include <MyGlobal.h>
#include <QDebug>
#include <serialdataframe.h>
using namespace CoreGlobal;

class G00_Pulgin:public QObject,public FunModuleInterface
{
    Q_OBJECT
public:
    G00_Pulgin();
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
   QVector<int> m_axisparamvec;
   bool m_InitOk;
   int m_InBeginBytePos;
   int m_OutBeginBytePos;
private:
   void GetAxisVarParamValue(QString);
   void GetAxisBeginBytePos();
   void SetAxisCWBit6();
   void SetAxisModel();
   void SetAxisParam();
   bool CheckAxisInit();
   bool SetAxisInit();
   void RunAxis();
   void StopAxis();
   int GetInputData(int &curPos);
};

#endif // G00_PULGIN_H
