#ifndef APPLICATION_H
#define APPLICATION_H


#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QCompleter>
#include <QDateTime>
#include <QDebug>
#include <QDesktopServices>
#include <QEvent>
#include <QFile>
#include <QFileDialog>
#include <QFont>
#include <QFontDialog>
#include <QHeaderView>
#include <QKeySequence>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenuBar>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QPainter>
#include <QPushButton>
#include <QSpinBox>
#include <QStatusBar>
#include <QString>
#include <QSystemTrayIcon>
#include <QTabWidget>
#include <QTimer>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QUrl>
#include <QWidget>
#include <QJsonArray>
#include <QSlider>
#include <QDialog>
#include <QSettings>
#include <QStatusBar>
#include <QComboBox>
#include <QDesktopWidget>

class ServiceInfoWidget;
class HostInfoWidget;

#include "commandwidget.h"
#include "livesocket.h"
#include "livesocketssl.h"
#include "icons.h"
#include "helpwidget.h"
#include "servicetree.h"
#include "hostinfowidget.h"
#include "serviceinfowidget.h"
#include "livesocketrequest.h"
#include "generationtree.h"
#include "configwidget.h"
#include "statusbar.h"
#include "helpwidget.h"
#include "webcommunication.h"



class Whassup: public QMainWindow
{
    Q_OBJECT

public:
    Whassup();
    ~Whassup();

private:
        ServiceTree* serviceTree;
        QTreeWidget* hostgroupTree;
        QTreeWidget* downtimeTree;
        QTreeWidget* logTree;
        QTreeWidget* inventarTree;
        GenerationTreeWidget* generationTreeWidget;
        ConfigWidget* configWidget;
        QWidget *window;
        QTimer *timer;
        bool hide, aufbau;
        QTabWidget* TabWidget;
        QWidget* tabService;
        QWidget* tabDowntime;
        QWidget* tabHostgroup;
        QWidget* tabLog;
        QWidget* tabInventar;
        QWidget* tabConfig;
        QWidget* tabGeneration;
        QFileDialog* configFile;
        QAction* okAction;
        QAction* warnAction;
        QAction* critAction;
        QAction* ackAction;
        QAction* notAction;
        QAction* downAction;
        QAction* stateAction;
        QString Filter;
        QWidget* findWidget;
        QLineEdit* inputFind;
        QString findString;
        QPushButton* inventarExpandButton;
        QString configLocation;
        QSystemTrayIcon* iconTray;
        QAction* exitAction;
        QLineEdit* searchLogHost;
        int requestCounter;
        int uniqObjectID;
        QList<HostInfoWidget*> hostInfoWidgetList;
        QList<ServiceInfoWidget*> serviceInfoWidgetList;
        QLineEdit* searchHostgroupHost;
        QPushButton* searchHostgroupHostButton;
        QList<QTreeWidgetItem *> hostgroupFoundList;
        StatusBar * statusBar;
        int lastBadStates;
        bool blinkInterator;
        QTimer *blinkTimer;
        bool haserror;

protected:
    void closeEvent(QCloseEvent*);
    void changeEvent(QEvent *);
private slots:
     void aboutslot();
     void gotDowntimeDataJson(QJsonArray);
     void gotLogDataJson(QJsonArray);
     void gotHostgroupDataJson(QJsonArray);
     void gotInventarDataJson(QJsonArray);
     void receiveLiveStatusJSONArray(QString,QJsonArray);
     void writeServiceSlot();
     void changeFilters(bool);
     void updateSlot();
     void configSubmitSlot();
     void aktualisierungChanged(int);
     void serviceItemDoubleClicked ( QTreeWidgetItem *, int);
     void downtimeItemDoubleClicked ( QTreeWidgetItem *, int);
     void inventarItemDoubleClicked ( QTreeWidgetItem *, int);
     void hostgroupItemDoubleClicked ( QTreeWidgetItem *, int);
     void logItemDoubleClicked ( QTreeWidgetItem *, int);
     void searchConfigFile();
     void newIconExplanation();
     void currentTabChanged(int);
     void currentTabChangedByName(QString);
     void findItem();
     void searchTrees();
     void inventarExpandSlot();
     void inventarUpdateSlot();
     void systemTrayActivated(QSystemTrayIcon::ActivationReason reason);
     QPixmap drawSystemTrayIcon(QString);
     void logUpdateSlot();
     void logSearch(QString);
     //void sendLiveStatusData(QString, QString, QString, int, QString);
     void sendLiveStatusData(QString, QString);
     void requestHostDetail(QString);
     void requestServiceDetail(QString, QString);
     void childRequestLiveStatusData(int,QString, QString);
     void searchHostgroup();
     void searchHostgroupHostChanged(QString);
     void writeSettings(QSettings *);
     void loadSettings(QSettings *);
     void foundIncident();
     void blinkTrayIcon();
     void writeCleanServiceSlot();
     void authEntered(){ writeCleanServiceSlot(); }
public slots:
     void setStatusBarSlot(QString message, int status){ statusBar->setMessage(message, status); }
     void errorOccured(QString,int);
    // We need that for Livestatus Request of child objects
     int getUniqObjectID(){ uniqObjectID++; return uniqObjectID; }
     void setTraySystemIcon(int);
     void setServiceFilters(int);
     void getServiceFilters();
     QString getServerAuthName(){ return configWidget->getAuthUser(); }
     QString getServerAuthPassword(){ return configWidget->getAuthPassword(); }
     void changeTimer(bool t){ if ( (t) && (!timer->isActive()) ) timer->start(); else timer->stop(); }
};
#endif
