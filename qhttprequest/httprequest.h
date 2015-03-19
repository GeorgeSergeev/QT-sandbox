#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QThread>
#include <QDebug>
#include <QEventLoop>

class HttpRequest : public QObject
{
    Q_OBJECT
public:
    explicit HttpRequest(QObject *parent = 0);
    void processRequest(QString URLaddress);
signals:

public slots:
    void finishedSlot(QNetworkReply* reply);
private:
    QNetworkAccessManager* nam;
};

#endif // HTTPREQUEST_H
