#include "livesocketrequest.h"

LiveSocketRequest::LiveSocketRequest()
{
    RequestType="GET";
    RequestLimit=QString::null;
}

void LiveSocketRequest::setOutputFormat(QString opf)
{
    if (opf == "json")
    {
        OutputFormat="OutputFormat: json\n";
    }
    else
    {
        OutputFormat=QString::null;
    }
}

void LiveSocketRequest::setLimit(int l)
{
    if (l > 0)
    {
        RequestLimit=QString("Limit: %1").arg(l);
    }
    else
    {
        RequestLimit=QString::null;
    }
}

QString LiveSocketRequest::getLiveStatusRequest()
{
  /*  QString LiveStatusRequest;
    QString Table;
    QString OutputFormat;
    QStringList RequestFields;
    QStringList RequestFilters;
    QString RequestLimit;
    QString RequestRaw;
*/
    QString out;
    if (RequestType.toLower() == "get")
    {
        if ( (!Table.isEmpty()) && (RequestColumns.size()>0)   )
        {
            out = "GET " + Table + "\n";
            out += prepareColumns().trimmed() + "\n";
            if (!RequestFilters.isEmpty())
            {
                out += prepareFilters().trimmed() +"\n";
            }
            if (!RequestRaw.isEmpty())
            {
                out += RequestRaw.trimmed() + "\n";
            }
            if (!RequestLimit.isEmpty())
            {
                out += RequestLimit.trimmed() + "\n";
            }
            if (!OutputFormat.isEmpty())
            {
                out += OutputFormat;
            }
            if (!AuthUser.isEmpty())
            {
                out += "AuthUser: " + AuthUser + "\n";
            }
            out += "\n";
            return out;
        }
        else
        {
            out=QString::null;
        }
    }
    else if (RequestType.toLower() == "command")
    {
        out=QString::null;
    }
    else
    {
        out=QString::null;
    }

    return out;
}

QString LiveSocketRequest::prepareColumns()
{
    QString out="Columns: ";
    for (int i=0;i<RequestColumns.size();i++)
    {
        out += RequestColumns.at(i).trimmed() + " ";
    }
    return out.trimmed();
}

QString LiveSocketRequest::prepareFilters()
{
    QString out;
    for (int i=0;i<RequestFilters.size();i++)
    {
        out += "Filter: " + RequestFilters.at(i).trimmed() + "\n";
    }
    return out.trimmed();
}

void LiveSocketRequest::addRawText(QString raw)
{
        RequestRaw += raw.trimmed() + "\n";
}
