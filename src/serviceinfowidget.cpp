#include "serviceinfowidget.h"

ServiceInfoWidget::ServiceInfoWidget(QJsonArray jsonUpperArray, QString TimeFormat,int TimeZone, QString ServiceURL,int LogLines,int uniqObjectID, QWidget * parent, Qt::WindowFlags f): QWidget( parent, f )
{
    uid=uniqObjectID;
    SURL=ServiceURL;
    QJsonArray jsonArray=jsonUpperArray.first().toArray();
    Hostname=jsonArray.at(0).toString();
    Description=jsonArray.at(1).toString();
    Host_Address=jsonArray.at(13).toString();
    timeFormat=TimeFormat;
    timeZone=TimeZone;
    logLines=LogLines;
    currentLimit=50;
    ActionUrl=jsonArray.at(11).toString();
    NotesUrl=jsonArray.at(12).toString();
    lastTopLevelState=-1;

    State=jsonArray.at(2).toDouble();
    //State_Type=jsonArray.at(14).toDouble();


    /*
    //ls.addColumns(QStringList() << "host_name" << "description" << "state" << "plugin_output" << "perf_data");
                                        0               1               2           3                   4
    //ls.addColumns(QStringList() << "comments_with_info" << "downtimes_with_info" << "last_hard_state_change" << "contact_groups"  << "contacts" << "groups");
                                        5                           6                       7                       8                       9           10
<< "action_url_expanded" << "notes_url_expanded" << "host_address" << "state_type"
        11              12                              13              14
        << "notifications_enabled" << "host_notifications_enabled" << "active_checks_enabled" << "host_active_checks_enabled");
        15                                  16                              17                      18
    */
    ServiceDetails=0;
    if (jsonArray.at(14).toDouble() == 1) ServiceDetails += 1;
    if (jsonArray.at(15).toDouble() == 1) ServiceDetails += 2;
    if (jsonArray.at(16).toDouble() == 1) ServiceDetails += 4;
    if (jsonArray.at(17).toDouble() == 1) ServiceDetails += 8;
    if (jsonArray.at(18).toDouble() == 1) ServiceDetails += 16;


    resize(640,480);
    setWindowTitle("ServiceInfo for " + Hostname + " " + Description);
    QGridLayout* mainGrid = new QGridLayout(this);

   int k = 0;
   int r=0;
    mainGrid->addWidget(new QLabel("Hostname:",this),r,0);
   QHBoxLayout *hostBox = new QHBoxLayout();
       ResultLabel* L_Hostname = new ResultLabel(Hostname,this);
            L_Hostname->setTextInteractionFlags(Qt::TextSelectableByMouse);
       hostBox->addWidget(L_Hostname);

       ResultLabel* L_Host_Address = new ResultLabel("("+ Host_Address+ ")",this);
            L_Host_Address->setAccessibleName("L_Host_Address");
            L_Host_Address->setTextInteractionFlags(Qt::TextSelectableByMouse);
        hostBox->addWidget(L_Host_Address);
        hostBox->addItem(new QSpacerItem ( 10, 10, QSizePolicy::Expanding, QSizePolicy::Fixed ));

        mainGrid->addLayout(hostBox,r,1,1,2);

   r=1;
   qDebug() <<r;
   mainGrid->addWidget(new QLabel("Description:",this),r,0);
   ResultLabel* L_Description = new ResultLabel(Description,this);
           L_Description->setTextInteractionFlags(Qt::TextSelectableByMouse);
   mainGrid->addWidget(L_Description,r,1,1,2);

   r=2;
   qDebug() <<r;
   mainGrid->addWidget(new QLabel("State:",this),r,0);

   QHBoxLayout *stateLayout=new QHBoxLayout(this);
   QLabel* stateLabel = new QLabel(this);
   if (jsonArray.at(2).toDouble() == 0)
   {
       stateLabel->setPixmap(QPixmap(":/images/ok.png"));
   }
   else if (jsonArray.at(2).toDouble() == 1)
   {
       stateLabel->setPixmap(QPixmap(":/images/warning.png"));

   }
   else if (jsonArray.at(2).toDouble() >= 2)
   {
       stateLabel->setPixmap(QPixmap(":/images/critical.png"));
   }
   stateLayout->addWidget(stateLabel,0);

   // Plugin OUtput


   //QLabel* L_Output = new QLabel(this);
   ResultLabel* L_Output = new ResultLabel(this);
           L_Output->setMinimumWidth(400);
           L_Output->setTextInteractionFlags(Qt::TextSelectableByMouse);
           QString Output=jsonArray.at(3).toString();
           if (Output.size()>60)
           {
               L_Output->setWordWrap(true);
           }
           L_Output->setText(Output);
    stateLayout->addWidget(L_Output,2);
    mainGrid->addLayout(stateLayout,r,1,1,3);
   mainGrid->addItem(new QSpacerItem ( 10, 10, QSizePolicy::Expanding, QSizePolicy::Fixed ),r,4);

   //last_hard_state_change

//Item->setText(3,dt.toString("HH:mm dd-MM-yy"));
   r=3;
   //inputTimezone->value()
   QDateTime lcdt = QDateTime::fromString("01-01-70 00:00:00", "dd-MM-yy HH:mm:ss").addSecs(jsonArray.at(7).toDouble()+(3600*TimeZone));
   mainGrid->addWidget(new QLabel("Last Change:",this),r,0);
   ResultLabel* L_LastChange = new ResultLabel(lcdt.toString(TimeFormat),this);
           L_LastChange->setTextInteractionFlags(Qt::TextSelectableByMouse);
   mainGrid->addWidget(L_LastChange,r,1,1,2);



   //contacts (8) & contact_groups (14)
    r=4;
   QGridLayout* comboGrid=new QGridLayout(this);

   comboGrid->addWidget(new QLabel("Contacts:",this),0,0);
   QComboBox* contactBox = new QComboBox(this);
                contactBox->setMinimumWidth(150);
       QJsonArray jsonContactArray=jsonArray.at(9).toArray();
       for (int i=0;i<jsonContactArray.size();i++)
       {
           contactBox->addItem(jsonContactArray.at(i).toString());
       }
   comboGrid->addWidget(contactBox,0,1);


   comboGrid->addWidget(new QLabel("Groups:",this),1,0);
   QComboBox* contactGroupBox = new QComboBox(this);
       QJsonArray jsonGroupArray=jsonArray.at(8).toArray();
       for (int i=0;i<jsonGroupArray.size();i++)
       {
           contactGroupBox->addItem(jsonGroupArray.at(i).toString());
       }
    comboGrid->addWidget(contactGroupBox,1,1);


   // Hostgroups (17)
   comboGrid->addWidget(new QLabel("Servicegroups:",this),2,0);
   QComboBox* serviceGroupBox = new QComboBox(this);
       QJsonArray jsonServiceGroupArray=jsonArray.at(10).toArray();
       for (int i=0;i<jsonServiceGroupArray.size();i++)
       {
           serviceGroupBox->addItem(jsonServiceGroupArray.at(i).toString());
       }
    comboGrid->addWidget(serviceGroupBox,2,1);
    comboGrid->addItem(new QSpacerItem ( 10, 10, QSizePolicy::Expanding, QSizePolicy::Fixed ),2,2);
    mainGrid->addLayout(comboGrid,r,0,1,3);

    QVBoxLayout *buttonLayout = new QVBoxLayout(this);

    QPushButton* browserUrlButton = new QPushButton("Browser",this);
        browserUrlButton->setIcon(QIcon(":/images/web.png"));
        browserUrlButton->setToolTip("Open in Browser");
       connect( browserUrlButton, SIGNAL( pressed()) , this, SLOT( browserUrlButtonPressed() ) );
    buttonLayout->addWidget(browserUrlButton);
    if (SURL.isEmpty()) browserUrlButton->setVisible(false);

    QPushButton* actionUrlButton = new QPushButton("Action URL", this);
        actionUrlButton->setIcon(QIcon(":/images/action.png"));
        actionUrlButton->setToolTip("Open Action Url");
       connect( actionUrlButton, SIGNAL( pressed()) , this, SLOT( actionUrlButtonPressed() ) );
    buttonLayout->addWidget(actionUrlButton);
    if (ActionUrl.isEmpty()) actionUrlButton->setVisible(false);

    QPushButton* notesUrlButton = new QPushButton("Notes URL", this);
        notesUrlButton->setIcon(QIcon(":/images/notes.png"));
        notesUrlButton->setToolTip("Open Notes Url");
       connect( notesUrlButton, SIGNAL( pressed()) , this, SLOT( notesUrlButtonPressed() ) );
    buttonLayout->addWidget(notesUrlButton);
    if (NotesUrl.isEmpty()) notesUrlButton->setVisible(false);

    buttonLayout->addItem(new QSpacerItem ( 10, 10, QSizePolicy::Fixed, QSizePolicy::Fixed ));

    // we get the combobox from a command widget
    CommandWidget *commandWidget = new CommandWidget(this);
        connect(commandWidget,SIGNAL(sendLiveStatusData(QString,QString)),this,SLOT(slotSendLiveStatusData(QString, QString)));
    buttonLayout->addWidget(commandWidget->getCommandComboBox(Hostname, Description, State, ServiceDetails));

    buttonLayout->addItem(new QSpacerItem ( 10, 10, QSizePolicy::Fixed, QSizePolicy::Expanding ));
    mainGrid->addLayout(buttonLayout,0,6,4,1);



    //Get overallstate
    //worst_service_state (18) + state (4)
    int overallState=jsonArray.at(18).toDouble()+jsonArray.at(4).toDouble();
    qDebug() << "overallState" << overallState;

    r=7;
    mainGrid->addWidget(new QLabel(" ",this),r,0);

    r=8;
    QHBoxLayout *logRowLayout = new QHBoxLayout(this);
        logRowLayout->addWidget(new QLabel("Log:",this));
    //mainGrid->addWidget(new QLabel("Log:",this),r,0);

    comboLogRows=new QComboBox(this);
        comboLogRows->addItem(QString("10 Rows"),QVariant(10));
        comboLogRows->addItem(QString("50 Rows"),QVariant(50));
        comboLogRows->addItem(QString("100 Rows"),QVariant(100));
        comboLogRows->addItem(QString("500 Rows"),QVariant(500));
        comboLogRows->addItem(QString("1000 Rows"),QVariant(1000));
        comboLogRows->setCurrentIndex(1);
        connect( comboLogRows, SIGNAL( currentIndexChanged(int)) , this, SLOT( requestNewLog(int) ) );

        logRowLayout->addWidget(comboLogRows);
       logRowLayout->addItem(new QSpacerItem ( 10, 10, QSizePolicy::Expanding, QSizePolicy::Fixed ));
    mainGrid->addLayout(logRowLayout,r,0,1,2);

    r=9;
    logTimeViewer = new LogView(this);
        logTimeViewer->setMinimumHeight(70);
        logTimeViewer->setMaximumHeight(70);
        logTimeViewer->setInteractive(false);
            logTimeScene = new QGraphicsScene(logTimeViewer);
            logTimeViewer->setScene(logTimeScene);
            connect( logTimeViewer, SIGNAL( doubleClickedLogItem(QPoint)) , this, SLOT( logTreeItemClicked(QPoint)) );

    mainGrid->addWidget(logTimeViewer,r,0,1,7);

    r=10;
    QVBoxLayout *vlayout=new QVBoxLayout(this);
    mainGrid->addLayout(vlayout,r,0,1,7);

    logTree = new QTreeWidget(this);
        logTree->setSelectionMode(QAbstractItemView::SingleSelection);
        logTree->setColumnCount(5);
        logTree->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
        logTree->setColumnCount(2);
   //     logTree->setMinimumWidth(600);
      //  logTree->header()->setSectionResizeMode(QHeaderView::Stretch);
        QStringList headerLabelsService;
        headerLabelsService << "time" << "state" << "hidden_state" << "hidden_time" << "message";
        logTree->setHeaderLabels(headerLabelsService);
        logTree->setSortingEnabled (false);
        logTree->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        logTree->hideColumn(2);
        logTree->hideColumn(3);
   //  mainGrid->addWidget(logTree,r,0,1,9);
        vlayout->addWidget(logTree);
        vlayout->addItem(new QSpacerItem ( 600, 0, QSizePolicy::Expanding, QSizePolicy::Fixed ));


}
ServiceInfoWidget::~ServiceInfoWidget()
{
}

void ServiceInfoWidget::closeEvent( QCloseEvent* ce )
{
    (void) ce;
    qDebug() << "ciao";
    emit closing(QString("%1").arg(uid));
    deleteLater();
}

void ServiceInfoWidget::requestLog()
{
    LiveSocketRequest ls;
    ls.setTable("log");
    ls.addColumns(QStringList() << "time" << "state" << "message");
    ls.addFilter("host_name = " + Hostname);
    ls.addFilter("service_description = " + Description);
    ls.setLimit(currentLimit);
    ls.setOutputFormat("json");
    QString wS=ls.getLiveStatusRequest();

    qDebug() << "requesting Log" << wS;

    emit requestLiveStatusData(uid,"json",wS);

}

void ServiceInfoWidget::gotLiveStatusJson(QJsonArray jsonUpperArray)
{
    qDebug() << "ServiceInfoWidget::gotLiveStatusJson" << jsonUpperArray;

    logTree->clear();
    logItems.clear();
    logTopLevelItems.clear();

    for (int i=0;i<jsonUpperArray.size();i++)
    {
        QJsonArray jsonArray = jsonUpperArray.at(i).toArray();

        // we got no usable answer
        if (jsonArray.size()<2)
            return;

        QDateTime lcdt = QDateTime::fromString("01-01-70 00:00:00", "dd-MM-yy HH:mm:ss").addSecs(jsonArray.at(0).toDouble()+(3600*timeZone));
        int state = jsonArray.at(1).toDouble();
        QString message = jsonArray.at(2).toString();
        qDebug() << lcdt << state << message;

        // some messages have no state
        QString stateString;
        if (QString("%1").arg(state).isEmpty())
        {
            stateString="127";
            state=127;
        }
        else
        {
            stateString=QString("%1").arg(state);
        }

        QStringList tmp;
        tmp << lcdt.toString(timeFormat) << "" << QString("%1").arg(state) << QString("%1").arg((int)jsonArray.at(0).toDouble()) << message.replace(QRegExp("^\\[\\d{10}\\] "), "");
        QTreeWidgetItem *Item = new QTreeWidgetItem(tmp,QTreeWidgetItem::Type);

        // lets do the icon for services
            if (state == 0)
            {
                Item->setIcon(1, QIcon(QPixmap(":/images/ok.png")) );
            }
            else if (state == 1)
            {
                Item->setIcon(1, QIcon(QPixmap(":/images/warning.png")) );

            }
            else if ( (state == 2) || (state == 3) )
            {
                Item->setIcon(1, QIcon(QPixmap(":/images/critical.png")) );
            }

            logItems.append(Item);


            //first row there is no last item
            if (logItems.size()>0)
            {
                qDebug() << "lastTopLevelState" << lastTopLevelState << "cuurentstate" << state;
                if ( lastTopLevelState == state)
                {
                    // state has not changed, add to last top level
                    lastTopLevelItem->addChild(Item);
                }
                else
                {
                    logTree->insertTopLevelItem(logTree->topLevelItemCount(),Item);
                    lastTopLevelItem=Item;
                    lastTopLevelState=state;
                    logTopLevelItems.append(Item);
                }
            }
            else
            {
                logTree->insertTopLevelItem(logTree->topLevelItemCount(),Item);
                lastTopLevelItem=Item;
                lastTopLevelState=state;
                logTopLevelItems.append(Item);
            }

    }

    redrawScene();
}

void ServiceInfoWidget::requestNewLog(int index)
{
    currentLimit=comboLogRows->itemData(index).toInt();
    lastTopLevelState=-1;

    logTree->clear();
    requestLog();
}

void ServiceInfoWidget::browserUrlButtonPressed()
{
    QString url=qApp->property("ClientServiceOpenUrl").toString();
        url.replace("%HOST%",Hostname);
        url.replace("%SERVICE%",Description);
    QDesktopServices::openUrl(url);
}

void ServiceInfoWidget::actionUrlButtonPressed()
{
     QDesktopServices::openUrl(ActionUrl);
     qDebug() << "Action Url" << ActionUrl;
}

void ServiceInfoWidget::notesUrlButtonPressed()
{
     QDesktopServices::openUrl(NotesUrl);
     qDebug() << "Notes Url" <<  NotesUrl;
}

void ServiceInfoWidget::redrawScene()
{
   // if (logItems.size()>2)
   // logTree->setCurrentItem(logItems.at(9));

    logTimeScene->clear();
    logTimeScene->setSceneRect(0,0,logTimeViewer->viewport()->geometry().width()-20,logTimeViewer->viewport()->geometry().height()-10);

    QRectF sr=logTimeScene->sceneRect();

    qDebug() << "left" << left << right << logTimeViewer->viewport()->geometry().width() << sr.width() << sr.height();
    QGraphicsLineItem *timeLine = new QGraphicsLineItem(10,sr.height()-12,sr.width()-10,sr.height()-12,0);
    logTimeScene->addItem(timeLine);

    if (logItems.size()>2)
    {
        stopTime=logItems.first()->text(3).toInt(); //bigger
        startTime=logItems.last()->text(3).toInt(); //smaller
        double diffTime=stopTime-startTime;
        qDebug() << "Timestart" << startTime << "Timestop" << stopTime << "timediff"<< diffTime;

        for (int i=0;i<5;i++)
        {
            QDateTime lcdt = QDateTime::fromString("01-01-70 00:00:00", "dd-MM-yy HH:mm:ss").addSecs(startTime+(i*(diffTime/4)));
            QGraphicsTextItem *text =new QGraphicsTextItem(lcdt.toString("dd/MM"));
            int cx=(10)+((i)*(sr.width()-25)/4)-10;
            text->setPos(cx,sr.height()-15);
            logTimeScene->addItem(text);

            QGraphicsLineItem *dLine = new QGraphicsLineItem(cx+12,sr.height()-14,cx+12,sr.height()-16,0);
            logTimeScene->addItem(dLine);
        }
    //append last item //must not be toplevel
    logTopLevelItems.append(logItems.last());

        for (int i=0;i<logTopLevelItems.size();i++)
        {
            QTreeWidgetItem* currentItem;
            if (logTopLevelItems.at(i)->childCount() > 0)
            {
                currentItem=logTopLevelItems.at(i)->child(logTopLevelItems.at(i)->childCount()-1);
            }
            else
            {
                currentItem=logTopLevelItems.at(i);
            }
            qint32 itemTime=currentItem->text(3).toInt(); // epoch of toplevelitem
            if (itemTime>0)
            {
                // first Item is has stopTime
                double cx;
                if (stopTime!=itemTime)
                {
                    cx=((stopTime)-itemTime)/diffTime;
                }
                else
                {
                    cx=0;
                }
//int cx=40;
//qDebug()<<"itemTime"<<itemTime << cx << diffTime <<(sr.width()-30)*cx <<logTopLevelItems.at(i)->text(0);
            LogViewItem* logViewItem =  new LogViewItem(currentItem->text(2).toInt(),0);

            // lets put ok a bit higher to see them better if overlapping
                int pixHeight;
                if (currentItem->text(2) == "0")
                {
                    pixHeight=sr.height()-40;
                }
                else
                {
                    pixHeight=sr.height()-30;
                }
                logViewItem->setPos((sr.width()-30)-((sr.width()-30)*cx),pixHeight);
                logTimeScene->addItem(logViewItem);
            }
        }
     }
}

void ServiceInfoWidget::resizeEvent(QResizeEvent * event)
{
    (void) event;
    redrawScene();
}

void ServiceInfoWidget::logTreeItemClicked(QPoint p)
{
    int match;
    qint32 xHit=p.x();
    if (xHit<1)
        xHit=1;

    QRectF sr=logTimeScene->sceneRect();
    double factor=xHit/sr.width();
    double clickTime=((stopTime-startTime)*factor)+startTime;

    //qDebug() << "logTreeItemClicked" << sr.width() << xHit << factor << (int)clickTime << "start" << startTime << stopTime;
   // logTree->setCurrentItem(item);
    for (int i=0; i<logItems.size();i++)
    {
        if (logItems.at(i)->text(3).toDouble()<clickTime)
        {
            logTree->setCurrentItem(logItems.at(i));
            break;
        }
    }
}

void ServiceInfoWidget::keyPressEvent(QKeyEvent * event)
{
    //qDebug() << "key pressed" << event->key();
    if (event->key() == Qt::Key_Escape)
    {
        deleteLater();
    }
}

