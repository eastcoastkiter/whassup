#ifndef LIVESOCKETREQUEST_H
#define LIVESOCKETREQUEST_H

#include <QObject>
#include <QDebug>
#include <QStringList>

class LiveSocketRequest
{
public:
    LiveSocketRequest();

    QString RequestType;
    QString LiveStatusRequest;
    QString Table;
    QString OutputFormat;
    QStringList RequestColumns;
    QStringList RequestFilters;
    QString RequestLimit;
    QString RequestRaw;

    void setCommand(QString c){ RequestType = c; }
    void setOutputFormat(QString);
    void setTable(QString t){ Table = t; }
    void addColumn(QString f){ RequestColumns.append(f); }
    void addColumns(QStringList fl){ RequestColumns.append(fl); }
    void addFilter(QString f){ RequestFilters.append(f); }
    void addFilters(QStringList fl){ RequestFilters.append(fl); }
    void addRawText(QString);
    void setLimit(int);
    QString getLiveStatusRequest();

private:
    QString prepareColumns();
    QString prepareFilters();
};

#endif // LIVESOCKETREQUEST_H
