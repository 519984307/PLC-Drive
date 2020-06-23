#ifndef SENDCOMMANDSCLASS_H
#define SENDCOMMANDSCLASS_H
#include <myshareconfig.h>
#include <QPair>
#include <QVector>
#include <MyGlobal.h>
#include <QDebug>
#include <serialdataframe.h>
#include <QObject>
using namespace CoreGlobal;
class SendCommandsClass:public QObject
{
   Q_OBJECT
public:
    SendCommandsClass();
    ~SendCommandsClass();
    bool Initallaxisparam();
    void StopComandsfun(QVector<int>axisidvec);
    QVector<int> GetCurValuefun(QVector<int>axisidvec,int role);
    void GetAxisBeginBytePos(int axisid,int &inbeginbytepos,int &outbeginbytepos);
    void SetAxisParam(QVector<uint>axisidvec,int id);
private:
    QVector<QPair<int,int>> m_tempInOP;
    enum mointorroles{
        none = 0,
        curpos = 1,
        ErrorCode =2,
        Fault =3,
        Runstate =4,
        AxisINP =5,
        Excited =6,
        curmodel =7,
    };
};

#endif // SENDCOMMANDSCLASS_H
