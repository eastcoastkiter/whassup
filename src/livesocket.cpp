#include "livesocket.h"
using namespace std;

LiveSocket::LiveSocket(QString Purpose, QString Hostname, int Port, QString Command, QObject * parent ): QTcpSocket( parent )
{
    storedPurpose=Purpose;

    connect( this, SIGNAL( readyRead() ) , this, SLOT( readyReadLiveStatus() ) );
    connect( this, SIGNAL( stateChanged(QAbstractSocket::SocketState)) , this, SLOT(stateChangedLiveStatus(QAbstractSocket::SocketState) ) );

    connectToHost(Hostname, Port);

        if ( !waitForConnected(3000) )
        {
            qDebug("Unable to connect To Server!\n");
            emit errorLiveSocket(errorString());
        }
        else
        {
            qDebug("Connection OK\n");
            write (Command.toLatin1());
        }
}

LiveSocket::~LiveSocket()
{
}

void LiveSocket::readyReadLiveStatus()
{
  //  qDebug("Reading\n");
    while (!atEnd())
    {
        QByteArray data = read(bytesAvailable());
        LiveSocketData+=data;
       // qDebug() << "data" << data;
    }
}

void LiveSocket::stateChangedLiveStatus(QAbstractSocket::SocketState state)
{
    qDebug() << state;
        if (state == QAbstractSocket::ClosingState)
        {
            QList<QStringList> listOfStringLists;

                //Splitt lines
                QList<QByteArray> dataList;
                dataList=LiveSocketData.split('\n');
        qDebug() << dataList.size() << "NEW LOG DATA" << LiveSocketData;
                for (int i = 0; i < dataList.size(); ++i)
                {
                        //split lines into datafield
                        QList<QByteArray> lineList;
                        lineList=dataList.at(i).split(';');
                        QStringList line;
                        for (int j = 0; j < lineList.size(); ++j)
                        {
                            //don't add empty fields (QString::KeepEmptyParts only on QString)
                            if (lineList.at(j).trimmed().isEmpty())
                            {
                               line << " ";
                            }
                            else
                            {
                               line << lineList.at(j).data();
                            }

                        }
                       listOfStringLists << line;
                 }

                emit sendLiveStatusData(storedPurpose, listOfStringLists);
        }
}
