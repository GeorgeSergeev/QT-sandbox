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
#include <QSettings>

#ifdef Q_OS_WIN
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#include <windows.h>
#endif

#define DEFAULT_DIVIDER "; "
#define PING_TIMEOUT 500


OsInfo::OsInfo()
{
    try {
#ifdef Q_OS_WIN
        name="Windows";
        OSVERSIONINFOEX osvi;
        ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
        osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
        GetVersionEx((OSVERSIONINFO*)&osvi);
        majorVersion = osvi.dwMajorVersion;
        minorVersion = osvi.dwMinorVersion;
        buildNumber = osvi.dwBuildNumber;
        platformId = osvi.dwPlatformId;
        servicePackMajor = osvi.wServicePackMajor;
        servicePackMinor = osvi.wServicePackMinor;
        suiteMask = osvi.wSuiteMask;
        productType = osvi.wProductType;
        switch(majorVersion) {
        case 10: version="10"; break;
        case  6 : switch(minorVersion) {
            case 3 : version = "8.1 or Server 2012 R2"; break;
            case 2 : version = "8 or Server 2012"; break;
            case 1 : version = "Seven or Server 2008 R2"; break;
            case 0 : version = "Vista or Server 2008"; break;
            }; break;
        case 5 : switch(minorVersion) {
            case 2 : version = "Server 2003";  break;
            case 1 : version = "XP"; break;
            case 0 : version = "Server 2000"; break;
            }; break;
        default :  version ="98 or early";
        }

#endif
#ifdef Q_OS_MAC
        name="OS MAC";
#endif
#ifdef Q_OS_LINUX
        name="Linux";
#endif
    }
    catch(...)
    {

    }
}


qint8 OsInfo::getBitOs()
{
    {
   #ifdef Q_OS_WIN
    #ifdef _WIN32
        QString path=getenv("PATH");
        QSettings settings("HKEY_CURRENT_USER\\Software", QSettings::NativeFormat);
        QStringList sections=settings.childGroups();
        if (sections.contains("Wow6432Node")) return 64;
        if (path.contains("Program Files (x86)")) return 64;
        return 32;
    #endif
    #ifdef _WIN64
           return 64;
    #endif
       }
   #endif
   return 32;
}



QString EnvironmentInfo::loadOSInfo()
{
    QString infoStr("Current Operating System: %1");
    try {
#ifdef Q_OS_WIN
        OsInfo *osInfo = new OsInfo();
        infoStr=infoStr.arg(osInfo->getName()).append(" ").append(osInfo->getVersion());
        infoStr=infoStr.append("\nWindows bit: %1\n").arg(osInfo->getBitOs());
        QString osDetails("Major version: %1\nMinor Version: %2\nBuild Number: %3\nPlatform Id: %4\nService pack major: %5\nService pack minor: %6\nSuite mask: %7\nProduct Type: %8");
        osDetails=osDetails.arg(osInfo->getMajorVersion())
                .arg(osInfo->getMinorVersion())
                .arg(osInfo->getBuildNumber())
                .arg(osInfo->getPlatformId())
                .arg(osInfo->getServicePackMajor())
                .arg(osInfo->getServicePackMinor())
                .arg(osInfo->getSuiteMask())
                .arg(osInfo->getProductType());
        return infoStr.append(osDetails);

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
    } catch (...)
    {
        return infoStr.arg("Unknown");
    }
}

QString EnvironmentInfo::loadProceessorInfo()
{
    QString result("Unknown architecture");
    try {
#ifdef Q_OS_WIN
        return QString("Processor architecture:%1 \nNumber of Processors:%2 \nProcessor Identifier:%3").arg(getenv("PROCESSOR_ARCHITECTURE")).arg(getenv("NUMBER_OF_PROCESSORS")).arg(getenv("PROCESSOR_IDENTIFIER"));

#endif // Q_OS_WIN
#ifdef Q_OS_LINUX
        return QString("Architecture:%1").arg(getenv("MACHTYPE"));
#endif
    } catch (...) {
        qDebug()<<"Loading processor info is crashed";
    }
    return result.toUtf8();
}


QString EnvironmentInfo::loadMemoryInfo()
{
    QString result("Unknown RAM");
    try {
#ifdef Q_OS_WIN
        MEMORYSTATUSEX memory_status;
        ZeroMemory(&memory_status, sizeof(MEMORYSTATUSEX));
        memory_status.dwLength = sizeof(MEMORYSTATUSEX);
        if (GlobalMemoryStatusEx(&memory_status)) {
            return QString("RAM: %1 MB").arg(memory_status.ullTotalPhys / (1024 * 1024));
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
    } catch (...) {
        qDebug()<<"Loading memory info is crashed";
    }
    return result.toUtf8();
}

QString EnvironmentInfo::loadIPAddress()
{
    try {
        foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
            if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
                return address.toString();
        }
    } catch(...) {
        qDebug()<<"Loading of IP addreses is crashed";
    }
    return QString("127.0.0.1");
}

QString EnvironmentInfo::loadIPAsDigitSequence()
{
    QString result;
    try {
        QString dirtyIP=loadIPAddress();
        QStringList ipParts=dirtyIP.split(".");
        foreach (QString part, ipParts) {
            while (part.length()<3) part.prepend("0");
            result.append(part);
        }
    } catch (...) {
        qDebug()<<"Loading IP ad digit sequence is crashed";
    }

    return result.toUtf8();
}

QString EnvironmentInfo::loadAllIPAddresses()
{
    QString result("IP Address:\n ");
    try {
        foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
            if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
                result.append(address.toString()).append("\n");
        }
    }
    catch (...) {
        qDebug()<<"Loading of IP addreses is crashed";
    }

    return result.toUtf8();
}


QString EnvironmentInfo::loadSystemEnvironment()
{
    QString result("System Environment");
    try {
        QStringList strlst=QProcessEnvironment::systemEnvironment().toStringList();
        for (int i=0;i<strlst.count();i++) {
            result.append(strlst[i]);
            result.append("\n");
        }
    } catch(...) {
        qDebug()<<"Loading of system environment is crashed";
    }

    return result.toUtf8();
}



QString EnvironmentInfo::pingHost(const QString &hostAddress)
{
    QString outStream;
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
        QByteArray BA = ping.readAll();
        QTextStream textStream ( &BA);
        textStream.setCodec("IBM-866");
        return textStream.readAll();
    }
    catch(...) {
        qDebug()<<"Error ping host "<<hostAddress;
    }
    //QTextStream textStream (&outStream);
    //textStream.setCodec("IBM-866");
  //  QTextCodec *codec = QTextCodec::codecForName("IBM-866");
  //  return codec->toUnicode(outStream.);
    return outStream;
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

QString EnvironmentInfo::processRequest(QString &urlAddress)
{
    QString result;
    try {
        QEventLoop eventLoop;
        QNetworkAccessManager mgr;
        qDebug()<<"Request "<<urlAddress;
        QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
        QUrl url;
        url= urlAddress;
        QNetworkRequest req(url);
        QNetworkReply *reply = mgr.get(req);
        eventLoop.exec();
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray bytes = reply->readAll();
            QString string(bytes);
            result=string;
        }
        else {
            qCritical()<<"Error request";
            result=reply->errorString();
        }
        delete reply;
        mgr.deleteLater();
        eventLoop.deleteLater();
    } catch (...)
    {
        qCritical()<<"Error request to "<<urlAddress;
    }

    return result;
}


QString EnvironmentInfo::loadValueFromRegestry(const char* regPath, const char* regKey)
{
#ifdef Q_OS_WIN
    QString val;
    try {
        QSettings settings(
                    regPath, QSettings::NativeFormat);
        val = settings.value(regKey).toString();
    }
    catch(...) {

    }
    return val;
#else
    return "";
#endif
}
