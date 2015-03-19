#include "environmentinfo.h"
#include <QSysInfo>
#include <QTextCodec>
#include <QProcess>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QDebug>


#ifdef Q_OS_WIN
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#include <windows.h>
#endif



static const char DEFAULT_DIVIDER[] = "; ";
static const int PING_TIMEOUT=500;

QString EnvironmentInfo::loadOSInfo()
{
QString infoStr("Current Operating System: %1");
#ifdef Q_OS_WIN
switch(QSysInfo::windowsVersion())
{
case QSysInfo::WV_NT: return infoStr.arg("Windows NT");
case QSysInfo::WV_2000: return infoStr.arg("Windows 2000");
case QSysInfo::WV_XP: return infoStr.arg("Windows XP");
case QSysInfo::WV_2003: return infoStr.arg("Windows 2003");
case QSysInfo::WV_VISTA: return infoStr.arg("Windows Vista");
case QSysInfo::WV_WINDOWS7: return infoStr.arg("Windows Seven");
default: return infoStr.arg("Windows");
}
#endif // Q_OS_WIN

#ifdef Q_OS_MAC
switch(QSysInfo::MacintoshVersion())
{
case QSysInfo::MV_CHEETAH: return infoStr.arg("Mac OS X 1.0 Cheetah");
case QSysInfo::MV_PUMA: return infoStr.arg("Mac OS X 1.1 Puma");
case QSysInfo::MV_JAGUAR: return infoStr.arg("Mac OS X 1.2 Jaguar");
case QSysInfo::MV_PANTHER: return infoStr.arg("Mac OS X 1.3 Panther");
case QSysInfo::MV_TIGER: return infoStr.arg("Mac OS X 1.4 Tiger");
case QSysInfo::MV_LEOPARD: return infoStr.arg("Mac OS X 1.5 Leopard");
case QSysInfo::MV_SNOWLEOPARD: return infoStr.arg("Mac OS X 1.6 Snow Leopard");
default: return infoStr.arg("Mac OS X");
}
#endif // Q_OS_MAC

#ifdef Q_OS_LINUX
#ifdef LINUX_OS_VERSION
return infoStr.arg(LINUX_OS_VERSION);
#else
return infoStr.arg("Linux");
#endif // LINUX_OS_VERSION
#endif // Q_OS_LINUX
}

QString EnvironmentInfo::loadProceessorInfo()
{
#ifdef Q_OS_WIN
 return QString("Architecture:%1 \nNumber of Processors:%2 \nProcessor Identifier:%3").arg(getenv("PROCESSOR_ARCHITECTURE")).arg(getenv("NUMBER_OF_PROCESSORS")).arg(getenv("PROCESSOR_IDENTIFIER"));
#endif // Q_OS_WIN
#ifdef Q_OS_LINUX
  QString("Architecture:%1").arg(getenv("MACHTYPE"));
#endif
}


QString EnvironmentInfo::loadMemoryInfo()
{
#ifdef Q_OS_WIN
    MEMORYSTATUSEX memory_status;
    ZeroMemory(&memory_status, sizeof(MEMORYSTATUSEX));
    memory_status.dwLength = sizeof(MEMORYSTATUSEX);
    if (GlobalMemoryStatusEx(&memory_status)) {
      return QString("RAM: %1 MB").arg(memory_status.ullTotalPhys / (1024 * 1024));
    } else {
      return QString("Unknown RAM");
    }
#endif // Q_OS_WIN
#ifdef Q_OS_LINUX
    QProcess p;
    p.start("awk", QStringList() << "/MemTotal/ { print $2 }" << "/proc/meminfo");
    p.waitForFinished();
    QString memory = p.readAllStandardOutput();
    memory.append(QString("; RAM: %1 MB").arg(memory.toLong() / 1024));
    p.close();
    return memory;
#endif
#ifdef Q_OS_MAC
    QProcess p;
    p.start("sysctl", QStringList() << "kern.version" << "hw.physmem");
    p.waitForFinished();
    QString system_info = p.readAllStandardOutput();
    p.close();
    return system_info;
#endif
}

QString EnvironmentInfo::pingHost(const QString &host_address)
{
    if(host_address.isEmpty())return QString(" Host address is empty");
/*
#ifdef DEBUG
        qDebug()<<"core: trying to ping host:"<<host_address;
#endif
        int ping_timeout=500;
        QString exe_path;
        #ifdef Q_WS_WIN
            exe_path="ping.exe";
        #endif
        #ifdef Q_WS_X11
            exe_path="/bin/ping";
        #endif

            QStringList arguments;
            #ifdef Q_WS_WIN
                QTextCodec::setCodecForCStrings(QTextCodec::codecForName("cp-866"));
                arguments<<host_address;
                arguments<<"-n 3"
            #endif
            #ifdef Q_WS_X11
                QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
                arguments<<host_address<<"-c 1";
            #endif

            QProcess myProcess;
            myProcess.start(exe_path,arguments);
            myProcess.waitForFinished(ping_timeout);

            QStringList strlstPingResults;
            strlstPingResults<<myProcess.readAll();
            myProcess.deleteLater();

            QString strResult="";
            for (int i=0;i<=strlstPingResults.count()-1;i++)
            {
#ifdef DEBUG
                qDebug()<<"Ping results:"<<strlstPingResults[i];
#endif
                if (!strResult.isEmpty()) {
                    strResult+='\n';
                }
                strResult+=strlstPingResults[i];
                if (strlstPingResults[i].contains("ttl",Qt::CaseInsensitive))
                 return strResult;
            };
*/

    if(host_address.isEmpty())return QString(" Host address is empty");
    QStringList arguments;
    if (host_address.isEmpty()) {
        return QString("Wrong host address %1").arg(host_address);
    }
    arguments<<host_address<<"-n"<<"3";
    QProcess ping;
    ping.start("ping",arguments);
    ping.waitForFinished(PING_TIMEOUT);
    QString result;
    while(ping.canReadLine()) {
        QString line = ping.readLine();
        result.append(line);
    }
    return result;

}

QString EnvironmentInfo::pingHostList(const QStringList &host_list)
{
    QString pingHostListResult;
    for (int i=0;i<=host_list.count()-1;i++)
    {

#ifdef DEBUG
     qDebug()<<"Ping address:"<<host_list[i];
#endif
     pingHostListResult+=pingHost(host_list[i]);
    }
    return pingHostListResult;
}


QString EnvironmentInfo::processRequest(QString URLaddress)
{
    // create custom temporary event loop on stack
        QEventLoop eventLoop;

        // "quit()" the event-loop, when the network request "finished()"
        QNetworkAccessManager mgr;
        qDebug()<<"Request "<<URLaddress;
        QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

        // the HTTP request
        QUrl url;
        url= URLaddress;
        QNetworkRequest req(url);
        QNetworkReply *reply = mgr.get(req);
        eventLoop.exec(); // blocks stack until "finished()" has been called
        QString result;
        if (reply->error() == QNetworkReply::NoError) {
            //success
            qDebug() << "Success";
            QByteArray bytes = reply->readAll();
            QString string(bytes);
            qDebug() << string;
            result=string;
        }
        else {
            //failure
            qDebug() << "Failure";
            result=reply->errorString();
            qDebug() << result;
            delete reply;
        }
        delete reply;
        return result;
}
