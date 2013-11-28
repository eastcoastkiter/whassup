#include "whassup.h"
using namespace std;

Whassup::Whassup()
	: QMainWindow( 0 )
{
uniqObjectID=0;
hide=false;
aufbau=false;
haserror=false;
blinkInterator=true;

requestCounter=0;

setWindowTitle("Whassup");
configLocation = QString(QStandardPaths::locate(QStandardPaths::HomeLocation, QString(), QStandardPaths::LocateDirectory) + "whassup.cfg");
setAccessibleName("application");
qApp->setQuitOnLastWindowClosed(false);

    //StatusBar
        statusBar = new StatusBar(this);
        setStatusBar(statusBar);

    //Tray Icon
    iconTray = new QSystemTrayIcon;
    QPixmap mwIcon(":/images/ok.png");
    iconTray->setIcon(mwIcon);

    // blink systemtray
    blinkTimer = new QTimer(this);
    blinkTimer->setSingleShot(false);
    connect(blinkTimer, SIGNAL(timeout()), this, SLOT(blinkTrayIcon()));

    //iconTray->setIcon(QPixmap(ok_xpm));
    iconTray->setVisible(true);
    connect(iconTray, SIGNAL(activated ( QSystemTrayIcon::ActivationReason )), this, SLOT(systemTrayActivated(QSystemTrayIcon::ActivationReason)));


//main widget
window = new QWidget(this);
  //      setCentralWidget(window);
window->setAccessibleName("window");
        resize(800,400);
        window->hide();

        // Prime TabWigdet provides space for tabs
        TabWidget = new QTabWidget( window );

        TabWidget->setSizePolicy ( QSizePolicy::Expanding, QSizePolicy::Expanding);
            connect(TabWidget, SIGNAL ( currentChanged( int )), this, SLOT(currentTabChanged(int)));
        setCentralWidget(TabWidget);
            tabService = new QWidget( TabWidget );
                tabService->setAccessibleName("Service");
                TabWidget->insertTab( 0, tabService, QString ("Service") );
            tabDowntime = new QWidget( TabWidget );
            tabDowntime->setAccessibleName("Downtime");
                TabWidget->insertTab( 1, tabDowntime, ("Downtime") );
            tabLog = new QWidget( TabWidget );
                tabLog->setAccessibleName("Log");
                TabWidget->insertTab( 2, tabLog, ("Log") );
            tabHostgroup = new QWidget( TabWidget );
                tabHostgroup->setAccessibleName("Hostgroup");
                TabWidget->insertTab( 3, tabHostgroup, ("Hostgroup") );
            tabInventar = new QWidget( TabWidget );
                tabInventar->setAccessibleName("Inventory");
                TabWidget->insertTab( 4, tabInventar, ("Inventory") );
            tabGeneration = new QWidget( TabWidget );
                tabGeneration->setAccessibleName("Generation");
                TabWidget->insertTab( 5, tabGeneration, ("Generation") );
            tabConfig = new QWidget( TabWidget );
                tabConfig->setAccessibleName("Config");
                TabWidget->insertTab( 6, tabConfig, ("Config") );

// Popup for file stuff

        QMenu* fileMenu = menuBar()->addMenu(tr("F&ile"));

        QAction* updateAction   = new QAction("&Update", window);
        updateAction->setShortcut(Qt::Key_Space);
            connect(updateAction, SIGNAL(triggered(bool)), this, SLOT(writeCleanServiceSlot()));
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


        QMenu* viewMenu = menuBar()->addMenu(tr("&View"));

        okAction   = new QAction("&Ok", window);
            okAction->setObjectName("okAction");
            okAction->setCheckable ( true );
          //  okAction->setChecked ( false );
            okAction->setShortcut(Qt::CTRL + Qt::Key_O);
                connect(okAction, SIGNAL(triggered(bool)), this, SLOT(changeFilters(bool)));
        viewMenu->addAction(okAction);
        okAction->setVisible(false);

        warnAction   = new QAction("&Warning", window);
            warnAction->setObjectName("warnAction");
            warnAction->setCheckable ( true );
          //  warnAction->setChecked ( true );
            warnAction->setShortcut(Qt::CTRL + Qt::Key_W);
                connect(warnAction, SIGNAL(triggered(bool)), this, SLOT(changeFilters(bool)));
        viewMenu->addAction(warnAction);

        critAction   = new QAction("&Critical", window);
            critAction->setObjectName("critAction");
            critAction->setCheckable ( true );
           // critAction->setChecked ( true );
            critAction->setShortcut(Qt::CTRL + Qt::Key_C);
                connect(critAction, SIGNAL(triggered(bool)), this, SLOT(changeFilters(bool)));
        viewMenu->addAction(critAction);

        viewMenu->addSeparator ();

        ackAction   = new QAction("&Acknowledged", window);
            ackAction->setObjectName("ackAction");
            ackAction->setCheckable ( true );
          //  ackAction->setChecked ( true );
            ackAction->setShortcut(Qt::CTRL + Qt::Key_A);
                connect(ackAction, SIGNAL(triggered(bool)), this, SLOT(changeFilters(bool)));
        viewMenu->addAction(ackAction);

        notAction   = new QAction("&Notifications", window);
            notAction->setObjectName("notAction");
            notAction->setCheckable ( true );
           // notAction->setChecked ( true );
            notAction->setShortcut(Qt::CTRL + Qt::Key_N);
                connect(notAction, SIGNAL(triggered(bool)), this, SLOT(changeFilters(bool)));
        viewMenu->addAction(notAction);

        downAction   = new QAction("&Downtime", window);
            downAction->setObjectName("downAction");
            downAction->setCheckable ( true );
          //  downAction->setChecked ( false );
            downAction->setShortcut(Qt::CTRL + Qt::Key_D);
                connect(downAction, SIGNAL(triggered(bool)), this, SLOT(changeFilters(bool)));
        viewMenu->addAction(downAction);

        stateAction   = new QAction("&Hardstate", window);
            stateAction->setObjectName("stateAction");
            stateAction->setCheckable ( true );
          //  stateAction->setChecked ( false );
            stateAction->setShortcut(Qt::CTRL + Qt::Key_H);
                connect(stateAction, SIGNAL(triggered(bool)), this, SLOT(changeFilters(bool)));
        viewMenu->addAction(stateAction);

        QMenu* aboutMenu = menuBar()->addMenu(tr("&?"));
            QAction* iconAction   = new QAction("&Help", window);
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
        connect(serviceTree, SIGNAL(setStatusBar(QString, int)), this, SLOT(setStatusBarSlot(QString, int)));
        connect(serviceTree, SIGNAL(signalLogSearch(QString)), this, SLOT(logSearch(QString)));
        connect(serviceTree, SIGNAL(signalHostDetail(QString)), this, SLOT(requestHostDetail(QString)));
        connect(serviceTree, SIGNAL(signalServiceDetail(QString, QString)), this, SLOT(requestServiceDetail(QString, QString)));
        connect(serviceTree, SIGNAL(signalSetTraySystemIcon(int)), this, SLOT(setTraySystemIcon(int)));
        connect(serviceTree, SIGNAL(changeTimer(bool)), this, SLOT(changeTimer(bool)));
        connect(serviceTree,SIGNAL(foundIncident()), this, SLOT(foundIncident()));
        connect(serviceTree,SIGNAL(sendLiveStatusData(QString, QString)),this,SLOT(sendLiveStatusData(QString,QString)));
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
        hostgroupTree->setColumnCount(6);
        hostgroupTree->setContextMenuPolicy(Qt::CustomContextMenu);
        hostgroupTree->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
        connect(hostgroupTree, SIGNAL(itemDoubleClicked ( QTreeWidgetItem *, int )), this, SLOT(hostgroupItemDoubleClicked ( QTreeWidgetItem *, int)));
        headerLabelsHostgroup << "Group" << "Hosts" << "Count" <<  "Down" << "Services" << "sorter"<<"";
                hostgroupTree->hideColumn(1);
            hostgroupTree->hideColumn(3);
            hostgroupTree->hideColumn(5);
            hostgroupTree->setHeaderLabels(headerLabelsHostgroup);
        hostgroupTree->setSortingEnabled (true);
    hostgroupGrid->addWidget(hostgroupTree,0,0,1,4);

    hostgroupGrid->addWidget(new QLabel("Search:"),1,0);

    searchHostgroupHost = new QLineEdit(tabHostgroup);
        connect( searchHostgroupHost, SIGNAL( returnPressed()) , this, SLOT( searchHostgroup() ) );
        connect(searchHostgroupHost, SIGNAL(textChanged(QString)),this, SLOT(searchHostgroupHostChanged(QString)));
        hostgroupGrid->addWidget(searchHostgroupHost,1,1);

    searchHostgroupHostButton = new QPushButton("Search", tabHostgroup);
        connect( searchHostgroupHostButton, SIGNAL( pressed()) , this, SLOT( searchHostgroup() ) );
        hostgroupGrid->addWidget(searchHostgroupHostButton,1,2);

        hostgroupGrid->addItem(new QSpacerItem ( 100, 0, QSizePolicy::Expanding, QSizePolicy::Fixed ),1,3);

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

    //GENERATION
    QGridLayout* generationGrid = new QGridLayout(tabGeneration);
    generationTreeWidget = new GenerationTreeWidget(tabGeneration);
        //connect(inventarTree, SIGNAL(itemDoubleClicked ( QTreeWidgetItem *, int )), this, SLOT(inventarItemDoubleClicked ( QTreeWidgetItem *, int)));
        connect(generationTreeWidget, SIGNAL( requestLiveStatusData(int,QString, QString)), this, SLOT(childRequestLiveStatusData(int,QString, QString)));
        generationGrid->addWidget(generationTreeWidget,0,0);

        //connect( pcDialog, SIGNAL(doubleClickedHost(QString)) , this, SLOT( slotDoubleClickedHost(QString) ) );

        //generationGrid->addItem(new QSpacerItem ( 100, 0, QSizePolicy::Expanding, QSizePolicy::Fixed ),0,1);
    //END GENERATION


    //CONFIG
        configWidget = new ConfigWidget(tabConfig);
            connect(configWidget,SIGNAL(setHiddenColumns(int)),serviceTree, SLOT(setHiddenColumns(int)));
            connect(configWidget,SIGNAL(setStatusBar(QString, int)), this, SLOT(setStatusBarSlot(QString,int)));
            connect(configWidget,SIGNAL(configChanged()) , this, SLOT( configSubmitSlot() ) );
            connect(configWidget,SIGNAL(writeSettings(QSettings*)), this, SLOT(writeSettings(QSettings*)));
            connect(configWidget,SIGNAL(loadSettings(QSettings*)), this, SLOT(loadSettings(QSettings*)));
    // End of Config


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

    //Stop blocked window on slow responses, first build window
    update();

    bool goodconfig=configWidget->loadConfig();

    //set local Service Filter, you will get them from Config
    setServiceFilters(configWidget->getServiceFilters());
    if (goodconfig)
    {
        configSubmitSlot();
    }
    //Lets remove hidden Tabs from Config
    QString configHideTabString=configWidget->getConfigHideTabString();
    if (!configHideTabString.isEmpty())
    {
        QString hideTabs=configHideTabString.split('=')[1];
        for (int i=0;i<hideTabs.split(',').size();i++)
        {
            QString tabName=hideTabs.split(',').at(i);
            //lets get trou the tabs and remove if found
            for (int j=0;j<TabWidget->count();j++)
            {
                if (tabName.toLower() == TabWidget->widget(j)->accessibleName().toLower())
                {
                    TabWidget->removeTab(j);
                }
            }
        }
    }


    qDebug() << "configWidget->getServiceFilters()" << configWidget->getServiceFilters();
    aufbau=true;



}

Whassup::~Whassup()
{
    iconTray->setVisible(false);
    iconTray->setIcon(QPixmap());

}


/////////////////////////////////////////


void Whassup::writeSettings(QSettings* settings)
{
    settings->beginGroup("MainWindow");
        settings->setValue("Geometry", geometry());
    settings->endGroup();
    settings->sync();
    serviceTree->writeSettings(settings);
}

void Whassup::loadSettings(QSettings* settings)
{
    settings->beginGroup("MainWindow");
        QRect rect = settings->value("Geometry").toRect();
        if (!rect.isEmpty())
        {
            // lets see, if we saved a rect outside current display
            QRect displayRect = QDesktopWidget().availableGeometry();
            if (displayRect.contains(rect))
            setGeometry(rect);
        }
    settings->endGroup();
    serviceTree->loadSettings(settings);
    writeServiceSlot();
}
void Whassup::findItem()
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

void Whassup::searchTrees()
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

void Whassup::newIconExplanation()
{
    HelpWidget *helpWidget = new HelpWidget(this, Qt::Dialog);
    helpWidget->show();

    //show all columns
    /*for (int i=0;i<serviceTree->columnCount();i++)
    {
        serviceTree->showColumn(i);
    }*/
}

void Whassup::inventarExpandSlot()
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


void Whassup::searchConfigFile()
{
    QFileDialog *configFile = new QFileDialog(this, tr("Open Config"), QString(QStandardPaths::HomeLocation), tr("Config Files (*.cfg *.conf)"));
        connect(configFile, SIGNAL(currentChanged(QString)), this, SLOT(loadConfig(QString)));
        configFile->show();

}

void Whassup::changeFilters(bool checked)
{
    (void) checked;
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

    getServiceFilters();
    writeServiceSlot();
}

void Whassup::updateSlot()
{
        tabConfig->show();
}

void Whassup::serviceItemDoubleClicked ( QTreeWidgetItem * item, int column )
{
    if (!item->text(column).trimmed().isEmpty())
    {
        if (column==0)
        {
            requestHostDetail(item->text(0));
        }
        else
        {
            if ( (!item->text(0).trimmed().isEmpty()) && (!item->text(1).trimmed().isEmpty()))
            requestServiceDetail(item->text(0),item->text(1));
        }
    }
}

void Whassup::inventarItemDoubleClicked ( QTreeWidgetItem * item, int column )
{
    //QDesktopServices::openUrl(QUrl(QString(inputCgiURL->text() + item->text(0))));
    if (column==0)
    {
        if (!item->text(0).trimmed().isEmpty())
        requestHostDetail(item->text(0));
    }
    else
    {
        if ( (!item->text(0).trimmed().isEmpty()) && (!item->text(1).trimmed().isEmpty()))
            requestServiceDetail(item->text(0),item->text(1));
    }
}

void Whassup::downtimeItemDoubleClicked ( QTreeWidgetItem * item, int column )
{
    if (column==0)
    {
        if (!item->text(0).trimmed().isEmpty())
        requestHostDetail(item->text(0));
    }
    else
    {
        if ( (!item->text(0).trimmed().isEmpty()) && (!item->text(1).trimmed().isEmpty()))
            requestServiceDetail(item->text(0),item->text(1));
    }
}

void Whassup::hostgroupItemDoubleClicked ( QTreeWidgetItem * item, int column )
{
    (void) column;
    //QDesktopServices::openUrl(QUrl(QString(inputCgiURL->text() + item->text(0))));
    if (!item->text(1).trimmed().isEmpty())
    requestHostDetail(item->text(1));
}

void Whassup::logItemDoubleClicked ( QTreeWidgetItem * item, int column )
{
    if (column==2)
    {
        if (!item->text(2).trimmed().isEmpty())
        requestHostDetail(item->text(2));
    }
    else if (column==3)
    {
        if ( (!item->text(2).trimmed().isEmpty()) && (!item->text(3).trimmed().isEmpty()))
            requestServiceDetail(item->text(2),item->text(3));
    }
}

void Whassup::configSubmitSlot()
{
    serviceTree->clearTree();

    timer->setInterval(configWidget->getTimerValue()*1000);

    configWidget->writeConfig();

    changeFilters(true);

    QFont font;
    font.fromString (configWidget->getFont());
    serviceTree->setFont (font);
    downtimeTree->setFont(font);
    inventarTree->setFont(font);
    hostgroupTree->setFont(font);
    generationTreeWidget->setFont(font);
    TabWidget->setCurrentIndex(0);

    //enable agaian after error
    timer->start();
    aufbau=true;
    writeServiceSlot();
}

void Whassup::writeServiceSlot()
{
    qDebug() << "write Service Slot";
    // we do this in send again
    if (aufbau==false)
        return;


    qDebug()<< "writeServiceSlot";
 //   QString state="Filter: state != 0\n";
    timer->start();
    serviceTree->setSortingEnabled (false);
    //serviceTree->clear();

            LiveSocketRequest ls;
            ls.setTable("services");
            ls.addColumns(QStringList() << "host_name" << "description" << "state" << "last_state_change" << "state_type" << "acknowledged" << "notifications_enabled" << "plugin_output" << "downtimes" << "scheduled_downtime_depth");
            ls.addColumns(QStringList() << "host_state" << "host_state_type" << "host_acknowledged" << "host_downtimes" << "host_scheduled_downtime_depth" << "contacts" << "host_address" << "host_notifications_enabled" << "active_checks_enabled" << "host_active_checks_enabled" );
            ls.addRawText(Filter);
            ls.addRawText(configWidget->getFilters());
            ls.setOutputFormat("json");
            ls.setAuthUser(configWidget->getAuthUser());
            QString wS=ls.getLiveStatusRequest();
           // qDebug() << "LS String" << wS; //.replace("\n","|\n");

    QDateTime dt = QDateTime::currentDateTime ();
    //statusBar()->showMessage( "Updated on: " + dt.toString("hh:mm") );

        sendLiveStatusData("Service", wS);
}

void Whassup::closeEvent( QCloseEvent* ce )
{

    // We want to exit
    if (qApp->property("ClientHideOnClose").toBool() == false)
    {
        qDebug() << "receive closeevent" << qApp->property("ClientHideOnClose").toBool();
        qApp->setQuitOnLastWindowClosed(true);
        qApp->activeWindow()->hide();
        ce->accept();
    }

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
void Whassup::changeEvent(QEvent * event)
{
   if( event->type() == QEvent::WindowStateChange )
   {
        if( isMinimized() )
        {
            qApp->setQuitOnLastWindowClosed(false);
              QMetaObject::invokeMethod(this, "hide", Qt::QueuedConnection);
        }
        else
        {
            qApp->setQuitOnLastWindowClosed(true);
        }
   }
}

void Whassup::aktualisierungChanged(int time)
{
    timer->setInterval(time*1000);
}

void Whassup::errorOccured(QString errorString, int errorLevel)
{
    qDebug() << "void Whassup::errorOccured(QString errorString)";
    timer->stop();
    haserror=true;

    statusBar->setMessage( "Error: " + errorString, errorLevel );

}

void Whassup::inventarUpdateSlot()
{
    inventarTree->clear();
    //currentTabChanged(4);
    currentTabChangedByName("Inventory");
}

void Whassup::currentTabChangedByName(QString tabName)
{
    // We have to give tabs names, so we can remove (hide) dome in configuration
    // and its safer, i just forget all the numbers for tabs
    for (int i=0;i<TabWidget->count();i++)
    {
        qDebug() << "Search in" << TabWidget->widget(i)->accessibleName() << "for"<< tabName <<i;
        if (tabName.toLower() == TabWidget->widget(i)->accessibleName().toLower())
        {
            currentTabChanged(i);
            TabWidget->setCurrentIndex(i);
        }
    }
}

void Whassup::currentTabChanged(int tab)
{
    //nicht beim ersten mal oder fehler ;o)
    if (!aufbau)
    {
        return;
    }


   // qDebug() << "tab" << TabWidget->widget(tab)->accessibleName();
   // return;
    QString tabName=TabWidget->widget(tab)->accessibleName().toLower();
    QString Purpose;
    QString wS;
    LiveSocketRequest ls;
    QString Format;
      qDebug()<<"Tab"<<tab;


    //we are not in the service Tree, lets stop the timer
    if(tabName != QString("Service").toLower())
    {
        timer->stop();
    }
    else
    {
        timer->start();
    }

    if (tabName == QString("Service").toLower())
    {
        // Service seperate
        changeFilters(true);
        return;
    }
    else if (tabName == QString("Downtime").toLower())
    {
        ls.setTable("downtimes");
        ls.addColumns(QStringList() << "host_name" << "service_description" << "start_time" << "end_time" << "author" << "comment");
        ls.setOutputFormat("json");
        ls.setAuthUser(configWidget->getAuthUser());
        wS=ls.getLiveStatusRequest();
        Purpose="Downtime";
        Format="json";
    }
    else if (tabName == QString("Log").toLower())
    {
        ls.setTable("log");
        ls.addColumns(QStringList() << "time state" << "current_host_name" << "current_service_description" << "message");
        if (!searchLogHost->text().isEmpty())
        {
            ls.addFilter("host_name = " + searchLogHost->text());
        }
        ls.setLimit(configWidget->getLogLines());
        ls.setOutputFormat("json");
        ls.setAuthUser(configWidget->getAuthUser());
        wS=ls.getLiveStatusRequest();
        Purpose="Log";
        Format="json";
    }
    else if (tabName == QString("Hostgroup").toLower())
    {
        //hostgroups
        ls.setTable("hostgroups");
        ls.addColumns(QStringList() << "alias" << "num_hosts" << "num_hosts_up" << "members_with_state" << "num_services" << "num_services_ok");
        ls.setOutputFormat("json");
        ls.setAuthUser(configWidget->getAuthUser());
        wS=ls.getLiveStatusRequest();
        Purpose="Hostgroup";
        Format="json";
    }
    else if (tabName == QString("Inventory").toLower())
    {
        //really faster
        inventarTree->setSortingEnabled (false);
        //qDebug() << "inventarTree->topLevelItemCount()" << inventarTree->topLevelItemCount();
        // dont update if you aready got data
        if (!inventarTree->topLevelItemCount() > 0)
        {
            ls.setTable("services");
            ls.addColumns(QStringList() << "host_name" << "description" << "check_interval" << "retry_interval" << "checks_enabled" << "flap_detection_enabled" << "max_check_attempts" << "check_period" << "contacts");
            ls.setOutputFormat("json");
            ls.setAuthUser(configWidget->getAuthUser());
            wS=ls.getLiveStatusRequest();
            Purpose="Inventar";
            Format="json";
        }
        else
        {
            return;
        }
    }
    else if (tabName == QString("Generation").toLower())
    {
        ls.setTable("hosts");
        ls.addColumns(QStringList() << "host_name" << "parents" << "childs" << "state" << "worst_service_state");
        ls.setOutputFormat("json");
        ls.setAuthUser(configWidget->getAuthUser());
        wS=ls.getLiveStatusRequest();
        Purpose="Generation";
        Format="json";
    }
    else if (tabName == QString("Config").toLower())
    {
        ls.setTable("services");
        ls.addColumns(QStringList() << "description" << "host_name" << "host_groups");
        ls.setOutputFormat("json");
        ls.setAuthUser(configWidget->getAuthUser());
        wS=ls.getLiveStatusRequest();
        Purpose="Config";
        Format="json";
    }
    sendLiveStatusData(Purpose, wS);

}

void Whassup::receiveLiveStatusJSONArray(QString Purpose,QJsonArray jsonArray)
{
    statusBar->clearLabel();

    if (Purpose == "HostDetail")
    {
        HostInfoWidget* hostInfoWidget = new HostInfoWidget(jsonArray,configWidget->getTimeFormat(), configWidget->getTimezone(),configWidget->getServerCgiUrl(), getUniqObjectID(), this,Qt::Dialog);
            connect(hostInfoWidget, SIGNAL( doubleClickedHost(QString)), this, SLOT(requestHostDetail(QString)));
            connect(hostInfoWidget, SIGNAL( doubleClickedService(QString,QString)), this, SLOT(requestServiceDetail(QString,QString)));
            connect(hostInfoWidget, SIGNAL( requestLiveStatusData(int,QString, QString)), this, SLOT(childRequestLiveStatusData(int,QString, QString)));
            connect(hostInfoWidget, SIGNAL( logSearch(QString)), this, SLOT(logSearch(QString)));
            connect(hostInfoWidget,SIGNAL(sendLiveStatusData(QString,QString)),this, SLOT(sendLiveStatusData(QString,QString)));
            hostInfoWidget->show();
        hostInfoWidgetList.append(hostInfoWidget);
    }
    else if (Purpose == "ServiceDetail")
    {
        ServiceInfoWidget* serviceInfoWidget = new ServiceInfoWidget(jsonArray,configWidget->getTimeFormat(), configWidget->getTimezone(),configWidget->getServerCgiUrl(), configWidget->getLogLines(), getUniqObjectID()+10000, this,Qt::Dialog);
            connect(serviceInfoWidget, SIGNAL( requestLiveStatusData(int,QString, QString)), this, SLOT(childRequestLiveStatusData(int,QString, QString)));
            connect(serviceInfoWidget, SIGNAL( closing(QString)), this, SLOT(childClosing(QString)));
            connect(serviceInfoWidget,SIGNAL(sendLiveStatusData(QString,QString)),this, SLOT(sendLiveStatusData(QString,QString)));

            serviceInfoWidget->requestLog();
            serviceInfoWidget->show();
        serviceInfoWidgetList.append(serviceInfoWidget);
    }
    else if (Purpose == "Service")
    {
        serviceTree->gotServiceDataJson(jsonArray);
    }
    else if (Purpose == "Generation")
    {
        generationTreeWidget->gotGenerationData(jsonArray);
    }
    else if (Purpose == "Log")
    {
        gotLogDataJson(jsonArray);
    }
    else if (Purpose == "Downtime")
    {
        gotDowntimeDataJson(jsonArray);
    }
    else if (Purpose == "Hostgroup")
    {
        gotHostgroupDataJson(jsonArray);
    }
    else if (Purpose == "Config")
    {
        configWidget->gotConfigDataJson(jsonArray);
    }
    else if (Purpose == "Inventar")
    {
        gotInventarDataJson(jsonArray);
    }
    else
    {
        bool ok;
        int value = Purpose.toInt(&ok);
        // Its a number, so it must be a child request to Livestatus
        // currently we have just one request Object, so lets start from 0 (later use 10000++)
        if (ok)
        {
            qDebug()<< "Purpose value" << Purpose << value;
            if ( (value > 0) && (value < 9999) )
            {
                 //its a hostdetail request
                qDebug() << "Its a child request for"<< Purpose;
                for (int j=0;j<hostInfoWidgetList.size();j++)
                {
                    HostInfoWidget* child = hostInfoWidgetList.at(j);
                    if (child->getUniqObjectID() == value)
                    {
                        child->gotLiveStatusJson(jsonArray);
                    }
                }
            }
            else if ( (value > 9999) && (value < 19999) )
            {
                // its a service detail request
               for (int j=0;j<serviceInfoWidgetList.size();j++)
               {
                   ServiceInfoWidget* child = serviceInfoWidgetList.at(j);
                   if (child->getUniqObjectID() == value)
                   {
                       child->gotLiveStatusJson(jsonArray);
                   }
               }
            }
            else if (value == 55555)
            {
                generationTreeWidget->gotLiveStatusJson(jsonArray);
            }
        }
    }
}


void Whassup::gotInventarDataJson(QJsonArray jsonUpperArray)
{
/*
host_name
description
check_interval
retry_interval
checks_enabled
flap_detection_enabled
max_check_attempts
check_period contacts
*/
    inventarTree->clear();
    QList<QTreeWidgetItem *> items;
    qDebug() << jsonUpperArray;

        for (int k = 0; k < jsonUpperArray.size()-1; ++k)
        {
            QJsonArray jsonArray=jsonUpperArray.at(k).toArray();
            QString jhost_name=jsonArray.at(0).toString();
            QString jdescription=jsonArray.at(1).toString();

            QList<QTreeWidgetItem *> hostList;
            hostList=inventarTree->findItems(jhost_name,Qt::MatchExactly,0);

            QStringList itemValues;
            itemValues << jhost_name << jdescription;
            for (int j=2;j<7;j++)
            {
                   itemValues << QString("%1").arg(jsonArray.at(j).toDouble());
            }
            itemValues << jsonArray.at(7).toString();

            //contacts
            QJsonArray contactsJsonArray=jsonArray.at(8).toArray();
            QString contactsString="";
            for (int l=0;l<contactsJsonArray.size();l++)
            {
                contactsString.append(contactsJsonArray.at(l).toString() +",");
            }
            if (contactsString.endsWith(","))
                itemValues<<contactsString.left(contactsString.size()-1);
            else
                itemValues<<contactsString;
            QTreeWidgetItem* Item = new QTreeWidgetItem(itemValues,QTreeWidgetItem::Type);


                if (hostList.size() > 0)
                    hostList.first()->addChild(Item);
                else
                    items.append(Item);

        //inventarTree->insertTopLevelItems(logTree->topLevelItemCount (), items);
        inventarTree->insertTopLevelItems(0, items);
        inventarTree->update();
        }

    inventarTree->sortByColumn(0,Qt::AscendingOrder);
    inventarTree->setSortingEnabled (true);
}

void Whassup::aboutslot()
{
    QString abouttext;
    abouttext ="Whassup\n";
    abouttext+="Version: "+ QString("%1").arg(VERSION) +"\n";
    abouttext+="\n\n";
    abouttext+="mailto:Web@ReneStorm.de for questions or comments\n";
    abouttext+="Copyright 2011 by Rene Storm\n";
    abouttext+="\n";
    abouttext+="License: GPL 3\n";
    abouttext+="Whassup is written in QT and compiled with mingw\n";
    abouttext+="Please take a look on qt-project.org for licenses\n";
    abouttext+="\n";
    abouttext+="BDR 529\n";
            QMessageBox::about(this,"About",abouttext );
    okAction->setVisible(true);
}

void Whassup::systemTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::DoubleClick)
    {
        raise();
        showNormal();
        activateWindow();
    }

    blinkTimer->stop();
    QPixmap mwIcon;
    mwIcon = drawSystemTrayIcon(QString("%1").arg(lastBadStates));
    iconTray->setIcon(mwIcon);
}

QPixmap Whassup::drawSystemTrayIcon(QString text)
{
   // qDebug() << "drawSystemTrayIcon(QString text)" << text;
    QPixmap pixmap(":/images/critical.png");
    QPainter painter;
    painter.begin(&pixmap);
        QFont font=painter.font() ;
        font.setWeight(QFont::DemiBold);
        painter.setFont(font);
        painter.drawText(0,0,16,16,Qt::AlignCenter,text);
    painter.end();

    return pixmap;
}

void Whassup::foundIncident()
{
    // wefound a new Incident, let trayicon blink
    blinkTimer->start(500);
}

void Whassup::blinkTrayIcon()
{
    QPixmap mwIcon;
    if (blinkInterator)
    {
        mwIcon = drawSystemTrayIcon(QString("%1").arg(lastBadStates));
        blinkInterator=false;
    }
    else
    {
        mwIcon=QPixmap(":/images/critical.png");
        blinkInterator=true;
    }
    iconTray->setIcon(mwIcon);
    //setWindowIcon(mwIcon);
}


void Whassup::logUpdateSlot()
{
    //currentTabChanged(2);
    currentTabChangedByName("Log");
}

void Whassup::logSearch(QString host)
{
    qDebug() << "Searching for host in log" << host;
    searchLogHost->setText(host);
    currentTabChangedByName("Log");
    //TabWidget->setCurrentIndex(2);
}


//void Whassup::sendLiveStatusData(QString Purpose, QString Format, QString HostName, int Port, QString request)
void Whassup::sendLiveStatusData(QString Purpose, QString request)
{
    qDebug() << "sendLiveStatusData aufbau" << aufbau;
    if (aufbau==false) return;
    requestCounter++;
    qDebug() << "requestCounter"<<requestCounter;
    //qDebug() << "sending to Livestatus" << Purpose << HostName << Port << request;
    statusBar->setMessage("Loading...",0);


    QString HostName=qApp->property("ServerHostname").toString();
    int Port=qApp->property("ServerPort").toInt();
    QString Format="json";

    if (qApp->property("ServerCommandType").toInt() == 0)
        //if (configWidget->getServerSsl() == "true")
        if (1 == 2) // ssl not running jet
        {
            LiveSocketSsl* ls = new LiveSocketSsl( Purpose, Format, HostName, Port, request, this );
                connect( ls, SIGNAL( sendLiveStatusJSONArray(QString,QJsonArray)) , this, SLOT( receiveLiveStatusJSONArray(QString,QJsonArray)) );
                connect( ls, SIGNAL( errorLiveSocket(QString, int)), this, SLOT( errorOccured(QString, int) ) );
            ls->connectLiveSocket();
        }
        else
        {
            LiveSocket* ls = new LiveSocket( Purpose, Format, HostName, Port, request, this );
                connect( ls, SIGNAL( sendLiveStatusJSONArray(QString,QJsonArray)) , this, SLOT( receiveLiveStatusJSONArray(QString,QJsonArray)) );
                connect( ls, SIGNAL( errorLiveSocket(QString, int)), this, SLOT( errorOccured(QString ,int) ) );
            ls->connectLiveSocket();
    }
    else if (qApp->property("ServerCommandType").toInt() == 1)
    {
        WebCommunication* wc = new WebCommunication(this);
            wc->getJsonData(Purpose, request);
                connect( wc, SIGNAL( sendWebCommunicationJSONArray(QString, QJsonArray)) , this, SLOT( receiveLiveStatusJSONArray(QString, QJsonArray)) );
                connect( wc, SIGNAL( errorWebCommunication(QString, int)), this, SLOT( errorOccured(QString, int) ) );
                connect( wc, SIGNAL( authEntered() ),this, SLOT( authEntered() ));
    }

}

void Whassup::requestHostDetail(QString host)
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
    ls.addColumns(QStringList() << "worst_service_state" << "perf_data" << "state_type" << "notifications_enabled" << "active_checks_enabled");
    ls.addFilter("host_name = " + host);
    ls.setOutputFormat("json");
    ls.setLimit(1);
    ls.setAuthUser(configWidget->getAuthUser());
    QString wS=ls.getLiveStatusRequest();
    QString Purpose="HostDetail";
    sendLiveStatusData(Purpose, wS);
}

void Whassup::requestServiceDetail(QString host, QString description)
{
    qDebug() << "Requesting for serviedetail for:" << host << description;

    LiveSocketRequest ls;
    ls.setTable("services");
    ls.addColumns(QStringList() << "host_name" << "description" << "state" << "plugin_output" << "perf_data");
    ls.addColumns(QStringList() << "comments_with_info" << "downtimes_with_info" << "last_hard_state_change");
    ls.addColumns(QStringList() << "contact_groups" << "contacts" << "groups" << "action_url_expanded");
    ls.addColumns(QStringList() << "notes_url_expanded" << "host_address" << "state_type");
    ls.addColumns(QStringList() << "notifications_enabled" << "host_notifications_enabled" << "active_checks_enabled" << "host_active_checks_enabled");
    ls.addFilter("host_name = " + host);
    ls.addFilter("description = " + description);
    ls.setOutputFormat("json");
    ls.setLimit(1);
    ls.setAuthUser(configWidget->getAuthUser());
    QString wS=ls.getLiveStatusRequest();
    QString Purpose="ServiceDetail";

    sendLiveStatusData(Purpose, wS);
}

void Whassup::childRequestLiveStatusData(int uid, QString OutputFormat, QString request)
{
    qDebug() << "childRequestLiveStatusData:" << uid;
    sendLiveStatusData(QString("%1").arg(uid), request);
}






void Whassup::setTraySystemIcon(int countBadStates)
{
    lastBadStates=countBadStates;
    QPixmap mwIcon;
    if (countBadStates > 1)
    {
        mwIcon = drawSystemTrayIcon(QString("%1").arg(countBadStates));
    }
    else
    {
        mwIcon=QPixmap(":/images/ok.png");
    }
    iconTray->setIcon(mwIcon);
    setWindowIcon(mwIcon);
}


void Whassup::gotLogDataJson(QJsonArray jsonUpperArray)
{
    QStringList completerList;
    logTree->clear();
    QList<QTreeWidgetItem *> items;

/*
time
state
current_host_name
current_service_description
message
*/
        QString last_host_name="";
        QString last_service_description="";
        QTreeWidgetItem *lastItem;

        for (int k = 0; k < jsonUpperArray.size(); ++k)
        {
            QJsonArray jsonArray=jsonUpperArray.at(k).toArray();
            qDebug() << jsonArray;


            QStringList itemValues;

            int c=0;
            int jtime=jsonArray.at(c).toDouble();
                itemValues << QString("%1").arg(jtime);
            c++;
            int jstate=jsonArray.at(c).toDouble();
                 itemValues << QString("%1").arg(jstate);
             c++;
             QString jcurrent_host_name=jsonArray.at(c).toString();
                  itemValues << jcurrent_host_name;
                  completerList << jcurrent_host_name;
             c++;
             QString jcurrent_service_description=jsonArray.at(c).toString();
                  itemValues << jcurrent_service_description;
              c++;
              QString jmessage=jsonArray.at(c).toString();
                   itemValues << jmessage;


            QTreeWidgetItem* Item = new QTreeWidgetItem(itemValues,QTreeWidgetItem::Type);


            int epochSecs=jtime;
            // ugly Timezone hack
            QDateTime dt = QDateTime::fromString("01-01-70 00:00:00", "dd-MM-yy HH:mm:ss").addSecs(epochSecs+(3600*configWidget->getTimezone()));
            //Item->setText(0, dt.toString("HH:mm:ss"));
             Item->setText(0,dt.toString(configWidget->getTimeFormat()));

        // lets do the icon for services
            if (jstate == 0)
            {
                Item->setIcon(1, QIcon(QPixmap(ok_xpm)) );
            }
            else if (jstate == 1)
            {
                Item->setIcon(1, QIcon(QPixmap(warning_xpm)) );

            }
            else if (jstate >= 2)
            {
                Item->setIcon(1, QIcon(QPixmap(critical_xpm)) );
            }
            //Status text not needed
            Item->setText(1,"");

            Item->setText(4,jmessage.replace(QRegExp("^\\[\\d{10}\\] "), ""));

            // if service too long
            if (jcurrent_service_description.size()>20)
                Item->setText(3,jcurrent_service_description.left(20) + "...");

            if ( (!last_host_name.isEmpty()) && (!last_service_description.isEmpty()) && (jcurrent_host_name == last_host_name) && (jcurrent_service_description == last_service_description))
            {
                //host exists, append next service
                lastItem->addChild(Item);
            }
            else
            {
                items.append(Item);
                lastItem=Item;
            }

            logTree->insertTopLevelItems(logTree->topLevelItemCount (), items);

            last_host_name=jcurrent_host_name;
            last_service_description=jcurrent_service_description;

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

void Whassup::gotDowntimeDataJson(QJsonArray jsonUpperArray)
{
/*
host_name
service_description
start_time
end_time
author
comment
*/

    downtimeTree->clear();
    QList<QTreeWidgetItem *> items;

         //hostname, size one bigger than nummer -1
        for (int k = 0; k < jsonUpperArray.size(); ++k)
        {
            QJsonArray jsonArray=jsonUpperArray.at(k).toArray();
            QStringList itemValues;

            int c=0;
            QString jhost_name=jsonArray.at(c).toString();
                itemValues << jhost_name;
            c++;
            QString jservice_description=jsonArray.at(c).toString();
                itemValues << jservice_description;
            c++;
            int jstart_time=jsonArray.at(c).toDouble();
            itemValues << QString("%1").arg(jstart_time);
            c++;
            int jend_time=jsonArray.at(c).toDouble();
            itemValues << QString("%1").arg(jend_time);
            c++;
            QString jauthor=jsonArray.at(c).toString();
                itemValues << jauthor;
            c++;
            QString jcomment=jsonArray.at(c).toString();
                itemValues << jcomment;


            QList<QTreeWidgetItem *> hostList;
            hostList=downtimeTree->findItems(jhost_name,Qt::MatchExactly,0);

        QTreeWidgetItem* Item = new QTreeWidgetItem(itemValues,QTreeWidgetItem::Type);

                QDateTime dt = QDateTime::fromString("01-01-70 00:00:00", "dd-MM-yy HH:mm:ss").addSecs(jstart_time+(3600*configWidget->getTimezone()));
        //Item->setText(2,dt.toString("HH:mm dd-MM-yy"));
        Item->setText(2,dt.toString(configWidget->getTimeFormat()));

                dt = QDateTime::fromString("01-01-70 00:00:00", "dd-MM-yy HH:mm:ss").addSecs(jend_time+(3600*configWidget->getTimezone()));
        //Item->setText(3,dt.toString("HH:mm dd-MM-yy"));
        Item->setText(3,dt.toString(configWidget->getTimeFormat()));

        if (hostList.size() > 0)
            hostList.first()->addChild(Item);
        else
            items.append(Item);

        downtimeTree->insertTopLevelItems(0, items);
        }

    downtimeTree->sortByColumn(0,Qt::AscendingOrder);
    //downtimeTree->expandAll ();
}

void Whassup::gotHostgroupDataJson(QJsonArray jsonUpperArray)
{
/*
alias
num_hosts
num_hosts_up
members_with_state
num_services
num_services_ok
*/
    QStringList completerList;
    hostgroupTree->clear();
    QList<QTreeWidgetItem *> items;

         //hostname, size one bigger than nummer -1
        for (int k = 0; k < jsonUpperArray.size(); ++k)
        {
            QJsonArray jsonArray=jsonUpperArray.at(k).toArray();
            QList<QTreeWidgetItem *> hostgroupList;


            int c=0;
            QString jalias=jsonArray.at(c).toString();
            completerList<<jalias;
            c++;
            int jnum_hosts=jsonArray.at(c).toDouble();
            c++;
            int jnum_hosts_up=jsonArray.at(c).toDouble();
            c++;
            QJsonArray memberJsonArray=jsonArray.at(c).toArray();
            c++;
            int jnum_services=jsonArray.at(c).toDouble();
            c++;
            int jnum_services_ok=jsonArray.at(c).toDouble();

            //Insert a Hostgroup with a empty member
            //QString servicesState=QString("%1").arg(jnum_services) + ", " + QString("%1").arg(jnum_services-jnum_services_ok);
            QStringList topItemList;
            QString sortString=QString("%1").arg(9999-(jnum_hosts-jnum_hosts_up))+jalias;
            topItemList << jalias << ""<< QString("%1").arg(jnum_hosts) << QString("%1").arg(jnum_hosts-jnum_hosts_up) << "" << sortString;
            QTreeWidgetItem* topItem = new QTreeWidgetItem( topItemList,QTreeWidgetItem::Type);

            QPixmap pix(hostgroupTree->columnWidth(4),20);

            pix.fill(Qt::white);
            QPainter *paint = new QPainter(&pix);
                paint->setPen(Qt::red);
//                QRect redRect(1,1,80,18);
                paint->fillRect(0,0,40,14,QBrush(Qt::green));
  //              paint->drawRect(redRect);
                paint->fillRect(40,0,40,14,QBrush(Qt::red));

                paint->setPen(Qt::black);
                QFont font=paint->font();
                paint->setFont(font);
                font.setWeight(QFont::DemiBold);
                paint->drawText(5,0,32,14,Qt::AlignCenter,QString("%1").arg(jnum_services_ok));
                paint->drawText(45,0,32,14,Qt::AlignCenter,QString("%1").arg(jnum_services-jnum_services_ok));
            delete paint;
            topItem->setBackground(4,QBrush(pix));


            if (jnum_hosts-jnum_hosts_up == 0)
            {
                topItem->setIcon(0, QIcon(QPixmap(ok_xpm)) );
            }
            else
            {
                topItem->setIcon(0, QIcon(QPixmap(critical_xpm)) );

            }
            items.append(topItem);


        // we have to splitt the members
            for (int j = 0; j <= memberJsonArray.size(); ++j)
            {
                QJsonArray memberDetailsJsonArray=memberJsonArray.at(j).toArray();
                qDebug() << memberDetailsJsonArray;
                QString jmember=memberDetailsJsonArray.at(0).toString();
                completerList<<jmember;
                int jmemberstate=memberDetailsJsonArray.at(1).toDouble();
                QString memberSortString=sortString+jmember;

                // add empty hostgroups, but no empty? member, will already inserted this
                if (jmember.isEmpty())
                {
                    continue;
                }

                hostgroupList=hostgroupTree->findItems(jalias,Qt::MatchExactly,0);
// alias member count down
                QStringList itemValues;
                itemValues <<  jmember << "" << "" << "" << ""<< memberSortString;
                QTreeWidgetItem* Item = new QTreeWidgetItem(itemValues,QTreeWidgetItem::Type);
                //QTreeWidgetItem* Item = new QTreeWidgetItem(listOfStringLists.at(k),QTreeWidgetItem::Type);

                if (jmemberstate == 0)
                {
                    Item->setIcon(0, QIcon(QPixmap(ok_xpm)) );
                }
                else
                {
                    Item->setIcon(0, QIcon(QPixmap(critical_xpm)) );

                }
                topItem->addChild(Item);

            //insert toplevels
            hostgroupTree->insertTopLevelItems(0, items);
            }
//            statusBar->clearLabel();
  //          statusBar->setMessage( "Adding " + jalias, 0 );
    //        statusBar->update();

            hostgroupTree->update();
        }

      //  hostgroupTree->insertTopLevelItems(0, items);
        hostgroupTree->sortByColumn(5,Qt::AscendingOrder);

        completerList.removeDuplicates();
        completerList.sort(Qt::CaseInsensitive);
        QCompleter *searchHostgroupCompleter = new QCompleter(completerList, this);
            searchHostgroupCompleter->setCaseSensitivity(Qt::CaseInsensitive);
        searchHostgroupHost->setCompleter(searchHostgroupCompleter);
}

void Whassup::searchHostgroup()
{
    qDebug() << "hostgroupFoundList.size()" << hostgroupFoundList.size() <<searchHostgroupHost->text();
    if (hostgroupFoundList.size()<1)
    {
        searchHostgroupHostButton->setText("Search");
        hostgroupFoundList.append(hostgroupTree->findItems(searchHostgroupHost->text(),Qt::MatchContains|Qt::MatchRecursive,0));
        hostgroupFoundList.append(hostgroupTree->findItems(searchHostgroupHost->text(),Qt::MatchContains|Qt::MatchRecursive,1));
        qDebug() << "hostgroupFoundList.size()" << hostgroupFoundList.size();
    }

    // note else, because we need both on first click
    if (hostgroupFoundList.size()>0)
    {
        searchHostgroupHostButton->setText("Next");
        hostgroupTree->setCurrentItem(hostgroupFoundList.takeFirst());
    }
}

void Whassup::searchHostgroupHostChanged(QString text)
{
    (void) text;
    hostgroupFoundList.clear();
    searchHostgroupHostButton->setText("Search");
}

void Whassup::setServiceFilters(int columnInt)
{

    qDebug()<<"Whassup::setServiceFilters(int columnInt)" << columnInt;
/*
   QAction* okAction;
    QAction* warnAction;
    QAction* critAction;
    QAction* ackAction;
    QAction* notAction;
    QAction* downAction;
    QAction* stateAction;
*/

if (columnInt & 1)
     okAction->setChecked(true);
else
     okAction->setChecked(false);

if (columnInt & 2)
     warnAction->setChecked(true);
else
     warnAction->setChecked(false);

if (columnInt & 4)
     critAction->setChecked(true);
else
     critAction->setChecked(false);

if (columnInt & 8)
     ackAction->setChecked(true);
else
     ackAction->setChecked(false);

if (columnInt & 16)
     notAction->setChecked(true);
else
     notAction->setChecked(false);

if (columnInt & 32)
     downAction->setChecked(true);
else
     downAction->setChecked(false);

if (columnInt & 64)
     stateAction->setChecked(true);
else
     stateAction->setChecked(false);
}

void Whassup::getServiceFilters()
{
int columnInt=0;
if (okAction->isChecked())
    columnInt=columnInt+1;

if (warnAction->isChecked())
    columnInt=columnInt+2;

if (critAction->isChecked())
    columnInt=columnInt+4;

if (ackAction->isChecked())
    columnInt=columnInt+8;

if (notAction->isChecked())
    columnInt=columnInt+16;

if (downAction->isChecked())
    columnInt=columnInt+32;

if (stateAction->isChecked())
    columnInt=columnInt+64;

configWidget->setServiceFilters(columnInt);
qDebug() << "getServiceFilters()" << columnInt;
}

void Whassup::writeCleanServiceSlot()
{
    serviceTree->clearTree();
    aufbau=true;
    timer->start();
    writeServiceSlot();
}
