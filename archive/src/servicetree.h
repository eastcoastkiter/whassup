#ifndef SERVICETREE_H
#define SERVICETREE_H

#include <QTreeWidget>
#include <QObject>
#include <QHeaderView>
#include <QStringList>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QUrl>
#include <QNetworkRequest>
#include <QByteArray>
#include <QNetworkReply>
#include <QDateTime>
#include <QDateTimeEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QPixmap>

#include "icons.h"

#define CMD_ACKNOWLEDGE_SVC_PROBLEM "34"
#define CMD_SCHEDULE_SVC_DOWNTIME "56"
#define CMD_SCHEDULE_SVC_CHECK "7"
#define CMD_SCHEDULE_HOST_SVC_DOWNTIME "86"

class ServiceTree: public QTreeWidget
{
	Q_OBJECT
	
public:
    ServiceTree(QWidget * parent = 0);
    ~ServiceTree();

private:
    QString storUsername;
    QString storPassword;
    QTreeWidgetItem * lastRightClickedItem;
    QString nagioshostname;
    int timezone;
    QNetworkReply * reply;
    QWidget* menuWidget;
    QString lastCmdTyp;
        QLineEdit* ackHostname;
        QLineEdit* ackService;
        QLineEdit* ackAuthor;
        QLineEdit* ackComment;
        QLineEdit* ackPassword;
        QLineEdit* ackDuration;
        QDateTimeEdit* ackStart;
        QSpinBox* ackMinuteDuration;
        QSpinBox* ackHourDuration;
        QCheckBox* ackAllServices;


private slots:
    void showServicePopup(const QPoint &);
    void menuRightSlot(bool);
    void ackSubmitSlot();
    void sendNagiosHttp(QNetworkRequest, QByteArray);
         void slotReadyReadHttp();
public slots:
    void setNagiosHostname(QString h) {nagioshostname=h;}
    void setTimezone(int t) {timezone=t;}
signals:
    void setStatusBar(QString);
};
#endif

