#include "hostinfowidget.h"
using namespace std;

HostInfoWidget::HostInfoWidget(QJsonArray jsonUpperArray, QString TimeFormat,int TimeZone, QString ServiceURL,int uniqObjectID, QWidget * parent, Qt::WindowFlags f): QWidget( parent, f )
{
    QJsonArray jsonArray = jsonUpperArray.first().toArray();
    uid=uniqObjectID;
    SURL=ServiceURL;  //not needed anymore

    Hostname=jsonArray.at(0).toString();
    NotesUrl=jsonArray.at(11).toString();
    ActionUrl=jsonArray.at(12).toString();
    State=jsonArray.at(4).toDouble();
    State_Type=jsonArray.at(20).toDouble();

    qDebug()<<"hsotname" << Hostname<<NotesUrl<<ActionUrl;

    resize(800,640);
    setWindowTitle("HostInfo for " + Hostname);
    QGridLayout* mainGrid = new QGridLayout(this);

    //Lets get the info, request was:
    //GET hosts\nColumns:host_name alias display_name address state last_time_down comments_with_info services_with_info contacts downtimes_with_info last_hard_state_change notes_url_expanded action_url_expanded plugin_output contact_groups parents childs  host_groups worst_service_state perf_data state_type "notifications_enabled" << "active_checks_enabled"\n
    //                     0       1       2           3       4           5           6                   7                   8       9                   10                      11                  12              13              14              15  16      17          18                  19       20              21                          22
    //Filter: host_name = " + host +" \nLimit: 1\n";

    HostDetails=0;
    if (jsonArray.at(20).toDouble() == 1) HostDetails += 1;
    if (jsonArray.at(21).toDouble() == 1) HostDetails += 4;
    if (jsonArray.at(22).toDouble() == 1) HostDetails += 16;

   int k = 0;
   int r=0;
   mainGrid->addWidget(new QLabel("Hostname:",this),r,0);
   ResultLabel* L_Hostname = new ResultLabel(Hostname,this);
           L_Hostname->setTextInteractionFlags(Qt::TextSelectableByMouse);
   mainGrid->addWidget(L_Hostname,r,1,1,2);

   r=1;
   qDebug() <<r;
   mainGrid->addWidget(new QLabel("Address:",this),r,0);
   ResultLabel* L_Address = new ResultLabel(jsonArray.at(3).toString(),this);
           L_Address->setTextInteractionFlags(Qt::TextSelectableByMouse);
   mainGrid->addWidget(L_Address,r,1,1,2);

   r=2;
   qDebug() <<r;
   mainGrid->addWidget(new QLabel("State:",this),r,0);
   QLabel* stateLabel = new QLabel(this);
   if (jsonArray.at(4).toDouble() == 0)
   {
       stateLabel->setPixmap(QPixmap(":/images/ok.png"));
   }
   else if (jsonArray.at(4).toDouble() == 1)
   {
       stateLabel->setPixmap(QPixmap(":/images/warning.png"));

   }
   else if (jsonArray.at(4).toDouble() >= 2)
   {
       stateLabel->setPixmap(QPixmap(":/images/critical.png"));
   }
   mainGrid->addWidget(stateLabel,r,1);

   // Plugin OUtput

   ResultLabel* L_Output = new ResultLabel(jsonArray.at(13).toString(),this);
           L_Output->setTextInteractionFlags(Qt::TextSelectableByMouse);
   mainGrid->addWidget(L_Output,r,2);
   mainGrid->addItem(new QSpacerItem ( 10, 10, QSizePolicy::Expanding, QSizePolicy::Fixed ),r,3);


   //last_hard_state_change

//Item->setText(3,dt.toString("HH:mm dd-MM-yy"));
   r=3;
   //inputTimezone->value()
   QDateTime lcdt = QDateTime::fromString("01-01-70 00:00:00", "dd-MM-yy HH:mm:ss").addSecs(jsonArray.at(10).toDouble()+(3600*TimeZone));
   mainGrid->addWidget(new QLabel("Last Change:",this),r,0);
   QLabel* L_LastChange = new QLabel(lcdt.toString(TimeFormat),this);
           L_LastChange->setTextInteractionFlags(Qt::TextSelectableByMouse);
   mainGrid->addWidget(L_LastChange,r,1,1,2);



   //contacts (8) & contact_groups (14)
   r=4;
   mainGrid->addWidget(new QLabel("Contacts:",this),r,0);
   QComboBox* contactBox = new QComboBox(this);
       QJsonArray jsonContactArray=jsonArray.at(8).toArray();
       for (int i=0;i<jsonContactArray.size();i++)
       {
           contactBox->addItem(jsonContactArray.at(i).toString());
       }
   mainGrid->addWidget(contactBox,r,1,1,2);

   r=5;
   mainGrid->addWidget(new QLabel("Groups:",this),r,0);
   QComboBox* contactGroupBox = new QComboBox(this);
       QJsonArray jsonGroupArray=jsonArray.at(14).toArray();
       for (int i=0;i<jsonGroupArray.size();i++)
       {
           contactGroupBox->addItem(jsonGroupArray.at(i).toString());
       }
    mainGrid->addWidget(contactGroupBox,r,1,1,2);


   // Hostgroups (17)
   r=6;
   mainGrid->addWidget(new QLabel("Hostgroups:",this),r,0);
   QComboBox* hostGroupBox = new QComboBox(this);
       QJsonArray jsonHostGroupArray=jsonArray.at(17).toArray();
       for (int i=0;i<jsonHostGroupArray.size();i++)
       {
           hostGroupBox->addItem(jsonHostGroupArray.at(i).toString());
       }
    mainGrid->addWidget(hostGroupBox,r,1,1,2);

   r=7;
   mainGrid->addWidget(new QLabel(" ",this),r,0);

   r=8;
   mainGrid->addWidget(new QLabel("Services:",this),r,0);

   r=9;
   //services_with_state (7)
   QTreeWidget* serviceTree = new QTreeWidget(this);
   connect(serviceTree, SIGNAL(itemDoubleClicked ( QTreeWidgetItem *, int )), this, SLOT(serviceItemDoubleClicked ( QTreeWidgetItem *, int)));
       serviceTree->setSelectionMode(QAbstractItemView::SingleSelection);
       serviceTree->setColumnCount(2);
       serviceTree->setMaximumWidth(600);
       serviceTree->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
       QStringList headerLabelsService;
       headerLabelsService << "Service" << "Output";
       serviceTree->setHeaderLabels(headerLabelsService);
       serviceTree->setSortingEnabled (false);
    mainGrid->addWidget(serviceTree,r,0,1,5);


    QJsonArray serviceJsonArray = jsonArray.at(7).toArray();
    qDebug() << "json service size" << serviceJsonArray.size();
    qDebug() << "json service " << serviceJsonArray.first();

    for (int i=0;i<serviceJsonArray.size();i++)
    {
        QJsonArray singleServiceJsonArray = serviceJsonArray.at(i).toArray();
        QStringList tmp;
        tmp << singleServiceJsonArray.at(0).toString() <<  singleServiceJsonArray.at(3).toString();
        qDebug() << "json service list" << singleServiceJsonArray.at(i).toString();
           QTreeWidgetItem* Item = new QTreeWidgetItem(tmp,QTreeWidgetItem::Type);

           if (singleServiceJsonArray.at(1).toDouble() == 0)
           {
               Item->setIcon(0, QIcon(QPixmap(ok_xpm)) );
           }
           else if (singleServiceJsonArray.at(1).toDouble() == 1)
           {
               Item->setIcon(0, QIcon(QPixmap(warning_xpm)) );
           }
           else
           {
               Item->setIcon(0, QIcon(QPixmap(critical_xpm)) );
           }
           serviceTree->insertTopLevelItem(0,Item);

    }

    r=1;
    QPushButton* browserUrlButton = new QPushButton("Browser", this);
       browserUrlButton->setIcon(QIcon(":/images/web.png"));
       browserUrlButton->setToolTip("Open in Browser");
       connect( browserUrlButton, SIGNAL( pressed()) , this, SLOT( browserUrlButtonPressed() ) );
    mainGrid->addWidget(browserUrlButton,r,4);
    if (SURL.isEmpty()) browserUrlButton->setVisible(false);

    r=2;
    QPushButton* actionUrlButton = new QPushButton("Action URL", this);
        actionUrlButton->setIcon(QIcon(":/images/action.png"));
        actionUrlButton->setToolTip("Open Action Url");
       connect( actionUrlButton, SIGNAL( pressed()) , this, SLOT( actionUrlButtonPressed() ) );
    mainGrid->addWidget(actionUrlButton,r,4);
    if (ActionUrl.isEmpty()) actionUrlButton->setVisible(false);

    r=3;
    QPushButton* notesUrlButton = new QPushButton("Notes URL", this);
        notesUrlButton->setIcon(QIcon(":/images/notes.png"));
        notesUrlButton->setToolTip("Open Notes Url");
       connect( notesUrlButton, SIGNAL( pressed()) , this, SLOT( notesUrlButtonPressed() ) );
    mainGrid->addWidget(notesUrlButton,r,4);
    if (NotesUrl.isEmpty()) notesUrlButton->setVisible(false);

    r=4;
    QPushButton* logButton = new QPushButton("Open Log", this);
        logButton->setIcon(QIcon(":/images/log.png"));
        logButton->setToolTip("Open Log");
       connect( logButton, SIGNAL( pressed()) , this, SLOT( logButtonPressed() ) );
    mainGrid->addWidget(logButton,r,4);

    r=5;
    // we get the combobox from a command widget
    CommandWidget *commandWidget = new CommandWidget(this);
        connect(commandWidget,SIGNAL(sendLiveStatusData(QString,QString)),this,SLOT(slotSendLiveStatusData(QString, QString)));
    mainGrid->addWidget(commandWidget->getCommandComboBox(Hostname, "", State, HostDetails),r,4);

    //Get overallstate
    //worst_service_state (18) + state (4)
    int overallState=jsonArray.at(18).toDouble()+jsonArray.at(4).toDouble();
    qDebug() << "overallState" << overallState;
    r=1;
    pcDialog = new PCDialog(Hostname, overallState, this);
        pcDialog->setCenterNode(Hostname, overallState);
    connect( pcDialog, SIGNAL(doubleClickedHost(QString)) , this, SLOT( slotDoubleClickedHost(QString) ) );
      mainGrid->addWidget(pcDialog,r,5,9,3);

      QJsonArray parentJsonArray=jsonArray.at(15).toArray();
     //pcDialog->setGeneration();
     for (int i=0;i<parentJsonArray.size();i++)
     {
         if (!parentJsonArray.at(i).toString().trimmed().isEmpty())
         {
           pcDialog->addParent(parentJsonArray.at(i).toString());
         }
     }

     //Childs (16)
     QJsonArray childJsonArray=jsonArray.at(16).toArray();
    //pcDialog->setGeneration();
    for (int i=0;i<childJsonArray.size();i++)
    {
        if (!childJsonArray.at(i).toString().trimmed().isEmpty())
        {
          pcDialog->addChild(childJsonArray.at(i).toString());
        }
    }

      r=0;
      QGridLayout *topButtonGrid = new QGridLayout(this);

      QPushButton* getParentsButton = new QPushButton("Next Generation", this);
         connect( getParentsButton, SIGNAL( pressed()) , this, SLOT( requestParents() ) );
      //mainGrid->addWidget(getParentsButton,r,5);
         topButtonGrid->addWidget(getParentsButton,0,0);

      QPushButton* forceButton = new QPushButton("Force", this);
         connect( forceButton, SIGNAL( pressed()) , pcDialog, SLOT( force() ) );
      topButtonGrid->addWidget(forceButton,0,1);

      QPushButton* scaleButton = new QPushButton("Scale", this);
        connect( scaleButton, SIGNAL( pressed()) , pcDialog, SLOT( autoScale() ) );
      topButtonGrid->addWidget(scaleButton,0,2);

    topButtonGrid->addItem(new QSpacerItem ( 100, 10, QSizePolicy::Expanding, QSizePolicy::Fixed ),0,3);
      mainGrid->addLayout(topButtonGrid,r,5,1,3);

     //last spacer
       //mainGrid->addItem(new QSpacerItem ( 300, 0, QSizePolicy::Maximum, QSizePolicy::Fixed ),10,1,1,5);
       mainGrid->addItem(new QSpacerItem ( 500, 0, QSizePolicy::Expanding, QSizePolicy::Fixed ),10,5);
}
HostInfoWidget::~HostInfoWidget()
{
}

void HostInfoWidget::browserUrlButtonPressed()
{
    QString url=qApp->property("ClientHostOpenUrl").toString();
        url.replace("%HOST%",Hostname);
    QDesktopServices::openUrl(url);
}

void HostInfoWidget::actionUrlButtonPressed()
{
     QDesktopServices::openUrl(ActionUrl);
     qDebug() << "Action Url" << ActionUrl;
}

void HostInfoWidget::notesUrlButtonPressed()
{
     QDesktopServices::openUrl(NotesUrl);
     qDebug() << "Notes Url" <<  NotesUrl;
}

void HostInfoWidget::requestParents()
{
    LiveSocketRequest ls;
    ls.setTable("hosts");
    ls.addColumns(QStringList() << "host_name" << "state" << "worst_service_state" << "parents" << "childs");

    QStringList currentGenerationNodes=pcDialog->getCurrentGenerationStringList();

    // There are no more child or parents
    if (currentGenerationNodes.size() == 0)
    {
        return;
    }

    for (int i=0;i<currentGenerationNodes.size();i++)
    {
        ls.addFilter("host_name = " + currentGenerationNodes.at(i));
    }
    if (currentGenerationNodes.size()>1)
    {
        ls.addRawText(QString("Or: %1\n").arg(currentGenerationNodes.size()));
    }
    ls.setOutputFormat("json");
    emit requestLiveStatusData(uid,"json",ls.getLiveStatusRequest());
}

void HostInfoWidget::gotLiveStatusJson(QJsonArray jsonUpperArray)
{
    //We add new generation, so ++
    pcDialog->setGeneration();


    for (int k = 0; k < jsonUpperArray.size(); ++k)
    {
        QJsonArray jsonArray=jsonUpperArray.at(k).toArray();
    qDebug() << "gotLiveStatusJson" << jsonArray;
        QString host = jsonArray.at(0).toString();
        if (host.trimmed().isEmpty())
        {
            continue;
        }
        int state=jsonArray.at(1).toDouble() + jsonArray.at(2).toDouble();
        QStringList parents;
        for (int i=0;i<jsonArray.at(3).toArray().size();i++)
        {
            parents << jsonArray.at(3).toArray().at(i).toString();
        }
        parents.sort();

        QStringList childs;
        for (int i=0;i<jsonArray.at(4).toArray().size();i++)
        {
            childs << jsonArray.at(4).toArray().at(i).toString();
        }
        childs.sort();

        //Lets find out, if host is child or parent
        Node* node=pcDialog->nodeFromNodename(host);
        qDebug() << "Recieved for HostInfo" << host << parents << childs;

            if (node->getGeneration()>0)
            {
                //it is a parent request
               //hostname always exists, could be parent, could be child
                for (int i=0;i<parents.size();i++)
                {
                   // if (!parents.at(i).trimmed().isEmpty())
                        pcDialog->addNode(parents.at(i), host, 127, "parent");
                }
            }
            else
            {
                //its a cild request
                for (int i=0;i<childs.size();i++)
                {
                   if (!childs.at(i).trimmed().isEmpty())
                        pcDialog->addNode(childs.at(i),host,127, "child");
                }
            }
            node->setState(state);
    }
}

void HostInfoWidget::serviceItemDoubleClicked ( QTreeWidgetItem * item, int col)
{
    (void) col;
    emit doubleClickedService (Hostname, item->text(0));
}

void HostInfoWidget::keyPressEvent(QKeyEvent * event)
{
    //qDebug() << "key pressed" << event->key();
    if (event->key() == Qt::Key_Escape)
    {
        deleteLater();
    }
}
