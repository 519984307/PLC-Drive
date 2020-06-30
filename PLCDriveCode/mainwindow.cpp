#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QRect>
#include <QApplication>
#include <QDesktopWidget>
#include <QDateTime>
CommandFunOp ob;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ob.InitAllParam();
    QRect  desk_rect = QApplication::desktop()->availableGeometry();
    this->resize(desk_rect.width(),desk_rect.height());
    QVBoxLayout *lay = new QVBoxLayout;
    QLabel *label = new QLabel ("轴的监视信息",this);
    lay->addWidget(label);
    QStringList hlist;
    hlist<<"轴id"<<"当前位置"<<"故障代码"<<"故障位"<<"运行状态0/1"<<"轴inp"<<"son状态";
    m_pmonitortable = new QTableWidget();
    m_pmonitortable->setShowGrid(true);
    m_pmonitortable->verticalHeader()->setVisible(false);
    m_pmonitortable->setSelectionBehavior(QAbstractItemView::SelectRows);//选择一行
    m_pmonitortable->setColumnCount(7);
    m_pmonitortable->setHorizontalHeaderLabels(hlist);
    m_pmonitortable->setRowCount( ob.m_threadidvec.size());
    m_pmonitortable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    for(int i= 0; i < ob.m_threadidvec.size();++i)
    {
        m_pmonitortable->setRowHeight(i,25);
    }
    for(int i  =0 ; i < ob.m_threadidvec.size(); ++i)
    {
        for(int j = 0; j < 8;++j)
        {
            m_pmonitortable->setColumnWidth(j,desk_rect.width()/7-5);
            QTableWidgetItem *item = new  QTableWidgetItem();
            item->setText("");
            m_pmonitortable->setItem(i,j,item);
            item->setTextAlignment(Qt::AlignCenter);
            if(j == 0)
            {
                item->setText(QString::number(ob.m_threadidvec[i]));
            }
        }
    }
    lay->addWidget(m_pmonitortable);
    QLabel *tasklabel = new QLabel ("执行的任务信息",this);
    m_ptaskmsg = new QTextEdit(this);
    m_ptaskmsg->document()->setMaximumBlockCount(100);
    m_ptaskmsg->setAcceptRichText(true);
    lay->addWidget(tasklabel);
    lay->addWidget(m_ptaskmsg);

    m_pshowerrorinfo = new QLineEdit(this);
    lay->addWidget(m_pshowerrorinfo);
    ui->centralWidget->setLayout(lay);
    m_timer = new QTimer(this);
    connect(m_timer,&QTimer::timeout,this,&MainWindow::slotmintorinfo);
    m_timer->start(200);
    connect(&ob,&CommandFunOp::signalShowInfo,this,&MainWindow::slotshowloginfo);
    connect(&ob,&CommandFunOp::signaltaskInfo,this,&MainWindow::slotRecordertaskinfo);
    QString msg = "接受指令索引运行指令";
    slotRecordertaskinfo(msg);
    msg = " 执行编号结束指令";
     slotRecordertaskinfo(msg);
}

MainWindow::~MainWindow()
{
    m_timer->stop();
     CoreLog::QLoggerManager::getInstance()->closeLogger();
    delete ui;
}
///
/// \brief MainWindow::slotmintorinfo
/// 展示信息，显示在表格上
void MainWindow::slotmintorinfo()
{
    ob.m_monitorvalues.size() ;

    for(int i =0; i < m_pmonitortable->rowCount(); ++i)
    {
        for(int j =1; j < m_pmonitortable->columnCount(); ++j)
        {
            QString text  = "";
            if(ob.m_monitorvalues.contains(j))
            {
                if(ob.m_monitorvalues[j].size() >i)
                {
                    text = QString::number(ob.m_monitorvalues[j].at(i));
                }
            }
            m_pmonitortable->item(i,j)->setText(text);//第一个当前位置 ，故障代码，故障位，到位信号
        }
    }
}
///
/// \brief MainWindow::slotshowloginfo
/// \param msg
///
void MainWindow::slotshowloginfo(QString msg)
{
    if(m_pshowerrorinfo)
    {
        m_pshowerrorinfo->setText(msg);
        m_pshowerrorinfo->setStyleSheet("color: rgb(255, 0, 0);");
    }
}
///任务接受信息
void MainWindow::slotRecordertaskinfo(QString msg)
{
//    if(msg.contains("结束"))
//    {
//     str = QString("<font color=\"#FF0000\">%1</font> ").arg(msg);
//    }
//    QString str = QString("<font color=\"#FF0000\">%1</font> ").arg(msg);
//    m_ptaskmsg->append(str);
    //   m_ptaskmsg->append("<font color=\"#00FF00\">绿色字体</font> ");
    // m_ptaskmsg->append("<font color=\"#0000FF\">蓝色字体</font> ");
    if(msg!="")
    {
        QDateTime time = QDateTime::currentDateTime();
        QString strtime = time.toString("yyyy-MM-dd ddd hh:mm:ss");
        msg=strtime+":"+msg+"\n";
        QString str;
        if(msg.contains("结束"))
        {
         str = QString("<font color=\"#00FF00\">%1</font> ").arg(msg);
        }
        else{
             str = QString("<font color=\"#0000FF\">%1</font> ").arg(msg);
        }
        //m_ptaskmsg->moveCursor(QTextCursor::End);
         m_ptaskmsg->append(str);
    }
}
