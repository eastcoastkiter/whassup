#ifndef LIVESOCKET_H
#define LIVESOCKET_H

#include <QTcpSocket>
#include <QObject>
#include <QList>
#include <QStringList>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
    
/*
 This socket is really shitty, we need own classe and structure.
 But thats for the next version
*/

class LiveSocket: public QTcpSocket
{
	Q_OBJECT
	
public:
    LiveSocket( QString Purpose="", QString Format="", QString Hostname="localhost", int Port=6557, QString Command="", QObject * parent = 0 );
    ~LiveSocket();
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
public slots:
    QString getPurpose(){ return storedPurpose; }
    void connectLiveSocket();
signals:
    void errorLiveSocket(QString,int);
    void sendLiveStatusData(QString, QList<QStringList> listOfStringLists);
    void sendLiveStatusJSONArray(QString, QJsonArray);
};
#endif

