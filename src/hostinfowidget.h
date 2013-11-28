#ifndef HOSTINFOWIDGET_H
#define HOSTINFOWIDGET_H

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

#include "icons.h"
#include "pcdialog.h"
#include "livesocketrequest.h"
#include "resultlabel.h"
#include "commandwidget.h"


class HostInfoWidget: public QWidget
{
	Q_OBJECT
	
public:
   HostInfoWidget(QJsonArray jsonArray,QString TimeFormat = "HH:mm dd-MM-yy",int TimeZone = 1, QString ServiceURL = QString::null, int uniqObjectID=0, QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~HostInfoWidget();
private:
    QString SURL;
    QString Hostname;
    int State;
    int State_Type;
    QString ActionUrl;
    QString NotesUrl;
    PCDialog *pcDialog;
    int uid;
    QStringList parentList;
    int HostDetails;
private slots:
    void browserUrlButtonPressed();
    void actionUrlButtonPressed();
    void notesUrlButtonPressed();
    void requestParents();
    void logButtonPressed(){ emit logSearch(Hostname); close(); }
public slots:
    void slotDoubleClickedHost(QString Hostname){ emit doubleClickedHost(Hostname); }
    int getUniqObjectID(){ return uid; }
    void gotLiveStatusJson(QJsonArray);
    void serviceItemDoubleClicked ( QTreeWidgetItem *, int);
    void slotSendLiveStatusData(QString a, QString b){ emit sendLiveStatusData(a,b); }
signals:
    void doubleClickedHost(QString);
    void requestLiveStatusData(int, QString, QString);
    void logSearch(QString);
    void doubleClickedService(QString, QString);
    void sendLiveStatusData(QString, QString);
protected:
    void keyPressEvent(QKeyEvent *);
};
#endif

