#ifndef LIVESOCKET_H
#define LIVESOCKET_H

#include <QTcpSocket>
#include <QObject>
#include <QList>
#include <QStringList>
#include <QByteArray>
    
/*
 This socket is really shitty, we need own classe and structure.
 But thats for the next version
*/

class LiveSocket: public QTcpSocket
{
	Q_OBJECT
	
public:
    LiveSocket( QString Purpose="", QString Hostname="localhost", int Port=6557, QString Command="", QObject * parent = 0 );
    ~LiveSocket();
private:
    QString storedPurpose;
    QByteArray LiveSocketData;
private slots:
    void readyReadLiveStatus();
    void stateChangedLiveStatus(QAbstractSocket::SocketState state);
public slots:
    QString getPurpose(){ return storedPurpose; }
signals:
    void errorLiveSocket(QString);
    void sendLiveStatusData(QString, QList<QStringList> listOfStringLists);
};
#endif

