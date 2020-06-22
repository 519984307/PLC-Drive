#ifndef READHWCONFIGFILE_H
#define READHWCONFIGFILE_H
#include <QObject>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QTextStream>
#include <QFile>
#include <QString>
#include <QMap>
#include <QVector>
#include "readhwconfigfile_global.h"
#include <hwconfig.h>
class READHWCONFIGFILESHARED_EXPORT ReadHWconfigFile:public QObject
{

public:
    ReadHWconfigFile();
    QString ReadHWconfigType(HWCONFIG_STRU &stru,QString filename);
private:
   void ReadHWInputType(QXmlStreamReader &reader,QString strElement, QMap<int,QMap<int,HWCONFIGPROPERTY_STRU>> &hwconfigInputMap);
   void ReadHWOutputType(QXmlStreamReader &reader,QString strElement, QMap<int,QMap<int,HWCONFIGPROPERTY_STRU>> &hwconfigOutputMap);
};

#endif // READHWCONFIGFILE_H
