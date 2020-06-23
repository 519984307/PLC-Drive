#ifndef ANALYSISDATALIB_H
#define ANALYSISDATALIB_H

#include "analysisdatalib_global.h"
#include <myshareconfig.h>
#include <QObject>
///
/// \brief The AnalysisDataLib class
///读硬件配置的数据的内容,并计算相关ec数据部分的赋值计算的内容
class  AnalysisDataLib:public QObject
{
public:
    AnalysisDataLib();
    void LoadPluginsmodule();
    void ReadHWConfigFile(QString path);
    void CalcECbytePosandNum(HWCONFIG_STRU &stru);
    bool MallocECbyteSize();
    void SetS_ONParam();
};

#endif // ANALYSISDATALIB_H
