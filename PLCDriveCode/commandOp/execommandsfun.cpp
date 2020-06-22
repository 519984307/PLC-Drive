#include "execommandsfun.h"

ExeCommandsFun::ExeCommandsFun()
{
    m_funcmd = NULL;
    m_taskid = -1;
      qRegisterMetaType<cmdstru>("cmdstru");
}
///
/// \brief ExeCommandsFun::SetCommandsParam
/// \param stru
/// \param pfun
///
void ExeCommandsFun::SetCommandsParam(uint8_t cmd,cmdstru stru, FunModuleInterface *pfun,int taskid)
{
    m_taskid =  taskid;
    m_funcmd = pfun;
    m_stru = stru;
    cmdname = cmd;

}
///
/// \brief ExeCommandsFun::SlotCommandRun
///
void ExeCommandsFun::SlotCommandRun()
{
    if(m_funcmd)
    {
        int iresult  = -1;
        QString strMsg = "";
        m_funcmd->Initialization();
        m_funcmd->SetModuleParam(m_stru);
        m_funcmd->RunModule();
        iresult = m_funcmd->GetExcResult(strMsg);
        m_funcmd->StopModule();
        emit signalComandsEnd(cmdname,m_stru,m_taskid);
    }
}
