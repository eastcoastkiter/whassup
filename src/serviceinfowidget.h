#ifndef SERVICEINFOWIDGET_H
#define SERVICEINFOWIDGET_H

#include <QWidget>
#include <QObject>
#include <QGridLayout>
#include <QLabel>
#include <QList>
#include <QPushButton>
#include <QDebug>
#include <QLineEdit>
#include <QDateTime>
#include <QTreeWidget>
#include <QHeaderView>
#include <QRegularExpression>
#include <QComboBox>
#include <QPushButton>
#include <QUrl>
#include <QDesktopServices>
#include <QJsonArray>
#include <QGraphicsView>
#include <QGraphicsScale>
#include <QGraphicsItem>
#include <QCloseEvent>
#include <QToolButton>

#include "icons.h"
#include "pcdialog.h"
#include "livesocketrequest.h"
#include "logviewitem.h"
#include "logview.h"
#include "resultlabel.h"
#include "commandwidget.h"

#include <math.h>

class ServiceInfoWidget: public QWidget
{
    Q_OBJECT

public:
   ServiceInfoWidget(QJsonArray jsonArray,QString TimeFormat = "HH:mm dd-MM-yy",int TimeZone = 1, QString ServiceURL = QString::null, int LogLines=500, int uniqObjectID=0, QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~ServiceInfoWidget();
private:
    QString SURL;
    QString Hostname;
    QString Description;
    int State;
    int State_Type;
    int ServiceDetails;
    QString Host_Address;
    int uid;
    QString timeFormat;
    int timeZone;
    QTreeWidget *logTree;
    QTreeWidgetItem* lastTopLevelItem;
    int logLines;
    int currentLimit;
    int lastTopLevelState;
    QComboBox* comboLogRows;
    QString ActionUrl;
    QString NotesUrl;
    LogView *logTimeViewer;
    QGraphicsScene *logTimeScene;
    QList<QTreeWidgetItem *> logItems;
    QList<QTreeWidgetItem *> logTopLevelItems;
    qint32 stopTime;
    qint32 startTime;
private slots:
    void browserUrlButtonPressed();
    void actionUrlButtonPressed();
    void notesUrlButtonPressed();
    void redrawScene();
public slots:
    int getUniqObjectID(){ return uid; }
    void requestLog();
    void gotLiveStatusJson(QJsonArray);
    void requestNewLog(int);
    void logTreeItemClicked(QPoint);
    void slotSendLiveStatusData(QString a, QString b){ emit sendLiveStatusData(a,b); }
signals:
    void requestLiveStatusData(int, QString, QString);
    void closing(QString);
    void sendLiveStatusData(QString, QString);
protected:
    void resizeEvent(QResizeEvent *);
    void closeEvent(QCloseEvent*);
    void keyPressEvent(QKeyEvent *);
};
#endif // SERVICEINFOWIDGET_H
