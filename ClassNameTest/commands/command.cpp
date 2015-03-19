#include "command.h"
#include <typeinfo>
#include <QDebug>

Command::Command()
{
}

QString Command::getClassName()
{
    char *z=(char *)typeid(*this).name();
    while (isdigit(*z)) z++;
    qDebug() << z;
    return  QString(z);
}
