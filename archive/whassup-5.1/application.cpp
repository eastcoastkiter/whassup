#include "application.h"
using namespace std;

ApplicationWindow::ApplicationWindow()
	: QMainWindow( 0 )
{
uniqObjectID=0;
hide=false;
aufbau=false;
hideInputFilter=false;
requestCounter=0;

setWindowTitle("Whassup");
configLocation = QString(QStandardPaths::locate(QStandardPaths::HomeLocation, QString(), QStandardPaths::LocateDirectory) + "whassup.cfg");
setAccessibleName("application");
qApp->setQuitOnLastWindowClosed(false);

//Tray Icon
iconTray = new QSystemTrayIcon;
QPixmap mwIcon(":/ok.png");
    iconTray->setIcon(mwIcon);


    //iconTray->setIcon(QPixmap(ok_xpm));
    iconTray->setVisible(true);
    iconTray->show();
    connect(iconTray, SIGNAL(activated ( QSystemTrayIcon::ActivationReason )), this, SLOT(systemTrayActivated(QSystemTrayIcon::ActivationReason)));


//main widget
window = new QWidget(this);
  //      setCentralWidget(window);
window->setAccessibleName("window");
        resize(800,400);
        statusBar()->showMessage( "Pease submit your configuration" );

        // Prime TabWigdet provides space for tabs
        TabWidget = new QTabWidget( window );

        TabWidget->setSizePolicy ( QSizePolicy::Expanding, QSizePolicy::Expanding);
            connect(TabWidget, SIGNAL ( currentChanged( int )), this, SLOT(currentTabChanged(int)));
        setCentralWidget(TabWidget);
            tabService = new QWidget( TabWidget );
                TabWidget->insertTab( 0, tabService, QString ("Services") );
            tabDowntime = new QWidget( TabWidget );
                TabWidget->insertTab( 1, tabDowntime, ("Downtimes") );
            tabLog = new QWidget( TabWidget );
                TabWidget->insertTab( 2, tabLog, ("Log") );
            tabHostgroup = new QWidget( TabWidget );
                    TabWidget->insertTab( 3, tabHostgroup, ("Hostgroups") );
            tabInventar = new QWidget( TabWidget );
                TabWidget->insertTab( 4, tabInventar, ("Inventar") );
            tabConfig = new QWidget( TabWidget );
                TabWidget->insertTab( 5, tabConfig, ("Config") );

// Popup for file stuff



        QMenu* fileMenu = menuBar()->addMenu(tr("&Datei"));

        QAction* updateAction   = new QAction("&Update", window);
        updateAction->setShortcut(Qt::Key_Space);
            connect(updateAction, SIGNAL(triggered(bool)), this, SLOT(writeServiceSlot()));
        fileMenu->addAction(updateAction);

        QAction* findAction   = new QAction("&Find", window);
        findAction->setShortcut(Qt::CTRL + Qt::Key_F);
            connect(findAction, SIGNAL(triggered(bool)), this, SLOT(findItem()));
        fileMenu->addAction(findAction);

        fileMenu->addSeparator ();

        exitAction = new QAction("&Exit", window);
            exitAction->setShortcut(Qt::CTRL + Qt::Key_X);
            connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
            fileMenu->addAction(exitAction);


        QMenu* viewMenu = menuBar()->addMenu(tr("&Ansicht"));

        okAction   = new QAction("&Ok", window);
            okAction->setObjectName("okAction");
            okAction->setCheckable ( true );
            okAction->setChecked ( false );
            okAction->setShortcut(Qt::CTRL + Qt::Key_O);
                connect(okAction, SIGNAL(triggered(bool)), this, SLOT(changeFilters(bool)));
        viewMenu->addAction(okAction);
        okAction->setVisible(false);

        warnAction   = new QAction("&Warning", window);
            warnAction->setObjectName("warnAction");
            warnAction->setCheckable ( true );
            warnAction->setChecked ( true );
            warnAction->setShortcut(Qt::CTRL + Qt::Key_W);
                connect(warnAction, SIGNAL(triggered(bool)), this, SLOT(changeFilters(bool)));
        viewMenu->addAction(warnAction);

        critAction   = new QAction("&Critical", window);
            critAction->setObjectName("critAction");
            critAction->setCheckable ( true );
            critAction->setChecked ( true );
            critAction->setShortcut(Qt::CTRL + Qt::Key_C);
                connect(critAction, SIGNAL(triggered(bool)), this, SLOT(changeFilters(bool)));
        viewMenu->addAction(critAction);

        viewMenu->addSeparator ();

        ackAction   = new QAction("&Acknowledged", window);
            ackAction->setObjectName("ackAction");
            ackAction->setCheckable ( true );
            ackAction->setChecked ( true );
            ackAction->setShortcut(Qt::CTRL + Qt::Key_A);
                connect(ackAction, SIGNAL(triggered(bool)), this, SLOT(changeFilters(bool)));
        viewMenu->addAction(ackAction);

        notAction   = new QAction("&Notifications", window);
            notAction->setObjectName("notAction");
            notAction->setCheckable ( true );
            notAction->setChecked ( true );
            notAction->setShortcut(Qt::CTRL + Qt::Key_N);
                connect(notAction, SIGNAL(triggered(bool)), this, SLOT(changeFilters(bool)));
        viewMenu->addAction(notAction);

        downAction   = new QAction("&Downtime", window);
            downAction->setObjectName("downAction");
            downAction->setCheckable ( true );
            downAction->setChecked ( false );
            downAction->setShortcut(Qt::CTRL + Qt::Key_S);
                connect(downAction, SIGNAL(triggered(bool)), this, SLOT(changeFilters(bool)));
        viewMenu->addAction(downAction);

        stateAction   = new QAction("&Hardstate", window);
            stateAction->setObjectName("stateAction");
            stateAction->setCheckable ( true );
            stateAction->setChecked ( false );
            stateAction->setShortcut(Qt::CTRL + Qt::Key_S);
                connect(stateAction, SIGNAL(triggered(bool)), this, SLOT(changeFilters(bool)));
        viewMenu->addAction(stateAction);

        QMenu* aboutMenu = menuBar()->addMenu(tr("&?"));
            QAction* iconAction   = new QAction("&Icons", window);
            connect(iconAction, SIGNAL(triggered()), this, SLOT(newIconExplanation()));
        aboutMenu->addAction(iconAction);

            QAction* aboutAction   = new QAction("&About", window);
            connect(aboutAction, SIGNAL(triggered()), this, SLOT(aboutslot()));
        aboutMenu->addAction(aboutAction);
// End of Menu

//Downtime
    QGridLayout* serviceGrid = new QGridLayout(tabService);
    serviceTree = new ServiceTree(tabService);
        connect(serviceTree, SIGNAL(itemDoubleClicked ( QTreeWidgetItem *, int )), this, SLOT(serviceItemDoubleClicked ( QTreeWidgetItem *, int)));
        connect(serviceTree, SIGNAL(setStatusBar(QString)), this, SLOT(setStatusBar(QString)));
        connect(serviceTree, SIGNAL(signalLogSearch(QString)), this, SLOT(logSearch(QString)));
        connect(serviceTree, SIGNAL(signalHostDetail(QString)), this, SLOT(requestHostDetail(QString)));

    serviceGrid->addWidget(serviceTree,0,0);

     //Downtime
    QGridLayout* downtimeGrid = new QGridLayout(tabDowntime);
    downtimeTree = new QTreeWidget(tabDowntime);
          QStringList headerLabelsDowntime;
        downtimeTree->setColumnCount(6);
        downtimeTree->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
        connect(downtimeTree, SIGNAL(itemDoubleClicked ( QTreeWidgetItem *, int )), this, SLOT(downtimeItemDoubleClicked ( QTreeWidgetItem *, int)));
        headerLabelsDowntime << "Hostname" <<  "Service" << "Start" << "End" << "Author" << "Comment";
            downtimeTree->setHeaderLabels(headerLabelsDowntime);
        downtimeTree->setSortingEnabled (true);
    downtimeGrid->addWidget(downtimeTree,0,0);

    //Hostgroup
    QGridLayout* hostgroupGrid = new QGridLayout(tabHostgroup);
    hostgroupTree = new QTreeWidget(tabHostgroup);
          QStringList headerLabelsHostgroup;
        hostgroupTree->setColumnCount(4);
        hostgroupTree->setContextMenuPolicy(Qt::CustomContextMenu);
        hostgroupTree->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
        connect(hostgroupTree, SIGNAL(itemDoubleClicked ( QTreeWidgetItem *, int )), this, SLOT(hostgroupItemDoubleClicked ( QTreeWidgetItem *, int)));
        headerLabelsHostgroup << "Group" << "Hosts" << "Count" <<  "Down";
            hostgroupTree->setHeaderLabels(headerLabelsHostgroup);
        hostgroupTree->setSortingEnabled (true);
    hostgroupGrid->addWidget(hostgroupTree,0,0);



    //LOG
    QGridLayout* logGrid = new QGridLayout(tabLog);
    logTree = new QTreeWidget(tabLog);
          QStringList headerLabelsLog;
        logTree->setColumnCount(5);
        logTree->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
        connect(logTree, SIGNAL(itemDoubleClicked ( QTreeWidgetItem *, int )), this, SLOT(logItemDoubleClicked ( QTreeWidgetItem *, int)));
        headerLabelsLog << "time" << "state" << "hostname" <<  "service" <<  "message";
            logTree->setHeaderLabels(headerLabelsLog);
        logTree->setSortingEnabled (false);
    logGrid->addWidget(logTree,0,0,1,4);

    logGrid->addWidget(new QLabel("Host:"),1,0);

    searchLogHost = new QLineEdit(tabInventar);
        connect( searchLogHost, SIGNAL( returnPressed()) , this, SLOT( logUpdateSlot() ) );
        logGrid->addWidget(searchLogHost,1,1);

    QPushButton* searchLogHostButton = new QPushButton("Search", tabInventar);
        connect( searchLogHostButton, SIGNAL( pressed()) , this, SLOT( logUpdateSlot() ) );
        logGrid->addWidget(searchLogHostButton,1,2);

        logGrid->addItem(new QSpacerItem ( 100, 0, QSizePolicy::Expanding, QSizePolicy::Fixed ),1,3);



    //INVENTAR
    QGridLayout* inventarGrid = new QGridLayout(tabInventar);
    inventarTree = new QTreeWidget(tabInventar);
          QStringList headerLabelsInventar;
        inventarTree->setColumnCount(9);
        inventarTree->setContextMenuPolicy(Qt::CustomContextMenu);
        inventarTree->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
        headerLabelsInventar << "host_name" << "description" << "check_interval" << "retry_interval" << "checks_enabled" << "flap_detection_enabled" << "max_check_attempts" << "check_period" << "contacts";
            inventarTree->setHeaderLabels(headerLabelsInventar);
        inventarTree->setSortingEnabled (false);
    connect(inventarTree, SIGNAL(itemDoubleClicked ( QTreeWidgetItem *, int )), this, SLOT(inventarItemDoubleClicked ( QTreeWidgetItem *, int)));
    inventarGrid->addWidget(inventarTree,0,0,1,8);

    //expandall
    inventarExpandButton = new QPushButton("Expand", tabConfig);
       inventarGrid->addWidget(inventarExpandButton,1,0);
            connect( inventarExpandButton, SIGNAL( pressed()) , this, SLOT( inventarExpandSlot() ) );
    //update
    QPushButton* inventarUpdateButton = new QPushButton("Update", tabConfig);
       inventarGrid->addWidget(inventarUpdateButton,1,1);
       connect( inventarUpdateButton, SIGNAL( pressed()) , this, SLOT( inventarUpdateSlot() ) );


    //CONFIG
    QGridLayout* configgrid = new QGridLayout(tabConfig);
        configgrid->addWidget(new QLabel("Hostname:", tabConfig),0,0);
        configgrid->addWidget(new QLabel("Port:", tabConfig),1,0);
        configgrid->addWidget(new QLabel("Refresh (Sec.):", tabConfig),2,0);
        configgrid->addWidget(new QLabel("Timezone (Hour):", tabConfig),3,0);
        configgrid->addWidget(new QLabel("Timeformat:", tabConfig),4,0);
        configgrid->addWidget(new QLabel("Log Lines:", tabConfig),5,0);
        configgrid->addWidget(new QLabel("DoubleClick URL:", tabConfig),6,0);
        configgrid->addWidget(new QLabel("Filter:", tabConfig),7,0);
        configgrid->addWidget(new QLabel("Font:", tabConfig),8,0);
        configgrid->addWidget(new QLabel("Configfile:", tabConfig),9,0);

        QRegExp rx("(\\w*|\\.|\\d*|\\-*)+");
        QValidator *hostnameValidator = new QRegExpValidator(rx, this);
        inputHostname = new QLineEdit("192.168.0.254",tabConfig);
            inputHostname->setValidator(hostnameValidator);
        configgrid->addWidget(inputHostname,0,1);

        inputPort = new QSpinBox(tabConfig);
            inputPort->setRange(1,65536);
            inputPort->setValue(6557);
        configgrid->addWidget(inputPort,1,1);

        inputAktualisierung = new QSpinBox(tabConfig);
            inputAktualisierung->setRange(1,65536);
            inputAktualisierung->setValue(20);
            connect( inputAktualisierung, SIGNAL( valueChanged ( int )) , this, SLOT( aktualisierungChanged(int) ) );
        configgrid->addWidget(inputAktualisierung,2,1);

        inputTimezone = new QSpinBox(tabConfig);
            inputTimezone->setRange(-23,+23);
            inputTimezone->setValue(1);
        configgrid->addWidget(inputTimezone,3,1);

        QRegExp tfrx("(d|M|y|h|H|m|s|z|:|/|-|\\s)+");
        QValidator *timeFormatValidator = new QRegExpValidator(tfrx, this);
        inputTimeFormat = new QLineEdit("dd-MM-yy HH:mm",tabConfig);
            inputTimeFormat->setValidator(timeFormatValidator);
        configgrid->addWidget(inputTimeFormat,4,1);

        inputLogLines = new QSpinBox(tabConfig);
            inputLogLines->setRange(0,5000);
            inputLogLines->setValue(500);
        configgrid->addWidget(inputLogLines,5,1);

        QString serviceURL="http://" + inputHostname->text() + "/nagios/cgi-bin/status.cgi?host=";
        inputServiceURL = new QLineEdit(serviceURL,tabConfig);
        configgrid->addWidget(inputServiceURL,6,1);

        QGridLayout* configFilterGrid = new QGridLayout(tabDowntime);
            inputFilter=new QLineEdit(tabConfig);
            configFilterGrid->addWidget(inputFilter,0,0);

            QPushButton* inputFilterButton = new QPushButton("..",tabConfig);
                connect( inputFilterButton, SIGNAL( pressed()) , this, SLOT( inputFilterButtonPressed() ) );
            configFilterGrid->addWidget(inputFilterButton,0,1);

        configgrid->addItem(configFilterGrid,7,1);

        QGridLayout* configFontGrid = new QGridLayout(tabDowntime);

            inputConfigFont = new QLineEdit("Segoe UI,9,-1,5,50,0,0,0,0,0",tabConfig);
            inputConfigFont->setEnabled(false);
            configFontGrid->addWidget(inputConfigFont,0,0);

            QPushButton* inputFont = new QPushButton("Schrift",tabConfig);
                connect( inputFont, SIGNAL( pressed()) , this, SLOT( setTreeFont() ) );
            configFontGrid->addWidget(inputFont,0,1);

        configgrid->addItem(configFontGrid,8,1);
        qDebug() << "configLocation" << configLocation;
        QGridLayout* configFileGrid = new QGridLayout(tabDowntime);
            inputConfigStorage = new QLineEdit(configLocation,tabConfig);
            inputConfigStorage->setEnabled(false);
            configFileGrid->addWidget(inputConfigStorage,0,0);

            QPushButton* inputConfigFile = new QPushButton("Load",tabConfig);
                connect( inputConfigFile, SIGNAL( pressed()) , this, SLOT( searchConfigFile() ) );
            configFileGrid->addWidget(inputConfigFile,0,1);

        configgrid->addItem(configFileGrid,9,1);





        /* headerLabelsService << "Hostname" << "Services" << "Status" << "Last Change" << "State Type" << "Acknowledged" << "Notification_enabled";
        headerLabelsService << "Plugin Output" << "Downtimes" << "Downtime Depth" << "host_state" << "host_state_type" << "host_acknowledged";
        headerLabelsService << "host_downtimes" << "host_scheduled_downtime_depth"  << "contacts" << "host_groups" << "last_check"; */

        QVBoxLayout *serviceTreeColumns = new QVBoxLayout(tabConfig);

        serviceTreeColumns->addWidget(new QLabel("Additional Columns:",tabConfig));

        cb_Last_Change = new QCheckBox("Last Change",tabConfig);
            cb_Last_Change->setChecked(true);
            connect( cb_Last_Change, SIGNAL( stateChanged (int)) , this, SLOT( columnsStateChange(int) ) );
            cb_Last_Change->setObjectName("cb_Last_Change");
            serviceTreeColumns->addWidget(cb_Last_Change);

        cb_State_Type = new QCheckBox("State Type",tabConfig);
            connect( cb_State_Type, SIGNAL( stateChanged (int)) , this, SLOT( columnsStateChange(int) ) );
            cb_State_Type->setObjectName("cb_State_Type");
            serviceTreeColumns->addWidget(cb_State_Type);

        cb_Downtimes = new QCheckBox("Downtimes",tabConfig);
            connect( cb_Downtimes, SIGNAL( stateChanged (int)) , this, SLOT( columnsStateChange(int) ) );
            cb_Downtimes->setObjectName("cb_Downtimes");
            serviceTreeColumns->addWidget(cb_Downtimes);

        cb_Downtime_Depth = new QCheckBox("Downtime Depth",tabConfig);
           connect( cb_Downtime_Depth, SIGNAL( stateChanged (int)) , this, SLOT( columnsStateChange(int) ) );
           cb_Downtime_Depth->setObjectName("cb_Downtime_Depth");
           serviceTreeColumns->addWidget(cb_Downtime_Depth);

        cb_host_state = new QCheckBox("Host State",tabConfig);
            connect( cb_host_state, SIGNAL( stateChanged (int)) , this, SLOT( columnsStateChange(int) ) );
            cb_host_state->setObjectName("cb_host_state");
            serviceTreeColumns->addWidget(cb_host_state);

        cb_host_state_type = new QCheckBox("Host State Type",tabConfig);
            connect( cb_host_state_type, SIGNAL( stateChanged (int)) , this, SLOT( columnsStateChange(int) ) );
            cb_host_state_type->setObjectName("cb_host_state_type");
            serviceTreeColumns->addWidget(cb_host_state_type);

        cb_host_downtimes= new QCheckBox("Host Downtimes",tabConfig);
            connect( cb_host_downtimes, SIGNAL( stateChanged (int)) , this, SLOT( columnsStateChange(int) ) );
            cb_host_downtimes->setObjectName("cb_last_check");
            serviceTreeColumns->addWidget(cb_host_downtimes);

        cb_last_check = new QCheckBox("Last Check",tabConfig);
            connect( cb_last_check, SIGNAL( stateChanged (int)) , this, SLOT( columnsStateChange(int) ) );
            cb_last_check->setObjectName("cb_last_check");
            serviceTreeColumns->addWidget(cb_last_check);


        configgrid->addLayout(serviceTreeColumns,1,5,7,2);

        configgrid->addItem(new QSpacerItem ( 100, 0, QSizePolicy::Fixed, QSizePolicy::Fixed ),10,2);

         QPushButton* configSubmitButton = new QPushButton("Save Config", tabConfig);
            configgrid->addWidget(configSubmitButton,10,5);
            connect( configSubmitButton, SIGNAL( pressed()) , this, SLOT( configSubmitSlot() ) );

        configgrid->addItem(new QSpacerItem ( 10, 10, QSizePolicy::Expanding, QSizePolicy::Expanding ),11,11);


// End of Config
        iWidget = new InfoWidget("Loading", this, Qt::Tool);


        // window->show();
        //setCentralWidget(TabWidget);
        //configSubmitButton->setFocus();
    TabWidget->setCurrentIndex(0);

    show();
    // open socket
    qDebug() << "Open Socket";

        timer = new QTimer(this);
            timer->setSingleShot(false);
             connect(timer, SIGNAL(timeout()), this, SLOT(writeServiceSlot()));
             timer->start(100000);

    bool goodconfig=loadConfig(configLocation);
    if (goodconfig)
        configSubmitSlot();
    // fertich
    aufbau=true;

    writeServiceSlot();

    //just test
  //  requestHostDetail("net-ams-ntt-router");
 //   QString wS="GET hosts\nColumns:host_name alias display_name address state last_time_down comments_with_info services_with_info contacts downtimes_with_info last_hard_state_change notes_url_expanded action_url_expanded plugin_output contact_groups parents childs host_groups worst_service_state perf_data\nFilter: host_name = est-tal-osasvm01\nOutputFormat: json\nLimit: 1\n\n";
  //  sendLiveStatusData("HostDetail", "json", HostName, Port, wS);

    /*
    LiveSocketRequest ls;
    ls.setTable("hosts");
    ls.addColumns(QStringList() << "host_name" << "alias");
    ls.addColumn("address");
    ls.setLimit(1);
    ls.addFilter("Filter: host_name = abc");
    ls.addFilters(QStringList() << "Filter: host_name = def" << "Filter: host_name = gfh");
    ls.addFilter("Or: 3");
    ls.addRawText("Filter: description = 1\nFilter: description = 2\nAnd: 2\n");
    QString ws=ls.getLiveStatusRequest();
    qDebug() << "LS String" << ws;
    */
}

ApplicationWindow::~ApplicationWindow()
{
}


/////////////////////////////////////////


void ApplicationWindow::findItem()
{
    findWidget = new QWidget(this, Qt::Dialog);
        findWidget->resize(200,50);
        findWidget->setWindowTitle("Find");


        QGridLayout* findGrid = new QGridLayout(findWidget);
            inputFind = new QLineEdit(findWidget);
            connect( inputFind, SIGNAL( returnPressed ()) , this, SLOT( searchTrees() ) );
                // nt useful to remember las string inputFind->setText(findString);
            findGrid->addWidget(inputFind,0,0);

            QPushButton* pushFind = new QPushButton("Find",findWidget);
                connect( pushFind, SIGNAL( pressed()) , this, SLOT( searchTrees() ) );
            findGrid->addWidget(pushFind,0,1);

    findWidget->show();
}

void ApplicationWindow::searchTrees()
{
    findString=inputFind->text();
    findWidget->close();

    qDebug() << "ic" << serviceTree->topLevelItemCount();
    if (TabWidget->currentIndex() == 0)
    {
        for (int i = serviceTree->topLevelItemCount()-1; i >=0 ;i--)
        {
            if ( !( ( serviceTree->topLevelItem(i)->text(0).contains(findString, Qt::CaseInsensitive) ) || ( serviceTree->topLevelItem(i)->text(1).contains(findString, Qt::CaseInsensitive) )))
            {
                delete serviceTree->topLevelItem(i);
            }
        }
    }
    else if (TabWidget->currentIndex() == 2)
    {
        for (int i = logTree->topLevelItemCount()-1; i >=0 ;i--)
        {
            if ( !( ( logTree->topLevelItem(i)->text(2).contains(findString, Qt::CaseInsensitive) ) || ( logTree->topLevelItem(i)->text(3).contains(findString, Qt::CaseInsensitive) ) || ( logTree->topLevelItem(i)->text(4).contains(findString, Qt::CaseInsensitive) )))
            {
                delete logTree->topLevelItem(i);
            }
        }
    }
    else if (TabWidget->currentIndex() == 3)
    {
        for (int i = hostgroupTree->topLevelItemCount()-1; i >=0 ;i--)
        {
            if ( !( ( hostgroupTree->topLevelItem(i)->text(2).contains(findString, Qt::CaseInsensitive) ) || ( hostgroupTree->topLevelItem(i)->text(3).contains(findString, Qt::CaseInsensitive) ) || ( logTree->topLevelItem(i)->text(4).contains(findString, Qt::CaseInsensitive) )))
            {
                delete hostgroupTree->topLevelItem(i);
            }
        }
    }
    else if (TabWidget->currentIndex() == 4)
    {
        for (int i = inventarTree->topLevelItemCount()-1; i >=0 ;i--)
        {
            if ( !( ( inventarTree->topLevelItem(i)->text(0).contains(findString, Qt::CaseInsensitive) ) || ( inventarTree->topLevelItem(i)->text(1).contains(findString, Qt::CaseInsensitive) )|| ( inventarTree->topLevelItem(i)->text(8).contains(findString, Qt::CaseInsensitive) )  ))
            {
                delete inventarTree->topLevelItem(i);
            }
        }
    }

}

void ApplicationWindow::newIconExplanation()
{
    HelpWidget *helpWidget = new HelpWidget(this, Qt::Dialog);
    helpWidget->show();
}

void ApplicationWindow::inventarExpandSlot()
{
    if (inventarExpandButton->text() == "Expand")
    {
        inventarTree->expandAll();
        inventarExpandButton->setText("Collapse");
    }
    else
    {
        inventarTree->collapseAll();
        inventarExpandButton->setText("Expand");
    }
}

void ApplicationWindow::setTreeFont()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, serviceTree->font(), this);
    if (ok)
    {
        inputConfigFont->setText(font.toString());
        qDebug() << "Font" << font.toString();
    }
}

void ApplicationWindow::searchConfigFile()
{
    QFileDialog *configFile = new QFileDialog(this, tr("Open Config"), QString(QStandardPaths::HomeLocation), tr("Config Files (*.cfg *.conf)"));
        connect(configFile, SIGNAL(currentChanged(QString)), this, SLOT(loadConfig(QString)));
        configFile->show();

}

void ApplicationWindow::changeFilters(bool checked)
{
  Filter=QString::null;
    int F=0;

    if (okAction->isChecked())
        F=F+1;

    if (warnAction->isChecked())
        F=F+2;

    if (critAction->isChecked())
        F=F+4;


    qDebug() << "F" << F;
    switch (F)
    {
        case 0: Filter+="Filter: state >= 3\nFilter: host_state >= 1\nOr: 2\n"; break;
        case 1: Filter+="Filter: state = 0\nFilter: host_state >= 1\nOr: 2\n"; break;
        case 2: Filter+="Filter: state = 1\nFilter: host_state >= 1\nOr: 2\n"; break;
        case 3: Filter+="Filter: state = 0\nFilter: state = 1\nFilter: host_state >= 1\nOr: 3\n"; break;
        case 4: Filter+="Filter: state = 2\nFilter: host_state >= 1\nOr: 2\n"; break;
        case 5: Filter+="Filter: state = 0\nFilter: state >= 2\nFilter: host_state >= 1\nOr: 3\n"; break;
        case 6: Filter+="Filter: state >= 1\nFilter: host_state >= 1\nOr: 2\n"; break;
//        case 7: Filter+="Filter: state <= 3\nFilter: host_state >= 1\nOr: 2\n"; break;
    }


    if (notAction->isChecked())
       Filter+="Filter: notifications_enabled = 1\n";

    if (ackAction->isChecked())
        Filter+="Filter: acknowledged = 0\n";

    if (stateAction->isChecked())
        Filter+="Filter: state_type = 1\n";

    if (downAction->isChecked())
        Filter+="Filter: scheduled_downtime_depth = 0\nFilter: host_scheduled_downtime_depth = 0\n";

    writeServiceSlot();
}

void ApplicationWindow::updateSlot()
{
        tabConfig->show();
}

void ApplicationWindow::serviceItemDoubleClicked ( QTreeWidgetItem * item, int column )
{
    if (!item->text(0).trimmed().isEmpty())
    requestHostDetail(item->text(0));
    //QDesktopServices::openUrl(QUrl(QString(inputServiceURL->text() + item->text(0))));
    //qDebug() << inputServiceURL->text() << item->text(0);
}

void ApplicationWindow::inventarItemDoubleClicked ( QTreeWidgetItem * item, int column )
{
    //QDesktopServices::openUrl(QUrl(QString(inputServiceURL->text() + item->text(0))));
    if (!item->text(0).trimmed().isEmpty())
    requestHostDetail(item->text(0));
}

void ApplicationWindow::downtimeItemDoubleClicked ( QTreeWidgetItem * item, int column )
{
    //QDesktopServices::openUrl(QUrl(QString(inputServiceURL->text() + item->text(0))));
    if (!item->text(0).trimmed().isEmpty())
    requestHostDetail(item->text(0));
}

void ApplicationWindow::hostgroupItemDoubleClicked ( QTreeWidgetItem * item, int column )
{
    //QDesktopServices::openUrl(QUrl(QString(inputServiceURL->text() + item->text(0))));
    if (!item->text(1).trimmed().isEmpty())
    requestHostDetail(item->text(1));
}

void ApplicationWindow::logItemDoubleClicked ( QTreeWidgetItem * item, int column )
{
    if (!item->text(2).trimmed().isEmpty())
    requestHostDetail(item->text(2));
}

void ApplicationWindow::configSubmitSlot()
{
    HostName=inputHostname->text();
    Port=inputPort->value();

    timer->setInterval(inputAktualisierung->value()*1000);

    writeConfig();

    changeFilters(true);

    QFont font;
    font.fromString (inputConfigFont->text());
    serviceTree->setFont (font);
    downtimeTree->setFont(font);
    inventarTree->setFont(font);
        serviceTree->setNagiosHostname(inputHostname->text());
        serviceTree->setTimezone(inputTimezone->value());
    TabWidget->setCurrentIndex(0);
}
void ApplicationWindow::writeServiceSlot()
{
 //   QString state="Filter: state != 0\n";
    timer->start();
    serviceTree->setSortingEnabled (false);
    serviceTree->clear();

 /*  QString start="GET services\nColumns: host_name description state last_state_change state_type acknowledged notifications_enabled plugin_output downtimes scheduled_downtime_depth ";
            start+="host_state host_state_type host_acknowledged host_downtimes host_scheduled_downtime_depth contacts\n";

            QString wS=start + Filter + getFilter() +"\n";
    //qDebug() << write;
*/
            LiveSocketRequest ls;
            ls.setTable("services");
            ls.addColumns(QStringList() << "host_name" << "description" << "state" << "last_state_change" << "state_type" << "acknowledged" << "notifications_enabled" << "plugin_output" << "downtimes" << "scheduled_downtime_depth");
            ls.addColumns(QStringList() << "host_state" << "host_state_type" << "host_acknowledged" << "host_downtimes" << "host_scheduled_downtime_depth" << "contacts");
            ls.addRawText(Filter);
            ls.addRawText(getFilter());
            QString wS=ls.getLiveStatusRequest();
           // qDebug() << "LS String" << wS; //.replace("\n","|\n");

    QDateTime dt = QDateTime::currentDateTime ();
    statusBar()->showMessage( "Updated on: " + dt.toString("hh:mm") );

    sendLiveStatusData("Service", "txt", HostName, Port, wS);
}

void ApplicationWindow::closeEvent( QCloseEvent* ce )
{
   // We just want to exit, if File->exit is used. Otherwise ignore the closeEvent
    QObject* obj = sender();
    if (obj != exitAction)
    {
        qApp->activeWindow()->hide();
        ce->ignore();
    }
    else
    {
        ce->accept();
    }
}

void ApplicationWindow::aktualisierungChanged(int time)
{
    timer->setInterval(time*1000);
}

void ApplicationWindow::errorOccured(QString errorString)
{
    qDebug() << "void ApplicationWindow::errorOccured(QString errorString)";
//    iWidget->showInfoMessage(errorString);
    iWidget->hideInfoMessage();
    timer->stop();
    aufbau=false;

    statusBar()->showMessage( "Error:" + errorString );
    QString mbString;
        mbString += "Error:";
        mbString += errorString + "\n";
        mbString += "Please check your configuration.";
    QMessageBox::critical(this,"Critical",mbString );
    TabWidget->setCurrentIndex(5);


}

void ApplicationWindow::gotServiceData(QList<QStringList> listOfStringLists)
{
    QList<QTreeWidgetItem *> items;

    //set SystemTrayIcon
    qDebug()<<"got Servicedata listOfStringLists.size()" << listOfStringLists.size();
    QPixmap mwIcon;
    if (listOfStringLists.size() > 1)
    {
        mwIcon = drawSystemTrayIcon(QString("%1").arg(listOfStringLists.size()-1));
    }
    else
    {
        mwIcon=QPixmap(":/ok.png");
    }
    iconTray->setIcon(mwIcon);
    setWindowIcon(mwIcon);

        //hostname, size one bigger than nummer -1
        for (int k = 0; k < listOfStringLists.size()-1; ++k)
        {

            QList<QTreeWidgetItem *> hostList;
            hostList=serviceTree->findItems(listOfStringLists.at(k).at(0),Qt::MatchExactly,0);

            QTreeWidgetItem* Item = new QTreeWidgetItem(listOfStringLists.at(k),QTreeWidgetItem::Type);

            int epochSecs=(listOfStringLists.at(k).at(3).toInt());
                    QDateTime dt = QDateTime::fromString("01-01-70 00:00:00", "dd-MM-yy HH:mm:ss").addSecs(epochSecs+(3600*inputTimezone->value()));
            //Item->setText(3,dt.toString("HH:mm dd-MM-yy"));
            Item->setText(3,dt.toString(inputTimeFormat->text()));

            if (hostList.size() > 0)
                hostList.first()->addChild(Item);
            else
                items.append(Item);

              serviceTree->insertTopLevelItems(0, items);
/*
0 host_name
1 description
2 state
3 last_state_change
4 state_type
5 acknowledged
6 notifications_enabled
7 plugin_output
8 downtimes
9 scheduled_downtime_depth
10 host_state
11 host_state_type
12 host_acknowledge
13 host_downtimes
14 host_scheduled_downtime_depth
15 contacts
*/

              // lets do the color for hosts
              if (listOfStringLists.at(k).at(10) == "1")
              {
                  if (listOfStringLists.at(k).at(12) == "0")
                    Item->setIcon(0, QIcon(QPixmap(critical_xpm)) );
                  else
                    Item->setIcon(0, QIcon(QPixmap(critical_ack_xpm)) );
              }
              else if (listOfStringLists.at(k).at(10) == "0")
              {
                      //Item->setIcon(0, QIcon(ok_xpm) );
              }
              else
              {
                   Item->setIcon(0, QIcon(QPixmap(unknown_xpm)) );
              }

              if (listOfStringLists.at(k).at(4) == "0")
              {
                  Item->setIcon(3, QIcon(QPixmap(soft_xpm)) );
              }
              if (listOfStringLists.at(k).at(6) == "0")
              {
                  Item->setIcon(1, QIcon(QPixmap(ndisabled_xpm)) );
              }


              // lets do the color fro services
              if (listOfStringLists.at(k).at(2) == "2")
              {
                  if (listOfStringLists.at(k).at(5) == "0")
                    Item->setIcon(1, QIcon(QPixmap(critical_xpm)) );
                  else
                    Item->setIcon(1, QIcon(QPixmap(critical_ack_xpm)) );
              }
              else if (listOfStringLists.at(k).at(2) == "1")
              {
                  if (listOfStringLists.at(k).at(5) == "0")
                       Item->setIcon(1, QIcon(QPixmap(warning_xpm)) );
                  else
                       Item->setIcon(1, QIcon(QPixmap(warning_ack_xpm)) );
              }
              else if (listOfStringLists.at(k).at(2) == "0")
              {
                      Item->setIcon(1, QIcon(QPixmap(ok_xpm)) );
              }
              else
              {
                   Item->setIcon(1, QIcon(QPixmap(unknown_xpm)) );
              }

              //notification
              if (listOfStringLists.at(k).at(6) == "0")
              {
                  Item->setIcon(3, QIcon(QPixmap(ndisabled_xpm)) );
              }

              //soft state
              if ( (listOfStringLists.at(k).at(4) == "0") || (listOfStringLists.at(k).at(11) == "0") )
              {
                  Item->setIcon(3, QIcon(QPixmap(soft_xpm)) );
              }
              //downtime service
              if ( (listOfStringLists.at(k).at(8) != "0") && (listOfStringLists.at(k).at(9) != "0") )
              {
                  Item->setIcon(1, QIcon(QPixmap(downtime_xpm)) );
              }
              //downtime host
              if ( (listOfStringLists.at(k).at(13) != "0") && (listOfStringLists.at(k).at(14) != "0") )
              {
                  Item->setIcon(0, QIcon(QPixmap(downtime_xpm)) );
              }

        }

        //ugly hack, sequentiell
        serviceTree->sortByColumn(0,Qt::AscendingOrder);
        serviceTree->setSortingEnabled (true);
       // serviceTree->expandAll();

}

void ApplicationWindow::inventarUpdateSlot()
{
    inventarTree->clear();
    currentTabChanged(4);
}

void ApplicationWindow::currentTabChanged(int tab)
{
    QString Purpose;
    QString wS;
    LiveSocketRequest ls;
      qDebug()<<"Tab"<<tab;
    //nicht beim ersten mal ;o)
    if (!aufbau)
    {
        return;
    }

    //we are not in the service Tree, lets stop the timer
    if(tab!=0)
    {
        timer->stop();
    }
    else
    {
        timer->start();
    }

    if (tab==0)
    {
        // Service seperate
        changeFilters(true);
        return;
    }
    else if (tab==1)
    {
        //wS = "GET downtimes\nColumns:host_name service_description start_time end_time author comment\n\n";

        ls.setTable("downtimes");
        ls.addColumns(QStringList() << "host_name" << "service_description" << "start_time" << "end_time" << "author" << "comment");
        wS=ls.getLiveStatusRequest();
        Purpose="Downtime";
    }
    else if (tab==2)
    {
        ls.setTable("log");
        ls.addColumns(QStringList() << "time state" << "current_host_name" << "current_service_description" << "message");

        if (!searchLogHost->text().isEmpty())
        {
           // wS= "GET log\nColumns: time state current_host_name current_service_description message\nFilter: host_name = " + searchLogHost->text() + "\nLimit:" + QString("%1").arg(inputLogLines->value()) + "\n\n";
            ls.addFilter("Filter: host_name = " + searchLogHost->text());
        }
        ls.setLimit(inputLogLines->value());
        wS=ls.getLiveStatusRequest();
        Purpose="Log";
    }
    else if (tab==3)
    {
        //hostgroups
        //wS= "GET hostgroups\nColumns: alias num_hosts num_hosts_up members\n\n";
        ls.setTable("hostgroups");
        ls.addColumns(QStringList() << "alias" << "num_hosts" << "num_hosts_up" << "members");
        wS=ls.getLiveStatusRequest();
        Purpose="Hostgroup";
    }
    else if (tab==4)
    {
        //really faster
        inventarTree->setSortingEnabled (false);
        //qDebug() << "inventarTree->topLevelItemCount()" << inventarTree->topLevelItemCount();
        // dont update if you aready got data
        if (!inventarTree->topLevelItemCount() > 0)
        {
           // wS = "GET services\nColumns: host_name description check_interval retry_interval checks_enabled flap_detection_enabled max_check_attempts check_period contacts\n\n";
            ls.setTable("services");
            ls.addColumns(QStringList() << "host_name" << "description" << "check_interval" << "retry_interval" << "checks_enabled" << "flap_detection_enabled" << "max_check_attempts" << "check_period" << "contacts");
            wS=ls.getLiveStatusRequest();
            Purpose="Inventar";
        }
        else
        {
            return;
        }
    }
    else if (tab==5)
    {
        wS="GET services\nColumns: description host_name host_groups\n\n";
        ls.setTable("services");
        ls.addColumns(QStringList() << "description" << "host_name" << "host_groups");
        wS=ls.getLiveStatusRequest();
        Purpose="Config";
    }

    sendLiveStatusData(Purpose, "txt", HostName, Port, wS);

}

void ApplicationWindow::receiveLiveStatusData(QString Purpose, QList<QStringList> listOfStringLists)
{
    //qDebug() << "listOfStringLists.size()" << listOfStringLists.size() << listOfStringLists.at(0);
    iWidget->hideInfoMessage();
    if ( Purpose == "Log" )
    {
        gotLogData(listOfStringLists);
    }
    else if ( Purpose == "Hostgroup" )
    {
        gotHostgroupData(listOfStringLists);
    }
    else if ( Purpose == "Downtime" )
    {
        gotDowntimeData(listOfStringLists);
    }
    else if ( Purpose == "Inventar" )
    {
        gotInventarData(listOfStringLists);
    }
    else if ( Purpose == "Service" )
    {
        gotServiceData(listOfStringLists);
    }
    else if ( Purpose == "Config" )
    {
        gotConfigData(listOfStringLists);
    }
    /* is handled by receiveLiveStatusJSONArray
    else if ( Purpose == "HostDetail" )
    {
        //there is always an empty StringList at the end
        if (listOfStringLists.size() == 2)
        {
            HostInfoWidget* hostInfoWidget = new HostInfoWidget(listOfStringLists,getInputTimeFormat(), getInputTimeZone(),getInputServiceURL(), getUniqObjectID(), this,Qt::Dialog);
                connect(hostInfoWidget, SIGNAL( doubleClickedHost(QString)), this, SLOT(requestHostDetail(QString)));
                connect(hostInfoWidget, SIGNAL( requestLiveStatusData(int, QString)), this, SLOT(childRequestLiveStatusData(int, QString)));
                connect(hostInfoWidget, SIGNAL( logSearch(QString)), this, SLOT(logSearch(QString)));
            hostInfoWidget->show();
            hostInfoWidgetList.append(hostInfoWidget);
        }
    }*/
    else
    {
        bool ok;
        int value = Purpose.toInt(&ok);
        // Its a number, so it must be a child request to Livestatus
        // currently we have just one request Object, so lets start from 0 (later use 10000++)
        if (ok)
        {
            qDebug() << "Its a child request for"<< Purpose;
            for (int j=0;j<hostInfoWidgetList.size();j++)
            {
                HostInfoWidget* child = hostInfoWidgetList.at(j);
                if (child->getUniqObjectID() == value)
                {
                    child->gotLiveStatusData(listOfStringLists);
                }
            }
        }
    }
}
void ApplicationWindow::receiveLiveStatusJSONArray(QString Purpose,QJsonArray jsonArray)
{
    iWidget->hideInfoMessage();
    if (Purpose == "HostDetail")
    {
            HostInfoWidget* hostInfoWidget = new HostInfoWidget(jsonArray,getInputTimeFormat(), getInputTimeZone(),getInputServiceURL(), getUniqObjectID(), this,Qt::Dialog);
        connect(hostInfoWidget, SIGNAL( doubleClickedHost(QString)), this, SLOT(requestHostDetail(QString)));
        connect(hostInfoWidget, SIGNAL( requestLiveStatusData(int, QString)), this, SLOT(childRequestLiveStatusData(int, QString)));
        connect(hostInfoWidget, SIGNAL( logSearch(QString)), this, SLOT(logSearch(QString)));
        hostInfoWidget->show();
        hostInfoWidgetList.append(hostInfoWidget);
    }
}

void ApplicationWindow::gotConfigData(QList<QStringList> listOfStringLists)
{
//These QStringLists could be used to create selectors/choosers
        for (int k = 0; k < listOfStringLists.size()-1; ++k)
        {
            configServiceList << listOfStringLists.at(k).at(0);
            configHostList << listOfStringLists.at(k).at(1);
            QStringList tmpHostgroupList = listOfStringLists.at(k).at(2).split(",");
            configHostgroupList << tmpHostgroupList;
        }
        qDebug() << "configServiceList" << configServiceList;

        configServiceList.sort();
        configServiceList.removeDuplicates();

        configHostList.sort();
        configHostList.removeDuplicates();

        configHostgroupList.sort();
        configHostgroupList.removeDuplicates();
}


void ApplicationWindow::gotLogData(QList<QStringList> listOfStringLists)
{
    QStringList completerList;
    logTree->clear();
    QList<QTreeWidgetItem *> items;
        for (int k = 0; k < listOfStringLists.size()-1; ++k)
        {
          //  qDebug() <<"Listsize" <<listOfStringLists.at(k).size();
            QTreeWidgetItem* Item;
            //separate all ; if list bigger 5, make it 5 or we lost informations
            if (listOfStringLists.at(k).size() > 5)
            {
                QStringList newList;
                for (int a=0;a<4;a++)
                {
                    newList << listOfStringLists.at(k).at(a);
                }
                QString leftOver;
                for (int b=4;b<listOfStringLists.at(k).size();b++)
                {
                    //qDebug() <<"appending" << listOfStringLists.at(k).at(b)+";";
                    leftOver.append(listOfStringLists.at(k).at(b)+";");
                }
                newList << leftOver;
             //   qDebug() << newList;
                    Item = new QTreeWidgetItem(newList,QTreeWidgetItem::Type);
            }
            else
            {
                Item = new QTreeWidgetItem(listOfStringLists.at(k),QTreeWidgetItem::Type);
            }

            int epochSecs=(QString(listOfStringLists.at(k).at(0)).toInt());
            // ugly Timezone hack
            QDateTime dt = QDateTime::fromString("01-01-70 00:00:00", "dd-MM-yy HH:mm:ss").addSecs(epochSecs+(3600*inputTimezone->value()));
            //Item->setText(0, dt.toString("HH:mm:ss"));
             Item->setText(0,dt.toString(inputTimeFormat->text()));

        // lets do the icon for services
            if (listOfStringLists.at(k).at(1) == "0")
            {
                Item->setIcon(1, QIcon(QPixmap(ok_xpm)) );
            }
            else if (listOfStringLists.at(k).at(1) == "1")
            {
                Item->setIcon(1, QIcon(QPixmap(warning_xpm)) );

            }
            else if (listOfStringLists.at(k).at(1).toInt() >= 2)
            {
                Item->setIcon(1, QIcon(QPixmap(critical_xpm)) );
            }
            //Status text not needed
            Item->setText(1,"");

//            QString messag = listOfStringLists.at(k).at(1);
  //          Item->setText(4,messag.replace(QRegExp("^\\[\\d{10}\\]"), "a"));

        items.append(Item);

        logTree->insertTopLevelItems(logTree->topLevelItemCount (), items);
        completerList << listOfStringLists.at(k).at(2);
        }

            completerList.removeDuplicates();
            completerList.sort(Qt::CaseInsensitive);
            QCompleter *searchLogHostCompleter = new QCompleter(completerList, this);
                searchLogHostCompleter->setCaseSensitivity(Qt::CaseInsensitive);
                //searchLogHostCompleter->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
            searchLogHost->setCompleter(searchLogHostCompleter);
            qDebug()<<"completerList"<<completerList;

  //  logTree->sortByColumn(0,Qt::DescendingOrder);
}


void ApplicationWindow::gotHostgroupData(QList<QStringList> listOfStringLists)
{
    hostgroupTree->clear();
    QList<QTreeWidgetItem *> items;

         //hostname, size one bigger than nummer -1
        for (int k = 0; k < listOfStringLists.size()-1; ++k)
        {
        QList<QTreeWidgetItem *> hostgroupList;
//        hostgroupList=hostgroupTree->findItems(listOfStringLists.at(k).at(0),Qt::MatchExactly,0);

        qDebug() << k << "members" << listOfStringLists.at(k).at(3) << listOfStringLists.at(k).at(3).count(",");

        // we have to splitt the members and create a new stringlist
            for (int j = 0; j <= listOfStringLists.at(k).at(3).count(","); ++j)
            {
                QString member=listOfStringLists.at(k).at(3).section(',',j,j);

                hostgroupList=hostgroupTree->findItems(listOfStringLists.at(k).at(0),Qt::MatchExactly,0);

                qDebug() << k << "member" << member;
                QStringList splitHostgroup;
                QString down= QString("%1").arg(listOfStringLists.at(k).at(1).toInt() - listOfStringLists.at(k).at(2).toInt());
                splitHostgroup << listOfStringLists.at(k).at(0) << member << listOfStringLists.at(k).at(1) << down;
                qDebug() << k << "new" << splitHostgroup << "size hgL" << hostgroupList.size();

                QTreeWidgetItem* Item = new QTreeWidgetItem(splitHostgroup,QTreeWidgetItem::Type);
                //QTreeWidgetItem* Item = new QTreeWidgetItem(listOfStringLists.at(k),QTreeWidgetItem::Type);

                if (hostgroupList.size() > 0)
                    hostgroupList.first()->addChild(Item);
                else
                    items.append(Item);

                hostgroupTree->insertTopLevelItems(0, items);
            }
            statusBar()->showMessage( "Adding " + listOfStringLists.at(k).at(0) );
        }

      //  hostgroupTree->insertTopLevelItems(0, items);
        hostgroupTree->sortByColumn(0,Qt::AscendingOrder);
        QDateTime dt = QDateTime::currentDateTime ();
        statusBar()->showMessage( "Updated on: " + dt.toString("hh:mm") );
        //downtimeTree->expandAll ();
}


void ApplicationWindow::gotInventarData(QList<QStringList> listOfStringLists)
{
    inventarTree->clear();
    QList<QTreeWidgetItem *> items;
        for (int k = 0; k < listOfStringLists.size()-1; ++k)
        {
            QList<QTreeWidgetItem *> hostList;
            hostList=inventarTree->findItems(listOfStringLists.at(k).at(0),Qt::MatchExactly,0);

          //  qDebug() <<"Listsize" <<listOfStringLists.at(k).size();
            QTreeWidgetItem* Item;
            //separate all ; if list bigger 5, make it 5 or we lost informations
                Item = new QTreeWidgetItem(listOfStringLists.at(k),QTreeWidgetItem::Type);

                    //check_interval
                    Item->setText(2, QString("%1").arg( listOfStringLists.at(k).at(2).toDouble() ));

                    //retry_interval
                    Item->setText(3, QString("%1").arg( listOfStringLists.at(k).at(3).toDouble() ));


                if (hostList.size() > 0)
                    hostList.first()->addChild(Item);
                else
                    items.append(Item);

        //inventarTree->insertTopLevelItems(logTree->topLevelItemCount (), items);
        inventarTree->insertTopLevelItems(0, items);
        }

    inventarTree->sortByColumn(0,Qt::AscendingOrder);
    inventarTree->setSortingEnabled (true);




}


void ApplicationWindow::gotDowntimeData(QList<QStringList> listOfStringLists)
{
    downtimeTree->clear();
    QList<QTreeWidgetItem *> items;

         //hostname, size one bigger than nummer -1
        for (int k = 0; k < listOfStringLists.size()-1; ++k)
        {

        QList<QTreeWidgetItem *> hostList;
        hostList=downtimeTree->findItems(listOfStringLists.at(k).at(0),Qt::MatchExactly,0);

        QTreeWidgetItem* Item = new QTreeWidgetItem(listOfStringLists.at(k),QTreeWidgetItem::Type);

        int epochSecs=(listOfStringLists.at(k).at(2).toInt());
                QDateTime dt = QDateTime::fromString("01-01-70 00:00:00", "dd-MM-yy HH:mm:ss").addSecs(epochSecs+(3600*inputTimezone->value()));
        //Item->setText(2,dt.toString("HH:mm dd-MM-yy"));
        Item->setText(2,dt.toString(inputTimeFormat->text()));

        epochSecs=(listOfStringLists.at(k).at(3).toInt());
                dt = QDateTime::fromString("01-01-70 00:00:00", "dd-MM-yy HH:mm:ss").addSecs(epochSecs+(3600*inputTimezone->value()));
        //Item->setText(3,dt.toString("HH:mm dd-MM-yy"));
        Item->setText(3,dt.toString(inputTimeFormat->text()));

        if (hostList.size() > 0)
            hostList.first()->addChild(Item);
        else
            items.append(Item);

        downtimeTree->insertTopLevelItems(0, items);
        }

    downtimeTree->sortByColumn(0,Qt::AscendingOrder);
    //downtimeTree->expandAll ();
}

void ApplicationWindow::aboutslot()
{
    QString abouttext;
    abouttext ="Whassup\n";
    abouttext+="Version 0.6 Beta (02/2012)\n";
    abouttext+="\n\n";
    abouttext+="mailto:Web@ReneStorm.de for questions or comments\n";
    abouttext+="Copyright 2011 by Rene Storm\n";
    abouttext+="BDR 529\n";
            QMessageBox::about(this,"About",abouttext );
    okAction->setVisible(true);
}

void ApplicationWindow::writeConfig()
{
    QFile file(QString(inputConfigStorage->text()));
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            statusBar()->showMessage( configLocation );
            return;
        }
        else
        {
            qDebug() << "Writing to:" << configLocation;
            statusBar()->showMessage( configLocation );

        QTextStream out(&file);
        out << "hostname=" << inputHostname->text() << "\n";
        out << "port=" << QString("%1").arg(inputPort->value()) << "\n";
        out << "aktualisierung=" << QString("%1").arg(inputAktualisierung->value()) << "\n";
        out << "timezone=" << QString("%1").arg(inputTimezone->value()) << "\n";
        out << "logLines=" << QString("%1").arg(inputLogLines->value()) << "\n";
        out << "timeformat=" << QString(inputTimeFormat->text()) << "\n";


        out << "serviceURL=" << inputServiceURL->text() << "\n";
        out << "font=" << serviceTree->font().toString() << "\n";

        if (ackAction->isChecked())
            out << "acknowledged=1\n";
        else
            out << "acknowledged=0\n";
        if (notAction->isChecked())
            out << "notifications=1\n";
        else
            out << "notifcations=0\n";
        if (downAction->isChecked())
            out << "downtime=1\n";
        else
            out << "downtime=0\n";
        if (stateAction->isChecked())
            out << "hardstate=1\n";
        else
            out << "hardstate=0\n";

       out << "columnInt=" << QString("%1").arg(globalColumnInt) << "\n";
       out << "filter=" << inputFilter->text() << "\n";
        file.close();
        }

}

bool ApplicationWindow::loadConfig(QString FileName)
{
    int revalint=0;
    inputConfigStorage->setText(FileName);

    QFile file(inputConfigStorage->text());
         if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
         {
             statusBar()->showMessage( "Could not load " + inputConfigStorage->text() );
             return false;
         }
         QTextStream in(&file);
            while (!in.atEnd())
            {
                 QString line = in.readLine().trimmed();

                 if (line.startsWith("hostname="))
                 {
                     inputHostname->setText( (line.split('='))[1] );
                     revalint++;
                 }
                 else if (line.startsWith("port="))
                 {
                    inputPort->setValue( (line.split('='))[1].toInt() );
                    revalint++;
                 }
                 else if (line.startsWith("aktualisierung="))
                 {
                    inputAktualisierung->setValue( (line.split('='))[1].toInt() );
                 }
                 else if (line.startsWith("timezone="))
                 {
                    inputTimezone->setValue( (line.split('='))[1].toInt() );
                 }
                 else if (line.startsWith("timeformat="))
                 {
                     inputTimeFormat->setText((line.split('='))[1]);
                 }
                 else if (line.startsWith("serviceURL="))
                 {
                     int g = line.indexOf ("=");
                    inputServiceURL->setText(line.rightRef(line.size()-1-g).toString());
                 }
                 else if (line.startsWith("font="))
                 {
                    inputConfigFont->setText( (line.split('='))[1] );
                 }
                 else if (line.startsWith("acknowledged="))
                 {
                    if (line.split('=')[1] == "1")
                    {
                        ackAction->setChecked ( true );
                    }
                    else if (line.split('=')[1] == "0")
                    {
                        ackAction->setChecked ( false );
                    }
                 }
                 else if (line.startsWith("notifications="))
                 {
                    if (line.split('=')[1] == "1")
                    {
                        notAction->setChecked ( true );
                    }
                    else if (line.split('=')[1] == "0")
                    {
                        notAction->setChecked ( false );
                    }
                 }
                 else if (line.startsWith("downtime="))
                 {
                    if (line.split('=')[1] == "1")
                    {
                        downAction->setChecked ( true );
                    }
                    else if (line.split('=')[1] == "0")
                    {
                        downAction->setChecked ( false );
                    }
                 }
                 else if (line.startsWith("hardstate="))
                 {
                    if (line.split('=')[1] == "1")
                    {
                        stateAction->setChecked ( true );
                    }
                    else if (line.split('=')[1] == "0")
                    {
                        stateAction->setChecked ( false );
                    }
                 }
                 else if (line.startsWith("columnInt="))
                 {
                     globalColumnInt=line.split('=')[1].toInt();
                     setCheckBoxes(globalColumnInt);
                 }
                 else if (line.startsWith("logLines="))
                 {
                    inputLogLines->setValue(QString((line.split('='))[1]).toInt());
                 }
                 else if (line.startsWith("filter="))
                 {
                     QString f=line.remove("filter=");
                        inputFilter->setText(f);
                 }




             }

            if (revalint==2)
                return true;
            else
                return false;
}

void ApplicationWindow::inputFilterButtonPressed()
{
    if (!hideInputFilter)
    {
    filterWidget = new FilterWidget(inputFilter->text(),configServiceList, configHostList, configHostgroupList,this, Qt::Dialog);
        connect( filterWidget, SIGNAL( gotNewFilter(QString)) , this, SLOT( gotNewFilter(QString)) );
        hideInputFilter=true;
    }
    filterWidget->show();
}

void ApplicationWindow::gotNewFilter(QString newFilter)
{
    qDebug() << newFilter;
        inputFilter->setText(newFilter);
        filterWidget->hide();
}

QString ApplicationWindow::getFilter()
{
    QString Filter;
    if (!inputFilter->text().trimmed().isEmpty())
    {
        QStringList FilterSplit=inputFilter->text().split(',');
        for (int i = 0; i < FilterSplit.size(); ++i)
        {
            if (!( (FilterSplit.at(i).trimmed().startsWith("And")) || (FilterSplit.at(i).trimmed().startsWith("Or")) ) )
            {
                Filter=Filter + "Filter: " + FilterSplit.at(i).trimmed() + "\n";
            }
            else
            {
                Filter=Filter + FilterSplit.at(i).trimmed() + "\n";
            }
        }
        qDebug() << "Filter" << Filter;
        //Filter="Filter: description != 100_WINDOWS-hosts\n";

    }
    return Filter;
}

void ApplicationWindow::setCheckBoxes(int columnInt)
{

     //Last Change
     if (columnInt & 1)
          cb_Last_Change->setChecked(true);

     //State_Type
     if (columnInt & 2)
        cb_State_Type->setChecked(true);

     //Downtimes
     if (columnInt & 4)
        cb_Downtimes->setChecked(true);

     //Downtime_Depths
     if (columnInt & 8)
        cb_Downtime_Depth->setChecked(true);

     //host_state
     if (columnInt & 16)
           cb_host_state->setChecked(true);

     //host_state_type
     if (columnInt & 32)
        cb_host_state_type->setChecked(true);

     //host_downtimes
     if (columnInt & 64)
        cb_host_downtimes->setChecked(true);

     //last_check
     if (columnInt & 128)
        cb_last_check->setChecked(true);
}

void ApplicationWindow::columnsStateChange(int)
{
    int columnInt=0;
    if (cb_Last_Change->isChecked())
        columnInt=columnInt+1;
    if (cb_State_Type->isChecked())
        columnInt=columnInt+2;
    if (cb_Downtimes->isChecked())
        columnInt=columnInt+4;
    if (cb_Downtime_Depth->isChecked())
        columnInt=columnInt+8;
    if (cb_host_state->isChecked())
        columnInt=columnInt+16;
    if (cb_host_state_type->isChecked())
        columnInt=columnInt+32;
    if (cb_host_downtimes->isChecked())
        columnInt=columnInt+64;
    if (cb_last_check->isChecked())
        columnInt=columnInt+128;


    qDebug() << "columnsStateChange" << columnInt;
    globalColumnInt=columnInt;
    serviceTree->setHiddenColumns(columnInt);

}

void ApplicationWindow::systemTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::DoubleClick)
    {
        raise();
        show();
    }
}

QPixmap ApplicationWindow::drawSystemTrayIcon(QString text)
{
    qDebug() << "drawSystemTrayIcon(QString text)" << text;
    QPixmap pixmap(":/critical.png");
    QPainter painter;
    painter.begin(&pixmap);
        QFont font=painter.font() ;
        font.setWeight(QFont::DemiBold);
        painter.setFont(font);
        painter.drawText(0,0,16,16,Qt::AlignCenter,text);
    painter.end();

    return pixmap;
}

void ApplicationWindow::logUpdateSlot()
{
    currentTabChanged(2);
}

void ApplicationWindow::logSearch(QString host)
{
    qDebug() << "Searching for host in log" << host;
    searchLogHost->setText(host);
    TabWidget->setCurrentIndex(2);
}


void ApplicationWindow::sendLiveStatusData(QString Purpose, QString Format, QString HostName, int Port, QString request)
{
    if (aufbau==false) return;
    requestCounter++;
    qDebug() << "requestCounter"<<requestCounter;
    qDebug() << "sending to Livestatus" << Purpose << HostName << Port << request;
    iWidget->showInfoMessage("Loading...");
    LiveSocket* ls = new LiveSocket( Purpose, Format, HostName, Port, request, this );
       connect( ls, SIGNAL( sendLiveStatusData(QString, QList<QStringList>)) , this, SLOT( receiveLiveStatusData(QString, QList<QStringList>) ) );
       connect( ls, SIGNAL( sendLiveStatusJSONArray(QString,QJsonArray)) , this, SLOT( receiveLiveStatusJSONArray(QString,QJsonArray)) );
       connect( ls, SIGNAL( errorLiveSocket(QString)), this, SLOT( errorOccured(QString) ) );
    ls->connectLiveSocket();
}

void ApplicationWindow::requestHostDetail(QString host)
{
    qDebug() << "Requesting for hostdetail for:" << host;
   //QString wS="GET hosts\nColumns:host_name alias display_name address state last_time_down comments_with_info services_with_info contacts downtimes_with_info last_hard_state_change notes_url_expanded action_url_expanded plugin_output contact_groups parents childs host_groups worst_service_state perf_data\nFilter: host_name = " + host +" \nOutputFormat: json\nLimit: 1\n\n";
    LiveSocketRequest ls;
    ls.setTable("hosts");
    ls.addColumns(QStringList() << "host_name" << "alias" << "display_name" << "address" << "state");
    ls.addColumns(QStringList() << "last_time_down" << "comments_with_info" << "services_with_info");
    ls.addColumns(QStringList() << "contacts" << "downtimes_with_info" << "last_hard_state_change");
    ls.addColumns(QStringList() << "notes_url_expanded" << "action_url_expanded" << "plugin_output");
    ls.addColumns(QStringList() << "contact_groups" << "parents" << "childs" << "host_groups");
    ls.addColumns(QStringList() << "worst_service_state" << "perf_data");
    ls.addFilter("Filter: host_name = " + host);
    ls.setOutputFormat("json");
    ls.setLimit(1);
    QString wS=ls.getLiveStatusRequest();
    QString Purpose="HostDetail";
    sendLiveStatusData(Purpose, "json", HostName, Port, wS);
}


void ApplicationWindow::childRequestLiveStatusData(int uid, QString request)
{
    sendLiveStatusData(QString("%1").arg(uid), "txt", HostName, Port, request);
}
