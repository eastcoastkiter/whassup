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


#include "livesocket.h"
#include "icons.h"
#include "helpwidget.h"
#include "servicetree.h"
#include "infoWidget.h"
#include "filterwidget.h"
#include "hostinfowidget.h"
#include "livesocketrequest.h"


/*
 TODO:
  - menuBar()->setVisible(false);
  - Ansicht aufraeumen
  - infoWidged zentrieren mapToGlobal
*/

class ApplicationWindow: public QMainWindow
{
    Q_OBJECT

public:
    ApplicationWindow();
    ~ApplicationWindow();


        QString HostName;
        int Port;
protected:
    void closeEvent(QCloseEvent*);

private slots:
     void aboutslot();
     void gotServiceData(QList<QStringList> listOfStringLists);
     void gotDowntimeData(QList<QStringList> listOfStringLists);
     void gotLogData(QList<QStringList> listOfStringLists);
     void gotHostgroupData(QList<QStringList> listOfStringLists);
     void gotInventarData(QList<QStringList> listOfStringLists);
     void receiveLiveStatusData(QString, QList<QStringList>);
     void receiveLiveStatusJSONArray(QString,QJsonArray);
     void writeServiceSlot();
     void changeFilters(bool);
     void updateSlot();
     void configSubmitSlot();
     void aktualisierungChanged(int);
     void setTreeFont();
     void writeConfig();
     bool loadConfig(QString);
     void serviceItemDoubleClicked ( QTreeWidgetItem *, int);
     void downtimeItemDoubleClicked ( QTreeWidgetItem *, int);
     void inventarItemDoubleClicked ( QTreeWidgetItem *, int);
     void hostgroupItemDoubleClicked ( QTreeWidgetItem *, int);
     void logItemDoubleClicked ( QTreeWidgetItem *, int);
     void searchConfigFile();
     void newIconExplanation();
     void currentTabChanged(int);
     void findItem();
     void searchTrees();
     void inventarExpandSlot();
     void inventarUpdateSlot();
     void inputFilterButtonPressed();
     void gotConfigData(QList<QStringList> listOfStringLists);
     void gotNewFilter(QString);
     QString getFilter();
     void setCheckBoxes(int);
     void columnsStateChange(int);
     void systemTrayActivated(QSystemTrayIcon::ActivationReason reason);
     QPixmap drawSystemTrayIcon(QString);
     void logUpdateSlot();
     void logSearch(QString);
     void sendLiveStatusData(QString, QString, QString, int, QString);
     void requestHostDetail(QString);
     void childRequestLiveStatusData(int, QString);

public slots:
     void setStatusBar(QString message){statusBar()->showMessage( message);}
     void errorOccured(QString);
     QString getInputTimeFormat(){ return inputTimeFormat->text(); }
     int getInputTimeZone(){ return inputTimezone->value(); }
     QString getInputServiceURL(){ return inputServiceURL->text(); }
    // We need that for Livestatus Request of child objects
     int getUniqObjectID(){ uniqObjectID++; return uniqObjectID; }
private:
        ServiceTree* serviceTree;
        QTreeWidget* hostgroupTree;
        QTreeWidget* downtimeTree;
        QTreeWidget* logTree;
        QTreeWidget* inventarTree;
        QWidget *window;
        QLineEdit* inputHostname;
        QLineEdit* inputServiceURL;
        QLineEdit* inputConfigStorage;
        QLineEdit* inputConfigFont;
        QSpinBox* inputPort;
        QSpinBox* inputAktualisierung;
        QSpinBox* inputTimezone;
        QSpinBox* inputLogLines;
        QTimer *timer;
        bool hide, aufbau;
        QTabWidget* TabWidget;
        QWidget* tabService;
        QWidget* tabDowntime;
        QWidget* tabHostgroup;
        QWidget* tabLog;
        QWidget* tabInventar;
        QWidget* tabConfig;
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
        InfoWidget* iWidget;
        QPushButton* inventarExpandButton;
        QLineEdit* inputFilter;
        QStringList configServiceList;
        QStringList configHostList;
        QStringList configHostgroupList;
        bool hideInputFilter;
        FilterWidget * filterWidget;
        int globalColumnInt;
        QLineEdit* inputTimeFormat;
 //       QString showWarnings;
        QCheckBox *cb_Last_Change;
        QCheckBox *cb_State_Type;
        QCheckBox *cb_Downtimes;
        QCheckBox *cb_Downtime_Depth;
        QCheckBox *cb_host_state;
        QCheckBox *cb_host_state_type;
        QCheckBox *cb_host_downtimes;
        QCheckBox *cb_last_check;
        QString configLocation;
        QSystemTrayIcon* iconTray;
        QAction* exitAction;
        QLineEdit* searchLogHost;
        int requestCounter;

        int uniqObjectID;
        QList<HostInfoWidget*> hostInfoWidgetList;
};
#endif
