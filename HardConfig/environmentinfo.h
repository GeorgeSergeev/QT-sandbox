#ifndef ENVIRONMENTINFO_H
#define ENVIRONMENTINFO_H
#include <QString>
#include <QStringList>

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
    static QString processRequest(QString URLaddress);
};

#endif // ENVIRONMENTINFO_H
