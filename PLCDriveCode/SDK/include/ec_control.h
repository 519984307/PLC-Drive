#ifndef EC_CONTROL_H
#define EC_CONTROL_H
#ifndef EC_CONTROL_GLOBAL_H
#define EC_CONTROL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(EC_CONTROL_LIBRARY)
#  define EC_CONTROLSHARED_EXPORT Q_DECL_EXPORT
#else
#  define EC_CONTROLSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // EC_CONTROL_GLOBAL_H

#include <MyEC_Shared.h>
#define MAX_DATA_SIZE  8 * 1024

class EC_CONTROLSHARED_EXPORT Ec_control
{
public:
    Ec_control();
    void Start();
    void Stop();
private:
    static char IOmap[MAX_DATA_SIZE];
    static unsigned long int* thread1;
    static unsigned long int* thread2;
    static unsigned long int* thread3;
    static char *ifname;

private:
    static void ec_loopData();
    static void ec_processCheck();
    static void ec_loopDC();

};

#endif // EC_CONTROL_H
