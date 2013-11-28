#include "webcommunication.h"

WebCommunication::WebCommunication(QObject * parent) : QObject(parent)
{

}

void WebCommunication::getJsonData(QString Purpose, QString Request)
{
    storedPurpose=Purpose;
    QString ServerAuthUser=qApp->property("ServerAuthUser").toString();
    QString ServerAuthPassword=qApp->property("ServerAuthPassword").toString();

    byteReply.clear();
    QString ServerCgiUrl=qApp->property("ServerCgiUrl").toString();

    //Get Data
    QUrl monitoringUrl;
    QUrlQuery monitoringUrlQuery;

    monitoringUrl.setUrl(ServerCgiUrl);


        if (!ServerAuthUser.isEmpty())
        {
            monitoringUrl.setUserName(ServerAuthUser.toLatin1());
        }
        if (!ServerAuthPassword.isEmpty())
        {
            monitoringUrl.setPassword(ServerAuthPassword.toLatin1());
        }



        QByteArray postData;

            // Host
           // monitoringUrlQuery.addQueryItem("data",QUrl::toPercentEncoding(Request));
            postData.append("data=" + QUrl::toPercentEncoding(Request));

            qDebug() << "Postdata" << postData;
            qDebug() << "monitoringUrl URL: "<< monitoringUrl.toString(QUrl::None);
            qDebug() << "URL Query: "<< monitoringUrlQuery.toString();
            //qDebug() << "Encoded:" << QUrl::fromEncoded(postData);

            //Data
            QNetworkRequest header;
            monitoringUrl.setQuery(monitoringUrlQuery);
            header.setUrl(monitoringUrl);
            header.setRawHeader( "User-Agent", "User Agent");
            header.setHeader(QNetworkRequest::ContentTypeHeader, "application/text");
            qDebug() << "send Http" << monitoringUrl;
            sendMonitoringHttp(header, postData);
}



void WebCommunication::sendMonitoringHttp(QNetworkRequest header, QByteArray postData)
{
qDebug() << "PostData: "<< postData;

QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    reply=manager->post(header,postData);
        connect(reply, SIGNAL(readyRead()), this, SLOT(slotReadyReadHttp()));
        connect(reply, SIGNAL(finished()), this, SLOT(proccessJsonData()));
        connect(reply, SIGNAL(sslErrors(QList<QSslError>)),this,SLOT(handleSslErrors(QList<QSslError>)));
}

void WebCommunication::handleSslErrors(const QList<QSslError> &errors)
{
    foreach (const QSslError &error, errors)
    {
        qDebug() << "SSL error" << error.errorString();
        reply->ignoreSslErrors();
        emit errorWebCommunication(error.errorString(),1);
    }
}

void WebCommunication::slotReadyReadHttp()
{
    qDebug() << "slotReadyReadHttp";
    //<title>401 Authorization Required</title>

    byteReply += reply->readAll();
        qDebug() << byteReply;

}


void WebCommunication::proccessJsonData()
{    
        QJsonParseError jsonError ;
        QJsonDocument json = QJsonDocument::fromJson(byteReply, &jsonError);


        if (!json.isNull())
        {
            QJsonArray jsonArray=json.array();
            emit sendWebCommunicationJSONArray(storedPurpose, jsonArray);


            for (int i=0;i<jsonArray.size();i++)
            {
                qDebug() << "Json Value"<< i<< jsonArray.at(i);

            }
        }
        else
        {
            //json Error occured, could be worng json, could be http error
            //except we send Command, will get no return from this one
            if (byteReply.isEmpty() && (storedPurpose != "Command") )
            {
                // we got nothing back, wierd
                errorWebCommunication("Communiction Error occured",2);
                return;
            }

            //<title>401

            // find like "<title>401 Authorization Required</title>"
            QRegExp re("<title>\\s*\\d{3}.*</title>",Qt::CaseInsensitive, QRegExp::RegExp);
            int pos = re.indexIn(byteReply);
            if (pos > 0)
            {
                // There is only one title
                QString found=re.capturedTexts().first();
                qDebug() << "cotains error" << pos << found ;
                found.remove(QRegExp("<\\W*title>"));


                // Lets search for authorization
                if (found.contains("401"))
                {
                    emit errorWebCommunication(found,1);
                    CommandWidget* cw = new CommandWidget(0);
                        bool auth=cw->showAuthCommandWidget();

                        if (auth)
                        {
                            qDebug() << "Tried auth";
                            emit authEntered();
                            deleteLater();
                        }


                }

                // in case something i dont think about
                if (storedPurpose != "Command")
                {
                    emit errorWebCommunication("Communication " +found,2);
                }

                //don't send generell json error, if http error code
                return;

            }

            // http ok, but json still kaputt, sad but true
            qDebug() << "json Error" << jsonError.errorString();
            if (storedPurpose != "Command")
            {
                emit errorWebCommunication(jsonError.errorString(),2);
            }
        }
}
