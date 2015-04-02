#ifndef ENVIRONMENTINFO_H
#define ENVIRONMENTINFO_H
#include <QString>

class OsInfo {
private:
    QString name;
    QString version;
    quint16 majorVersion;
    quint16 minorVersion;
    quint16 buildNumber;
    quint16 platformId;
    quint16 servicePackMajor;
    quint16 servicePackMinor;
    quint16 suiteMask;
    qint8   productType;
public:
    OsInfo();
    QString getName() {
        return name;
    }
    QString getVersion() {
        return version;
    }

    quint16 getMajorVersion() {
        return majorVersion;
    }

    quint16 getMinorVersion() {
        return minorVersion;
    }

    quint16 getBuildNumber() {
        return buildNumber;
    }

    quint16 getPlatformId() {
        return platformId;
    }

    quint16 getServicePackMajor() {
        return servicePackMajor;
    }

    quint16 getServicePackMinor() {
        return servicePackMinor;
    }

    quint16 getSuiteMask() {
        return suiteMask;
    }

    qint8   getProductType() {
        return productType;
    }
    qint8   getBitOs();
};

class EnvironmentInfo
{
public:
    static QString loadOSInfo();
    static QString loadProceessorInfo();
    static QString loadMemoryInfo();
    static QString loadSystemEnvironment();
    static QString loadIPAddress();
    static QString loadIPAsDigitSequence();
    static QString loadAllIPAddresses();
    static QString pingHost(const QString &hostAddress);
    static QString pingHostList(const QStringList &host_list);
    static QString processRequest(QString &urlAddress);
    static QString loadValueFromRegestry(const char* regPath, const char* regKey);
};

#endif // ENVIRONMENTINFO_H
