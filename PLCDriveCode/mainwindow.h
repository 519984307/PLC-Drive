#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "commandOp/commandfunop.h"
#include  <QTableWidget>
#include <QTextEdit>
#include <QTimer>
#include <QLineEdit>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QTableWidget *m_pmonitortable;
    QTextEdit *m_ptaskmsg;
    QTimer *m_timer;
    QLineEdit *m_pshowerrorinfo;
private slots:
    void slotmintorinfo();
    void slotshowloginfo(QString msg);
    void slotRecordertaskinfo(QString msg);
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
