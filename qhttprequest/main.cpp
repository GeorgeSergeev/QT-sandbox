#include <QtCore/QCoreApplication>
#include "httprequest.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    HttpRequest req;
    req.processRequest("http://localhost:8080/MangoCRMServer/Health");
    qDebug()<<"Yes";
    return a.exec();
}
