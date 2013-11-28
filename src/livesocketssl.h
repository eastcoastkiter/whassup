#ifndef LIVESOCKETSSL_H
#define LIVESOCKETSSL_H

#include <QTcpSocket>
#include <QObject>
#include <QList>
#include <QStringList>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSslSocket>
#include <QSslCipher>
    
/*
 This socket is really shitty, we need own classe and structure.
 But thats for the next version
*/

class LiveSocketSsl: public QSslSocket
{
	Q_OBJECT
	
public:
    LiveSocketSsl( QString Purpose="", QString Format="", QString Hostname="localhost", int Port=6557, QString Command="", QObject * parent = 0 );
    ~LiveSocketSsl();
private:
    QString storedPurpose;
    QByteArray LiveSocketData;
    QString h;
    QString c;
    int p;
    QString OutputFormat;
private slots:
    void readyReadLiveStatus();
    void stateChangedLiveStatus(QAbstractSocket::SocketState state);
    void handleSslErrors(const QList<QSslError> &errors);
    void socketEncrypted();
public slots:
    QString getPurpose(){ return storedPurpose; }
    void connectLiveSocket();
signals:
    void errorLiveSocket(QString, int);
    void sendLiveStatusData(QString, QList<QStringList> listOfStringLists);
    void sendLiveStatusJSONArray(QString, QJsonArray);
};
#endif

