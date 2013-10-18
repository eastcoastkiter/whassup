#ifndef APPLICATION_H
#define APPLICATION_H

#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QHideEvent>
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
#include <QObject>
#include <QPushButton>
#include <QSpinBox>
#include <QStatusBar>
#include <QString>
#include <QTabWidget>
#include <QTimer>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QUrl>
#include <QWidget>
#include <QSystemTrayIcon>
#include <QPixmap>
#include <QBitmap>
#include <QPainter>


#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

#include "livesocket.h"
#include "icons.h"
#include "helpwidget.h"
#include "servicetree.h"
#include "infoWidget.h"
#include "filterwidget.h"

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
  //  void hideEvent(QHideEvent*);
private slots:
     void aboutslot();
     void gotServiceData(QList<QStringList> listOfStringLists);
     void gotDowntimeData(QList<QStringList> listOfStringLists);
     void gotLogData(QList<QStringList> listOfStringLists);
     void gotHostgroupData(QList<QStringList> listOfStringLists);
     void gotInventarData(QList<QStringList> listOfStringLists);
     void gotConfigData(QList<QStringList> listOfStringLists);
     void receiveLiveStatusData(QString, QList<QStringList>);
     void writeServiceSlot();
     void errorOccured(QString);
     void changeFilters(bool);
     void updateSlot();
     void configSubmitSlot();
     void aktualisierungChanged(int);
     void setTreeFont();
     void writeConfig();
     bool loadConfig(QString);
     void serviceItemDoubleClicked ( QTreeWidgetItem *, int);
     void inventarItemDoubleClicked ( QTreeWidgetItem *, int);
     void searchConfigFile();
     void newIconExplanation();
     void currentTabChanged(int);
     void findItem();
     void searchTrees();
     void inventarExpandSlot();
     void inventarUpdateSlot();
     void systemTrayActivated(QSystemTrayIcon::ActivationReason reason);
     QPixmap drawSystemTrayIcon(QString);
     void columnsStateChange(int);
     void setCheckBoxes(int);
     void msgBoxClosed();
     QString getFilter();
     void inputFilterButtonPressed();
     void gotNewFilter(QString);

public slots:
     void setStatusBar(QString message){statusBar()->showMessage( message);}
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
     //   QLineEdit* inputHostGroups;
        QSpinBox* inputPort;
        QSpinBox* inputAktualisierung;
        QSpinBox* inputTimezone;
        QLineEdit* inputTimeFormat;
        QSpinBox* inputLogTime;
        QLineEdit* inputFilter;
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
        QSystemTrayIcon* iconTray;
        QTreeWidgetItem* lastHostgroupRightClickedItem;
        QCheckBox *cb_Last_Change;
        QCheckBox *cb_State_Type;
        QCheckBox *cb_Downtimes;
        QCheckBox *cb_Downtime_Depth;
        QCheckBox *cb_host_state;
        QCheckBox *cb_host_state_type;
        QCheckBox *cb_host_downtimes;
        QCheckBox *cb_last_check;
        int globalColumnInt;
        QLabel* hostGroupLabel;
        QString showWarnings;
        QCheckBox *cb_showWarnings;
        QHideEvent *she;
        FilterWidget * filterWidget;
        QStringList configServiceList;
        QStringList configHostList;
        QStringList configHostgroupList;
        bool hideInputFilter;
};
#endif
