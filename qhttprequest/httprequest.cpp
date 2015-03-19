#include "httprequest.h"

/*
 * http://2lx.ru - Блог помешанного программиста
 * Статьи, учебники, руководства по программированию на C, C++, C#, Java, JavaScript, PHP, Perl, RegEx, SQL, и многое другое...
 */

HttpRequest::HttpRequest(QObject *parent) :
    QObject(parent)
{
    // 1. Создаем QNetworkAccessManager и ожидаем от него сигнала finished
    nam = new QNetworkAccessManager(this);
    QObject::connect(nam, SIGNAL(finished(QNetworkReply*)),
             this, SLOT(finishedSlot(QNetworkReply*)));
}

void HttpRequest::processRequest(QString URLaddress)
{
    // 2. осуществляем вызов нужного УРЛа
    QUrl url(URLaddress);
    qDebug()<<"Request "<<URLaddress;
    QNetworkReply* reply = nam->get(QNetworkRequest(url));
//    eventLoop.exec();
}

// 3. Принимаем и обрабатываем ответ сервера
void HttpRequest::finishedSlot(QNetworkReply* reply)
{
    // Не произошло-ли ошибки?
//    eventLoop.wakeUp();
    if (reply->error() == QNetworkReply::NoError)
    {
        // Читаем ответ от сервера
        QByteArray bytes = reply->readAll();
        QString string(bytes);

        // Выводим ответ на экран
        qDebug() << string;
    }
    // Произошла какая-то ошибка
    else
    {
        // обрабатываем ошибку
        qDebug() << reply->errorString();
    }
    delete reply;
}

