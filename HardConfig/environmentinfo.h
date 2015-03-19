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
    static QString pingHost(const QString &host_address);
    static QString pingHostList(const QStringList &host_list);
    static QString processRequest(QString URLaddress);
};

#endif // ENVIRONMENTINFO_H
