#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "commandOp/commandfunop.h"
#include  <QTableWidget>
#include <QTextEdit>
#include <QTimer>
#include <QLineEdit>
#include<QLabel>
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
    QLabel *m_connectstat;

private slots:
    void slotmintorinfo();
    void slotshowloginfo(QString msg);
    void slotRecordertaskinfo(QString msg);
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
