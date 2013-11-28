#ifndef LIVESOCKETCOMMAND_H
#define LIVESOCKETCOMMAND_H

#include <QObject>
#include <QDebug>
#include <QStringList>
#include <QString>
#include <QDateTime>

#include "globals.h"

class LiveSocketCommand
{
public:
    LiveSocketCommand();

    QString commandType;
        void setCommandType(QString c){ commandType = c; }
    QString host_name;
        void setHostName(QString c){ host_name=c; }
    QString service_desription;
        void setServiceDescription(QString c){ service_desription=c; }
    int check_time;
        void setCheckTime(int t){ check_time=t; }
    int start_time;
        void setStartTime(int t){ start_time=t; }
    int end_time;
        void setEndTime(int t){ end_time=t; }
    int fixed;
        void setFixed(int t){ fixed=t; }
    int trigger_id;
        void setTriggerId(int t){ trigger_id=t; }
    int duration;
        void setDuration(int t){ duration=t; }
    QString author;
        void setAuthor(QString c){ author=c; }
    QString comment;
        void setComment(QString c){ comment=c; }
    int sticky;
        void setSticky(int t){ sticky=t; }
    int notify;
        void setNotify(int t){ notify=t; }
    int persistent;
        void setPersistant(int t){ persistent=t; }
    QString AuthUser;
        void setAuthUser(QString c){ AuthUser=c; }
    QString AuthPassword;
        void setAuthPassword(QString c){ AuthPassword=c; }

    QString getLiveSocketCommand();

private:
};


#endif // LIVESOCKETCOMMAND_H
