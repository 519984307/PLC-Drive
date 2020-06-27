#include "mainwindow.h"
#include <QApplication>
#include <QQueue>
#include <QDebug>
#include "commandOp/commandfunop.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
//    CommandFunOp ob;
//    ob.InitAllParam();
    //test 队列用法
//    QQueue <int> testqueue;
//    testqueue.push_back(0);
//    testqueue.push_back(1);
//    testqueue.push_back(2);
//    testqueue.push_back(3);
//    qDebug()<<"当前队列的第一个元素front:"<<testqueue.front();
//    qDebug()<<"当前队列的最后一个元素back:"<<testqueue.back();
//    /*  qDebug()<<"移出当前队列的一个元素pop:"<<*/testqueue.pop_back();
//    qDebug()<<"当前队列的第一个元素front:"<<testqueue.front();
//    qDebug()<<"当前队列的最后一个元素back:"<<testqueue.back();
    return a.exec();
}
