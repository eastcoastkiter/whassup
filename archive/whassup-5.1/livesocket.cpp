#include "livesocket.h"
using namespace std;

LiveSocket::LiveSocket(QString Purpose, QString Format, QString Hostname, int Port, QString Command, QObject * parent ): QTcpSocket( parent )
{
    h=Hostname;
    p=Port;
    c=Command;
    storedPurpose=Purpose;
    OutputFormat=Format;
    connect( this, SIGNAL( readyRead() ) , this, SLOT( readyReadLiveStatus() ) );
    connect( this, SIGNAL( stateChanged(QAbstractSocket::SocketState)) , this, SLOT(stateChangedLiveStatus(QAbstractSocket::SocketState) ) );
}


void LiveSocket::connectLiveSocket()
{
    connectToHost(h, p);

        if ( !waitForConnected(30000) )
        {
            qDebug() << "Unable to connect To Server" << errorString();
            emit errorLiveSocket(QString(errorString()));
        }
        else
        {
            qDebug() << "Connection OK\n" << c.toLatin1();
            write (c.toLatin1());
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
            if (OutputFormat == "json")
            {
                qDebug()<< "got JSON output" << LiveSocketData;
                QJsonParseError jsonError ;
                QJsonDocument json = QJsonDocument::fromJson(LiveSocketData, &jsonError);

                if (json.isNull())
                {
                    qDebug() << jsonError.errorString();
                    errorLiveSocket(jsonError.errorString());
                }
                else
                {
                    //qDebug()<< "JSON burn" << json.toJson();

                    QJsonArray jsonArray=json.array().first().toArray();
                    //QJsonArray jsonSubArray=jsonArray.first().toArray();

                    for (int i=0;i<jsonArray.size();i++)
                    {
                        qDebug() << "Json Value"<< i<< jsonArray.at(i);

                    }
                    emit sendLiveStatusJSONArray(storedPurpose, jsonArray);
                }

            }
            else
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
   /*     else if (state == QAbstractSocket::UnconnectedState )
        {
            emit errorLiveSocket(QString(errorString()));
        }
        */
}
