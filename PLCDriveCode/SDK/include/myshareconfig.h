#ifndef MYSHARECONFIG_H
#define MYSHARECONFIG_H
#include <QString>
#include <hwconfig.h>
#include <funmoduleinterface.h>
#include <QMutex>

class MyShareconfig
{
private:
    MyShareconfig()
    {
        pluginMap.clear();

    }
public:
    static MyShareconfig *GetInstance()
    {
        static MyShareconfig pInstance;

        return &pInstance;
    }
    HWCONFIG_STRU hwconfigstru;
    QMap<QString,FunModuleInterface*> pluginMap;
    QMutex m_mutex;
    char m_Runstate[10] = {0};
};
#endif // MYSHARECONFIG_H
