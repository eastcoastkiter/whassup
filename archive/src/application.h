#ifndef APPLICATION_H
#define APPLICATION_H

#include <QAction>
#include <QApplication>
#include <QCloseEvent>
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

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

#include "livesocket.h"
#include "icons.h"
#include "helpwidget.h"
#include "servicetree.h"
#include "infoWidget.h"

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
        QSpinBox* inputPort;
        QSpinBox* inputAktualisierung;
        QSpinBox* inputTimezone;
        QSpinBox* inputLogTime;
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


};
#endif
