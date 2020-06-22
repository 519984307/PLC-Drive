#ifndef SENDCOMMANDSCLASS_H
#define SENDCOMMANDSCLASS_H

#include <QObject>

class SendCommandsClass:public QObject
{
public:
    SendCommandsClass();
    void StopComandsfun();
    void GetCurPosfun();
    void GetCurErrorCodeFun();
};

#endif // SENDCOMMANDSCLASS_H
