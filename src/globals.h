#ifndef GLOBALS_H
#define GLOBALS_H


#define CMD_SCHEDULE_FORCED_SVC_CHECK "54"
//SCHEDULE_FORCED_SVC_CHECK;<host_name>;<service_description>;<check_time>
#define CMD_ACKNOWLEDGE_HOST_PROBLEM "33"
//ACKNOWLEDGE_HOST_PROBLEM;<host_name>;<sticky>;<notify>;<persistent>;<author>;<comment>
#define CMD_ACKNOWLEDGE_SVC_PROBLEM "34"
//ACKNOWLEDGE_SVC_PROBLEM;<host_name>;<service_description>;<sticky>;<notify>;<persistent>;<author>;<comment>
#define CMD_SCHEDULE_FORCED_HOST_SVC_CHECKS "53"
//SCHEDULE_FORCED_HOST_SVC_CHECKS;<host_name>;<check_time>
#define CMD_SCHEDULE_HOST_DOWNTIME "55"
//SCHEDULE_HOST_DOWNTIME;<host_name>;<start_time>;<end_time>;<fixed>;<trigger_id>;<duration>;<author>;<comment>
#define CMD_SCHEDULE_SVC_DOWNTIME "56"
//SCHEDULE_SVC_DOWNTIME;<host_name>;<service_desription><start_time>;<end_time>;<fixed>;<trigger_id>;<duration>;<author>;<comment>
#define CMD_SCHEDULE_HOST_SVC_DOWNTIME "86"
//SCHEDULE_HOST_SVC_DOWNTIME;<host_name>;<start_time>;<end_time>;<fixed>;<trigger_id>;<duration>;<author>;<comment>
#define CMD_SCHEDULE_FORCED_HOST_CHECK "98"

#define CMD_ENABLE_HOST_CHECK "47"
#define CMD_DISABLE_HOST_CHECK "48"
//DISABLE_HOST_CHECK;<host_name>

#define CMD_ENABLE_SVC_CHECK "5"
#define CMD_DISABLE_SVC_CHECK "6"
//DISABLE_SVC_CHECK;<host_name>;<service_description>

#define CMD_ENABLE_HOST_NOTIFICATIONS "24"
#define CMD_DISABLE_HOST_NOTIFICATIONS "25"
//DISABLE_HOST_NOTIFICATIONS;<host_name>

#define CMD_ENABLE_SVC_NOTIFICATIONS "22"
#define CMD_DISABLE_SVC_NOTIFICATIONS "23"
//DISABLE_SVC_NOTIFICATIONS;<host_name>;<service_description>
#endif // GLOBALS_H
