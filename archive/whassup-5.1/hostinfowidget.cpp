#include "hostinfowidget.h"
using namespace std;
/* json
HostInfoWidget::HostInfoWidget(QList<QStringList> listOfStringLists, QString TimeFormat,int TimeZone, QString ServiceURL,int uniqObjectID, QWidget * parent, Qt::WindowFlags f): QWidget( parent, f )
{
    uid=uniqObjectID;
    SURL=ServiceURL;

    Hostname=listOfStringLists.at(0).at(0);
    NotesUrl=listOfStringLists.at(0).at(11);
    ActionUrl=listOfStringLists.at(0).at(12);


     resize(800,640);
     setWindowTitle("HostInfo for " + Hostname);
     QGridLayout* mainGrid = new QGridLayout(this);

     //Lets get the info, request was:
     //GET hosts\nColumns:host_name alias display_name address state last_time_down comments_with_info services_with_info contacts downtimes_with_info last_hard_state_change notes_url_expanded action_url_expanded plugin_output contact_groups parents childs  host_groups worst_service_state perf_data\n
     //                     0       1       2           3       4           5           6                   7                   8       9                   10                      11                  12              13              14              15  16      17          18                  19
     //Filter: host_name = " + host +" \nLimit: 1\n";

    int k = 0;
    int r=0;
    mainGrid->addWidget(new QLabel("Hostname:",this),r,0);
    QLabel* L_Hostname = new QLabel(Hostname,this);
            L_Hostname->setTextInteractionFlags(Qt::TextSelectableByMouse);
    mainGrid->addWidget(L_Hostname,r,1,1,2);

    r=1;
    qDebug() <<r;
    mainGrid->addWidget(new QLabel("Address:",this),r,0);
    QLabel* L_Address = new QLabel(listOfStringLists.at(k).at(3),this);
            L_Address->setTextInteractionFlags(Qt::TextSelectableByMouse);
    mainGrid->addWidget(L_Address,r,1,1,2);

    r=2;
    qDebug() <<r;
    mainGrid->addWidget(new QLabel("State:",this),r,0);
    QLabel* stateLabel = new QLabel(this);
    if (listOfStringLists.at(k).at(4) == "0")
    {
        stateLabel->setPixmap(QPixmap(ok_xpm));
    }
    else if (listOfStringLists.at(k).at(4) == "1")
    {
        stateLabel->setPixmap(QPixmap(warning_xpm));

    }
    else if (listOfStringLists.at(k).at(4).toInt() >= 2)
    {
        stateLabel->setPixmap(QPixmap(critical_xpm));
    }
    mainGrid->addWidget(stateLabel,r,1);

    // Plugin OUtput

    QLabel* L_Output = new QLabel(listOfStringLists.at(k).at(13),this);
            L_Output->setTextInteractionFlags(Qt::TextSelectableByMouse);
    mainGrid->addWidget(L_Output,r,2);
    mainGrid->addItem(new QSpacerItem ( 10, 10, QSizePolicy::Expanding, QSizePolicy::Fixed ),r,3);


    //last_hard_state_change

//Item->setText(3,dt.toString("HH:mm dd-MM-yy"));
    r=3;
    //inputTimezone->value()
    QDateTime lcdt = QDateTime::fromString("01-01-70 00:00:00", "dd-MM-yy HH:mm:ss").addSecs(QString(listOfStringLists.at(k).at(10)).toInt()+(3600*TimeZone));
    mainGrid->addWidget(new QLabel("Last Change:",this),r,0);
    QLabel* L_LastChange = new QLabel(lcdt.toString(TimeFormat),this);
            L_LastChange->setTextInteractionFlags(Qt::TextSelectableByMouse);
    mainGrid->addWidget(L_LastChange,r,1,1,2);



    //contacts (8) & contact_groups (14)
    r=4;
    mainGrid->addWidget(new QLabel("Contacts:",this),r,0);
    QComboBox* contactBox = new QComboBox(this);
        contactBox->addItems(listOfStringLists.at(k).at(8).split(","));
    mainGrid->addWidget(contactBox,r,1,1,2);

    r=5;
    mainGrid->addWidget(new QLabel("Groups:",this),r,0);
    QComboBox* contactGroupBox = new QComboBox(this);
        contactGroupBox->addItems(listOfStringLists.at(k).at(14).split(","));
        mainGrid->addWidget(contactGroupBox,r,1,1,2);


    // Hostgroups (17)
    r=6;
    mainGrid->addWidget(new QLabel("Hostgroups:",this),r,0);
    QComboBox* hostGroupBox = new QComboBox(this);
        hostGroupBox->addItems(listOfStringLists.at(k).at(17).split(","));
        mainGrid->addWidget(hostGroupBox,r,1,1,2);

    r=7;
    mainGrid->addWidget(new QLabel(" ",this),r,0);

    r=8;
    mainGrid->addWidget(new QLabel("Services:",this),r,0);
    r=9;
    //services_with_state (7)
    QTreeWidget* serviceTree = new QTreeWidget(this);
        serviceTree->setSelectionMode(QAbstractItemView::SingleSelection);
        serviceTree->setColumnCount(2);
        serviceTree->setMaximumWidth(600);
        serviceTree->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
        QStringList headerLabelsService;
        headerLabelsService << "Service" << "Output";
        serviceTree->setHeaderLabels(headerLabelsService);
        serviceTree->setSortingEnabled (false);
     mainGrid->addWidget(serviceTree,r,0,1,5);



     //QStringList listServices = QString(listOfStringLists.at(k).at(7)).contains();


    QStringList ServiceList;
    QStringList StateList;
    QStringList OutputList;

     // lets get the services (ugly format)
     QString state_with_info="," + QString(listOfStringLists.at(k).at(7));
     QRegularExpression re("(,(\\w|\\d|/|:| )+\\|)");
     QRegularExpressionMatchIterator i = re.globalMatch(state_with_info);
     while (i.hasNext())
     {
         QRegularExpressionMatch match = i.next();

         // Lets create a String List with Service name
         ServiceList << match.captured(0).remove(",").remove("|");

         //Here comes the trick, just remove what we just found, we need it nomore, but don't removw the seperating pipe
         state_with_info.remove(match.captured(0).remove("|"));
         qDebug()<< "use match" << match.captured(0);
     }
     qDebug() << "state_with_info" << state_with_info;

     //Lets find the State
     QRegularExpression re2("\\|\\d+\\|\\d+\\|");
     QRegularExpressionMatchIterator i2 = re2.globalMatch(state_with_info);
     while (i2.hasNext())
     {
            QRegularExpressionMatch match = i2.next();
            qDebug()<< "use match 2" << match.captured(0);
            qDebug() << "state" << match.captured(0).split("|")[1];

            StateList << match.captured(0).split("|")[1];
            state_with_info.replace(match.captured(0),"|");
     }
     qDebug() << "state_with_info" << state_with_info;

     // and last the output (remove first pipe
     state_with_info.remove(0,1);
     OutputList=state_with_info.split("|");

     for (int i=0;i<ServiceList.size();i++)
     {
         qDebug() << "ServicesList" << i << ServiceList.at(i) << StateList.at(i) << OutputList.at(i);
         QStringList tmp;
         tmp << ServiceList.at(i) <<  OutputList.at(i);
            QTreeWidgetItem* Item = new QTreeWidgetItem(tmp,QTreeWidgetItem::Type);
            if (StateList.at(i) == "0")
            {
                Item->setIcon(0, QIcon(QPixmap(ok_xpm)) );
            }
            else if (StateList.at(i) == "1")
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
     QPushButton* browserUrlButton = new QPushButton("Open in Browser", this);
        connect( browserUrlButton, SIGNAL( pressed()) , this, SLOT( browserUrlButtonPressed() ) );
     mainGrid->addWidget(browserUrlButton,r,4);

     r=2;
     QPushButton* actionUrlButton = new QPushButton("Open Action URL", this);
        connect( actionUrlButton, SIGNAL( pressed()) , this, SLOT( actionUrlButtonPressed() ) );
     mainGrid->addWidget(actionUrlButton,r,4);

     r=3;
     QPushButton* notesUrlButton = new QPushButton("Open Notes URL", this);
        connect( notesUrlButton, SIGNAL( pressed()) , this, SLOT( notesUrlButtonPressed() ) );
     mainGrid->addWidget(notesUrlButton,r,4);

     r=4;
     QPushButton* logButton = new QPushButton("Open Log", this);
        connect( logButton, SIGNAL( pressed()) , this, SLOT( logButtonPressed() ) );
     mainGrid->addWidget(logButton,r,4);


     //Get overallstate
     //worst_service_state (18) + state (4)
     int overallState=listOfStringLists.at(k).at(18).toInt()+listOfStringLists.at(k).at(4).toInt();
     qDebug() << "overallState" << overallState;
     r=1;
     pcDialog = new PCDialog(Hostname, overallState, this);
     connect( pcDialog, SIGNAL(doubleClickedHost(QString)) , this, SLOT( slotDoubleClickedHost(QString) ) );
       mainGrid->addWidget(pcDialog,r,5,9,3);

     //Parents (15)
      parentList=listOfStringLists.at(k).at(15).split(",");
      parentList.sort();
      //pcDialog->setGeneration();
      for (int i=0;i<parentList.size();i++)
      {
          if (!parentList.at(i).trimmed().isEmpty())
          {
            pcDialog->addParent(parentList.at(i));
            //pcDialog->addNode(parentList.at(i),Hostname,127,"parent");
          }
      }

      //Childs (16)
       QStringList childList=listOfStringLists.at(k).at(16).split(",");
       childList.sort();
       for (int i=0;i<childList.size();i++)
       {

           if (!childList.at(i).trimmed().isEmpty())
           {
                pcDialog->addChild(childList.at(i));
                //pcDialog->addNode(childList.at(i),Hostname,127,"child");
           }
       }

       r=0;
       QPushButton* getParentsButton = new QPushButton("Get Parents", this);
          connect( getParentsButton, SIGNAL( pressed()) , this, SLOT( requestParents() ) );
       mainGrid->addWidget(getParentsButton,r,5);

       QPushButton* forceButton = new QPushButton("Force", this);
          connect( forceButton, SIGNAL( pressed()) , pcDialog, SLOT( force() ) );
       mainGrid->addWidget(forceButton,r,6);

       mainGrid->addItem(new QSpacerItem ( 10, 10, QSizePolicy::Expanding, QSizePolicy::Fixed ),r,7);
      //last spacer
        //mainGrid->addItem(new QSpacerItem ( 300, 0, QSizePolicy::Maximum, QSizePolicy::Fixed ),10,1,1,5);
        mainGrid->addItem(new QSpacerItem ( 500, 0, QSizePolicy::Expanding, QSizePolicy::Fixed ),10,5);


}
*/
HostInfoWidget::HostInfoWidget(QJsonArray jsonArray, QString TimeFormat,int TimeZone, QString ServiceURL,int uniqObjectID, QWidget * parent, Qt::WindowFlags f): QWidget( parent, f )
{
    uid=uniqObjectID;
    SURL=ServiceURL;

    Hostname=jsonArray.at(0).toString();
    NotesUrl=jsonArray.at(11).toString();
    ActionUrl=jsonArray.at(12).toString();

    qDebug()<<"hsotname" << Hostname<<NotesUrl<<ActionUrl;

    resize(800,640);
    setWindowTitle("HostInfo for " + Hostname);
    QGridLayout* mainGrid = new QGridLayout(this);

    //Lets get the info, request was:
    //GET hosts\nColumns:host_name alias display_name address state last_time_down comments_with_info services_with_info contacts downtimes_with_info last_hard_state_change notes_url_expanded action_url_expanded plugin_output contact_groups parents childs  host_groups worst_service_state perf_data\n
    //                     0       1       2           3       4           5           6                   7                   8       9                   10                      11                  12              13              14              15  16      17          18                  19
    //Filter: host_name = " + host +" \nLimit: 1\n";

   int k = 0;
   int r=0;
   mainGrid->addWidget(new QLabel("Hostname:",this),r,0);
   QLabel* L_Hostname = new QLabel(Hostname,this);
           L_Hostname->setTextInteractionFlags(Qt::TextSelectableByMouse);
   mainGrid->addWidget(L_Hostname,r,1,1,2);

   r=1;
   qDebug() <<r;
   mainGrid->addWidget(new QLabel("Address:",this),r,0);
   QLabel* L_Address = new QLabel(jsonArray.at(3).toString(),this);
           L_Address->setTextInteractionFlags(Qt::TextSelectableByMouse);
   mainGrid->addWidget(L_Address,r,1,1,2);

   r=2;
   qDebug() <<r;
   mainGrid->addWidget(new QLabel("State:",this),r,0);
   QLabel* stateLabel = new QLabel(this);
   if (jsonArray.at(4).toDouble() == 0)
   {
       stateLabel->setPixmap(QPixmap(ok_xpm));
   }
   else if (jsonArray.at(4).toDouble() == 1)
   {
       stateLabel->setPixmap(QPixmap(warning_xpm));

   }
   else if (jsonArray.at(4).toDouble() >= 2)
   {
       stateLabel->setPixmap(QPixmap(critical_xpm));
   }
   mainGrid->addWidget(stateLabel,r,1);

   // Plugin OUtput

   QLabel* L_Output = new QLabel(jsonArray.at(13).toString(),this);
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
    QPushButton* browserUrlButton = new QPushButton("Open in Browser", this);
       connect( browserUrlButton, SIGNAL( pressed()) , this, SLOT( browserUrlButtonPressed() ) );
    mainGrid->addWidget(browserUrlButton,r,4);

    r=2;
    QPushButton* actionUrlButton = new QPushButton("Open Action URL", this);
       connect( actionUrlButton, SIGNAL( pressed()) , this, SLOT( actionUrlButtonPressed() ) );
    mainGrid->addWidget(actionUrlButton,r,4);

    r=3;
    QPushButton* notesUrlButton = new QPushButton("Open Notes URL", this);
       connect( notesUrlButton, SIGNAL( pressed()) , this, SLOT( notesUrlButtonPressed() ) );
    mainGrid->addWidget(notesUrlButton,r,4);

    r=4;
    QPushButton* logButton = new QPushButton("Open Log", this);
       connect( logButton, SIGNAL( pressed()) , this, SLOT( logButtonPressed() ) );
    mainGrid->addWidget(logButton,r,4);


    //Get overallstate
    //worst_service_state (18) + state (4)
    int overallState=jsonArray.at(18).toDouble()+jsonArray.at(4).toDouble();
    qDebug() << "overallState" << overallState;
    r=1;
    pcDialog = new PCDialog(Hostname, overallState, this);
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
      QPushButton* getParentsButton = new QPushButton("Get Parents", this);
         connect( getParentsButton, SIGNAL( pressed()) , this, SLOT( requestParents() ) );
      mainGrid->addWidget(getParentsButton,r,5);

      QPushButton* forceButton = new QPushButton("Force", this);
         connect( forceButton, SIGNAL( pressed()) , pcDialog, SLOT( force() ) );
      mainGrid->addWidget(forceButton,r,6);

      mainGrid->addItem(new QSpacerItem ( 10, 10, QSizePolicy::Expanding, QSizePolicy::Fixed ),r,7);
     //last spacer
       //mainGrid->addItem(new QSpacerItem ( 300, 0, QSizePolicy::Maximum, QSizePolicy::Fixed ),10,1,1,5);
       mainGrid->addItem(new QSpacerItem ( 500, 0, QSizePolicy::Expanding, QSizePolicy::Fixed ),10,5);



}
HostInfoWidget::~HostInfoWidget()
{
}

void HostInfoWidget::browserUrlButtonPressed()
{
     QDesktopServices::openUrl(SURL+Hostname);
     qDebug() << "Browser Url" << QString (SURL+Hostname);
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
    //QStringList currentParents=pcDialog->getParentStringList();
    QStringList currentGenerationNodes=pcDialog->getCurrentGenerationStringList();
    QString filters;

    // There are no more child or parents
    if (currentGenerationNodes.size() == 0)
    {
        return;
    }

    for (int i=0;i<currentGenerationNodes.size();i++)
    {
        filters=filters + "Filter: host_name = " +currentGenerationNodes.at(i) + "\n";
    }
    if (currentGenerationNodes.size()>1)
    {
        filters=filters + QString("Or: %1\n").arg(currentGenerationNodes.size());
    }
    //qDebug() << "Filters for Childrequest" << filters;
    emit requestLiveStatusData(uid,"GET hosts\nColumns: host_name state worst_service_state parents childs\n" + filters + "\n");
}

void HostInfoWidget::gotLiveStatusData(QList<QStringList> listOfStringLists)
{

    //We add new generation, so ++
    pcDialog->setGeneration();

    for (int k = 0; k < listOfStringLists.size()-1; ++k)
    {
        QString host = listOfStringLists.at(k).at(0);
        if (host.trimmed().isEmpty())
        {
            continue;
        }
        int state=QString(listOfStringLists.at(k).at(1)).toInt() + QString(listOfStringLists.at(k).at(2)).toInt();
        QStringList parents = listOfStringLists.at(k).at(3).split(",");
        parents.sort();
        QStringList childs = listOfStringLists.at(k).at(4).split(",");
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
