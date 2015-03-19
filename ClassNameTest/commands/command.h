#ifndef COMMAND_H
#define COMMAND_H
#include <QString>
#include <QObject>

class Command
{
public:
    Command();
    virtual QString getClassName();
};

#endif // COMMAND_H
