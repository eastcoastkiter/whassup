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
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QByteArray>
#include <QNetworkReply>
#include <QDateTime>
#include <QDateTimeEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QPixmap>
#include <QSlider>
#include <QJsonArray>
#include <QTreeWidgetItem>
#include <QVariant>
#include <QSettings>

#include "icons.h"
#include "commandwidget.h"
#include "globals.h"
#include "servicetreeitem.h"

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
    QString hostname;
    QString lastCmdTyp;
    QList<QStringList> selectedServices;
    QStringList lastServices;
    QStringList currentServices;
    bool foundIncidents;
    int sortOrder;
    int sortIndicator;
private slots:
    void showServicePopup(const QPoint &);
    void menuRightSlot(bool);
    void compareIncidents(QString, QString);
    void sortIndicatorChanged(int,Qt::SortOrder);
public slots:
    void setHostname(QString h) {hostname=h;}
    void setHiddenColumns(int);
    void gotServiceDataJson(QJsonArray);
    void receivedStatusBar(QString message, int status){ emit setStatusBar(message, status); }
    void writeSettings(QSettings*);
    void loadSettings(QSettings*);
    void clearTree(){ lastServices.clear(); clear(); }
    void slotSendLiveStatusData(QString a, QString b){ emit sendLiveStatusData(a,b); }
signals:
    void setStatusBar(QString, int);
    void signalLogSearch(QString);
    void signalHostDetail(QString);
    void signalServiceDetail(QString, QString);
    void sendLiveStatusData(QString, QString);
    void signalSetTraySystemIcon(int);
    void changeTimer(bool);
    void foundIncident();
};
#endif


