#include "livesocketssl.h"
using namespace std;

LiveSocketSsl::LiveSocketSsl(QString Purpose, QString Format, QString Hostname, int Port, QString Command, QObject * parent ): QSslSocket( parent )
{
    qDebug() << "QSslSocket::supportsSsl()" << QSslSocket::supportsSsl();
    setPeerVerifyMode(QSslSocket::VerifyNone);


    h=Hostname;
    p=Port;
   // c="auth line from whassup\n" + Command;
    c=Command;
    storedPurpose=Purpose;
    OutputFormat=Format;
   // connect( this, SIGNAL( readyRead() ) , this, SLOT( readyReadLiveStatus() ) );
       connect( this, SIGNAL( readyRead() ) , this, SLOT( readyReadLiveStatus() ) );
       connect( this, SIGNAL( stateChanged(QAbstractSocket::SocketState)) , this, SLOT(stateChangedLiveStatus(QAbstractSocket::SocketState) ) );
       connect( this, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT( handleSslErrors(QList<QSslError>)));
       connect(this, SIGNAL(encrypted()), this, SLOT(socketEncrypted()));
}


void LiveSocketSsl::connectLiveSocket()
{
    connectToHostEncrypted(h, p);

       // if ( !waitForConnected(3000) )
        if (waitForEncrypted(3000))
        {
            qDebug() << "Unable to connect To Server" << errorString();
            //emit errorLiveSocket(QString(errorString()));
        }
        else
        {
            qDebug() << "Connection OK\n" ;
          //  write (c.toLatin1());
        }
}

LiveSocketSsl::~LiveSocketSsl()
{
}

void LiveSocketSsl::readyReadLiveStatus()
{
  //  qDebug("Reading\n");
    while (!atEnd())
    {
        //QByteArray data = readAll();
        QByteArray data = read(bytesAvailable());
        LiveSocketData+=data;
        qDebug() << "data" << data;
    }
}

void LiveSocketSsl::stateChangedLiveStatus(QAbstractSocket::SocketState state)
{
    qDebug() << state;
        if (state == QAbstractSocket::ClosingState)
        {
            if (OutputFormat.toLower() == "json")
            {
                qDebug()<< "got JSON output" << LiveSocketData;
                QJsonParseError jsonError ;
                QJsonDocument json = QJsonDocument::fromJson(LiveSocketData, &jsonError);

                if (json.isNull())
                {
                    qDebug() << jsonError.errorString();
                    emit errorLiveSocket(jsonError.errorString(),2);
                }
                else
                {
                    //qDebug()<< "JSON burn" << json.toJson();

                    QJsonArray jsonArray=json.array();
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

void LiveSocketSsl::handleSslErrors(const QList<QSslError> &errors)
{
    foreach (const QSslError &error, errors)
        qDebug() << "SSL error" << error.errorString();
    ignoreSslErrors();
        qDebug() << "QSslSocket::isEncrypted() in handle Errors" << isEncrypted();
}

void LiveSocketSsl::socketEncrypted()
{
    qDebug() << "Socket encrypted in Slot socketEncrypted()" << isEncrypted();

QSslCipher ciph = sessionCipher();
QString cipher = QString("%1, %2 (%3/%4)").arg(ciph.authenticationMethod())
                 .arg(ciph.name()).arg(ciph.usedBits()).arg(ciph.supportedBits());;
qDebug() << "Cipher" << cipher;
qDebug() << "Writing" << c.toLatin1();
  write (c.toLatin1());
}
