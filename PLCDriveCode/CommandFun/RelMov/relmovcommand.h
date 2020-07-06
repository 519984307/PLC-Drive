#ifndef RELMOVCOMMAND_H
#define RELMOVCOMMAND_H

#include <QObject>
#include <QtPlugin>
#include <funmoduleinterface.h>
#include <QPair>
#include <QVector>
#include <MyGlobal.h>
#include <QDebug>
#include <serialdataframe.h>
using namespace CoreGlobal;

class RelMovCommand:public QObject,public FunModuleInterface
{
     Q_OBJECT
public:
    RelMovCommand();
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
  bool  m_isInitStatOk;
  ppcmdstru m_relstru;
   int m_axisid;
  int m_InBeginBytePos;
  int m_OutBeginBytePos;
private:
  void GetAxisBeginBytePos();
  void RelativeChange();
  void SetAxisParm();
  void SetAxisModel();
  void ResetAllAxis(const QVector<int> &tempVec);
  bool SetAxisInit(const QVector<int> &tempVec);
  bool CheckAxisInit();
  void RunAxis();
  void StopAxis();
  int GetInputData(int &curPos);
};

#endif // RELMOVCOMMAND_H
