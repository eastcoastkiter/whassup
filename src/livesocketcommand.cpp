#include "livesocketcommand.h"

LiveSocketCommand::LiveSocketCommand()
{
    QDateTime dt = QDateTime::currentDateTime ();

    host_name=QString::null;
    service_desription=QString::null;
    check_time=dt.toTime_t();
    start_time=dt.toTime_t();
    end_time=dt.toTime_t();
    fixed=1;
    trigger_id=0;
    duration=0;
    author=QString::null;
    comment=QString::null;
    sticky=2;
    notify=0;
    persistent=1;
    AuthUser=QString::null;
    AuthPassword=QString::null;
}


QString LiveSocketCommand::getLiveSocketCommand()
{
QString socketCommand;

socketCommand = "COMMAND";
QDateTime dt = QDateTime::currentDateTime ();
socketCommand+=QString(" [%1] ").arg(dt.toTime_t());

if (commandType == CMD_SCHEDULE_FORCED_SVC_CHECK)
    socketCommand+="SCHEDULE_FORCED_SVC_CHECK";
else if (commandType == CMD_SCHEDULE_FORCED_HOST_SVC_CHECKS)
    socketCommand+="SCHEDULE_FORCED_HOST_SVC_CHECKS";
else if (commandType == CMD_ACKNOWLEDGE_HOST_PROBLEM)
    socketCommand+="ACKNOWLEDGE_HOST_PROBLEM";
else if (commandType == CMD_ACKNOWLEDGE_SVC_PROBLEM)
    socketCommand+="ACKNOWLEDGE_SVC_PROBLEM";
else if (commandType == CMD_SCHEDULE_HOST_DOWNTIME)
    socketCommand+="SCHEDULE_HOST_DOWNTIME";
else if (commandType == CMD_SCHEDULE_SVC_DOWNTIME)
    socketCommand+="SCHEDULE_SVC_DOWNTIME";
else if (commandType == CMD_SCHEDULE_HOST_SVC_DOWNTIME)
    socketCommand+="SCHEDULE_HOST_SVC_DOWNTIME";
else if (commandType == CMD_SCHEDULE_FORCED_HOST_SVC_CHECKS)
    socketCommand+="SCHEDULE_FORCED_HOST_SVC_CHECKS";
else if (commandType == CMD_SCHEDULE_HOST_SVC_DOWNTIME)
    socketCommand+="SCHEDULE_HOST_SVC_DOWNTIME";
else if (commandType == CMD_ENABLE_HOST_CHECK)
    socketCommand+="ENABLE_HOST_CHECK";
else if (commandType == CMD_DISABLE_HOST_CHECK)
    socketCommand+="DISABLE_HOST_CHECK";
else if (commandType == CMD_ENABLE_SVC_CHECK)
    socketCommand+="ENABLE_SVC_CHECK";
else if (commandType == CMD_DISABLE_SVC_CHECK)
    socketCommand+="DISABLE_SVC_CHECK";
else if (commandType == CMD_ENABLE_HOST_NOTIFICATIONS)
    socketCommand+="ENABLE_HOST_NOTIFICATIONS";
else if (commandType == CMD_DISABLE_HOST_NOTIFICATIONS)
    socketCommand+="DISABLE_HOST_NOTIFICATIONS";
else if (commandType == CMD_ENABLE_SVC_NOTIFICATIONS)
    socketCommand+="ENABLE_SVC_NOTIFICATIONS";
else if (commandType == CMD_DISABLE_SVC_NOTIFICATIONS)
    socketCommand+="DISABLE_SVC_NOTIFICATIONS";
else
    return "Error"; // Add Errors later

    qDebug() << "commandType" << commandType << socketCommand ;

    socketCommand += ";";
    socketCommand += host_name;
    socketCommand += ";";

    if ( (commandType == CMD_SCHEDULE_FORCED_SVC_CHECK) ||  (commandType == CMD_ACKNOWLEDGE_SVC_PROBLEM) || (commandType == CMD_SCHEDULE_SVC_DOWNTIME) || (commandType == CMD_ENABLE_SVC_CHECK)|| (commandType == CMD_DISABLE_SVC_CHECK)|| (commandType == CMD_ENABLE_SVC_NOTIFICATIONS)|| (commandType == CMD_DISABLE_SVC_NOTIFICATIONS) )
    {
         socketCommand += service_desription;
         socketCommand += ";";
    }

    if ( (commandType == CMD_SCHEDULE_FORCED_SVC_CHECK) || (commandType == CMD_SCHEDULE_FORCED_HOST_SVC_CHECKS) )
     {
         socketCommand += QString("%1").arg(check_time);
     }

     if ( (commandType == CMD_ACKNOWLEDGE_HOST_PROBLEM) || (commandType == CMD_ACKNOWLEDGE_SVC_PROBLEM) )
     {
         socketCommand += QString("%1").arg(sticky);
         socketCommand += ";";
         socketCommand += QString("%1").arg(notify);
         socketCommand += ";";
         socketCommand += QString("%1").arg(persistent);
         socketCommand += ";";
     }

     if ( (commandType == CMD_SCHEDULE_HOST_DOWNTIME) || (commandType == CMD_SCHEDULE_SVC_DOWNTIME) || (commandType == CMD_SCHEDULE_HOST_SVC_DOWNTIME))
     {
         socketCommand += QString("%1").arg(start_time);
         socketCommand += ";";
         socketCommand += QString("%1").arg(end_time);
         socketCommand += ";";
         socketCommand += QString("%1").arg(fixed);
         socketCommand += ";";
         socketCommand += QString("%1").arg(trigger_id);
         socketCommand += ";";
         socketCommand += QString("%1").arg(duration);
         socketCommand += ";";
     }
     if ( (commandType == CMD_ACKNOWLEDGE_HOST_PROBLEM) || (commandType == CMD_ACKNOWLEDGE_SVC_PROBLEM) || (commandType == CMD_SCHEDULE_HOST_DOWNTIME) || (commandType == CMD_SCHEDULE_SVC_DOWNTIME) || (commandType == CMD_SCHEDULE_HOST_SVC_DOWNTIME) )
     {
         socketCommand += author;
         socketCommand += ";";
         socketCommand += comment;
     }
     // remove bad comments or programming scrab ;o)
     socketCommand=socketCommand.trimmed();
     socketCommand+="\n\n";

    return socketCommand;
}
