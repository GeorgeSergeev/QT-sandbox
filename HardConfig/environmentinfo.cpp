#include "environmentinfo.h"
#include <QSysInfo>
#include <QTextCodec>
#include <QProcess>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QDebug>


#ifdef Q_OS_WIN
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#include <windows.h>
#endif

#define DEFAULT_DIVIDER "; "
#define PING_TIMEOUT 500

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
    return QString("Processor architecture:%1 \nNumber of Processors:%2 \nProcessor Identifier:%3").arg(getenv("PROCESSOR_ARCHITECTURE")).arg(getenv("NUMBER_OF_PROCESSORS")).arg(getenv("PROCESSOR_IDENTIFIER"));
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

QString EnvironmentInfo::loadIPAddress()
{
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
            return address.toString();
    }
    return QString("127.0.0.1");
}

QString EnvironmentInfo::loadIPAsDigitSequence()
{
    QString dirtyIP=loadIPAddress();
    QStringList ipParts=dirtyIP.split(".");
    QString result;
    foreach (QString part, ipParts) {
       while (part.length()<3) part.prepend("0");
       result.append(part);
    }
    return result;
}

QString EnvironmentInfo::loadAllIPAddresses()
{
    QString result("IP Address:\n ");
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
            result.append(address.toString()).append("\n");
    }
    return result;
}


QString EnvironmentInfo::loadSystemEnvironment()
{
    QStringList strlst=QProcessEnvironment::systemEnvironment().toStringList();
    QString result("System Environment");
    for (int i=0;i<strlst.count();i++) {
        result.append(strlst[i]);
        result.append("\n");
    }
    return result;
}



QString EnvironmentInfo::pingHost(const QString &hostAddress)
{
    QString result;
    try
    {
        QString adr=hostAddress.trimmed();
        if(adr.isEmpty()) {
            return QString(" Host address is empty");
        }
        QStringList arguments;
        if (!adr.indexOf("http")) {
           adr=QUrl(adr).host();
        }
        if (adr.isEmpty()) {
            return QString("Wrong host address %1").arg(adr);
        }
        qDebug()<<"Ping "<<adr;
        arguments<<adr<<"-n"<<"2";
        QProcess ping;
        ping.start("ping",arguments);
        ping.waitForFinished();
        while(ping.canReadLine()) {
            QString line = ping.readLine();
            result.append(line);
        }
    }
    catch(...) {
        qCritical()<<"Error ping host "<<hostAddress;
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
    QEventLoop eventLoop;
    QNetworkAccessManager mgr;
    qDebug()<<"Request "<<URLaddress;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    QUrl url;
    url= URLaddress;
    QNetworkRequest req(url);
    QNetworkReply *reply = mgr.get(req);
    eventLoop.exec();
    QString result;
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray bytes = reply->readAll();
        QString string(bytes);
        result=string;
    }
    else {
        qDebug()<<"Error request";
        result=reply->errorString();
    }
    delete reply;
    return result;
}
