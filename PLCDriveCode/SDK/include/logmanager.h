/************************************
* @brief 摘要 log rizhi de baocun
* @author sunny
* @date 2017/
************************************/

#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include "logmanager_global.h"
#include<QStringList>
#include<QObject>
#include <QDir>
#include<QMap>
#include<QThread>
#include<QMutex>
#include <QQueue>
namespace CoreLog {

    enum LogLevel { InfoLevel = 0, WarnLevel, ErrorLevel};

    typedef struct _LogStru
    {
        QString strModule;
        LogLevel iLevel;
        QString strMsg;
        _LogStru()
        {
            strModule = "";
            iLevel = InfoLevel;
            strMsg = "";
        }
    }LogStru;
    static QMutex g_Mutex;
    void LOGMANAGERSHARED_EXPORT QLog_Info(const QString &module, const QString &message);

    void LOGMANAGERSHARED_EXPORT QLog_Warning(const QString &module, const QString &message);

    void LOGMANAGERSHARED_EXPORT QLog_Error(const QString &module, const QString &message);

    void LOGMANAGERSHARED_EXPORT QLog_(const QString &module, LogLevel level, const QString &message);
    void LOGMANAGERSHARED_EXPORT QLog_Async(const QString &module, LogLevel level, const QString &message);
    /**
     * @brief The QLoggerWriter class writes the message and manages the file where it is printed.
     */
    class LOGMANAGERSHARED_EXPORT QLogger : public QObject
    {
        Q_OBJECT

        public:
            explicit QLogger(const QString &fileDestination, LogLevel level,QObject *parent=0);
            explicit QLogger(LogLevel level,QObject *parent=0);

            LogLevel getLevel() const { return m_level; }

            bool write(const QString &module, const QString &message);
            bool write(const QString &i_module,const LogLevel i_level, const QString &i_message);

            bool read(QString &o_message);
            bool read(QStringList &o_message);
    public:
            ~QLogger();
    signals:
            void LogUpdateUI(QString str);
        private:
            QString m_fileDestination;
            LogLevel m_level;
    private:
        QFile file;
        bool isFirst ;
    public:
        QQueue<QString> m_strMsgQue;
    };

    class LOGMANAGERSHARED_EXPORT QLoggerManager : public QThread
    {
        public:
            static QLoggerManager * getInstance();
            static QString levelToText(const LogLevel &level);

            bool addLogInstance(const QString &fileDest, const QStringList &modules, LogLevel level);
            bool addLogInstance(const QString &fileDest, const QString &module, LogLevel level);
            bool addLogInstance(const QString &module,LogLevel level);

            QLogger * getLogInstance(const QString &module) { return moduleDest.value(module); }
            QMap<QString,QLogger*> getLogInstanceMap(){return moduleDest;}
            void closeLogger();
    protected:
        void run();
    public:
        QQueue<LogStru> m_queue; //取元素释放出错
        QMutex mutex;

        private:
            bool m_stopFlag;
            static QLoggerManager *INSTANCE;
            QMap<QString,QLogger*> moduleDest;
            QLoggerManager();
    };

}

#endif // LOGMANAGER_H
