#ifndef LIVESOCKETREQUEST_H
#define LIVESOCKETREQUEST_H

#include <QObject>
#include <QDebug>
#include <QStringList>

#define CMD_SCHEDULE_SVC_CHECK "7"
#define CMD_ACKNOWLEDGE_HOST_PROBLEM "33"
#define CMD_ACKNOWLEDGE_SVC_PROBLEM "34"
#define CMD_SCHEDULE_HOST_DOWNTIME "55"
#define CMD_SCHEDULE_SVC_DOWNTIME "56"
#define CMD_SCHEDULE_HOST_SVC_DOWNTIME "86"


/*
LiveSocketRequest ls;
ls.setTable("hosts");
ls.addColumns(QStringList() << "host_name" << "alias");
ls.addColumn("address");
ls.setLimit(1);
ls.addFilter("Filter: host_name = abc");
ls.addFilters(QStringList() << "Filter: host_name = def" << "Filter: host_name = gfh");
ls.addFilter("Or: 3");
ls.addRawText("Filter: description = 1\nFilter: description = 2\nAnd: 2\n");
QString ws=ls.getLiveStatusRequest();
qDebug() << "LS String" << ws;
*/

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
    QString AuthUser;

    void setCommand(QString c){ RequestType = c; }
    void setOutputFormat(QString);
    void setTable(QString t){ Table = t; }
    void addColumn(QString f){ RequestColumns.append(f); }
    void addColumns(QStringList fl){ RequestColumns.append(fl); }
    void addFilter(QString f){ RequestFilters.append(f); }
    void addFilters(QStringList fl){ RequestFilters.append(fl); }
    void addRawText(QString);
    void setAuthUser(QString au){ AuthUser=au; }
    void setLimit(int);
    QString getLiveStatusRequest();

private:
    QString prepareColumns();
    QString prepareFilters();
};

#endif // LIVESOCKETREQUEST_H
