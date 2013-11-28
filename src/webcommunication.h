#ifndef WEBCOMMUNICATION_H
#define WEBCOMMUNICATION_H

#include <QApplication>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonParseError>
#include <QByteArray>
#include <QRegExp>
#include <QStringList>
#include <QList>
#include <QSsl>
#include<QSslError>


#include "commandwidget.h"

class WebCommunication: public QObject
{
    Q_OBJECT
public:
    WebCommunication(QObject * parent = 0);


private:
    QByteArray byteReply;
    QNetworkReply * reply;
    QString storedPurpose;
private slots:
    void sendMonitoringHttp(QNetworkRequest, QByteArray);
    void slotReadyReadHttp();
    void proccessJsonData();
    void handleSslErrors(const QList<QSslError> &);
public slots:
    void getJsonData(QString, QString);
signals:
    void errorWebCommunication(QString, int);
    void sendWebCommunicationJSONArray(QString storedPurpose, QJsonArray jsonArray);
    void authEntered();
};


#endif // FCGICOMMUNICATION_H
