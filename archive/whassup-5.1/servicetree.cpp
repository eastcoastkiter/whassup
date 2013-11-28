#include "servicetree.h"
using namespace std;

ServiceTree::ServiceTree(QWidget * parent): QTreeWidget( parent )
{
    setSelectionMode(QAbstractItemView::MultiSelection);
    setColumnCount(15);
    setContextMenuPolicy(Qt::CustomContextMenu);
    header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    QStringList headerLabelsService;
        headerLabelsService << "Hostname" << "Services" << "Status" << "Last Change" << "State Typ" << "Acknowledged" << "Notification_enabled";
        headerLabelsService << "Plugin Output" << "Downtimes" << "Downtime Depth" << "host_state" << "host_state_type" << "host_acknowledged";
        headerLabelsService << "host_downtimes" << "host_scheduled_downtime_depth"  << "contacts";

        setHeaderLabels(headerLabelsService);
    setSortingEnabled (false);

        hideColumn ( 2 );
        hideColumn ( 4 );
        hideColumn ( 5 );
        hideColumn ( 6 );
        hideColumn ( 8 );
        hideColumn ( 9 );
        hideColumn ( 10 );
        hideColumn ( 11 );
        hideColumn ( 12 );
        hideColumn ( 13 );
        hideColumn ( 14 );
        hideColumn ( 15 );

        connect(this, SIGNAL(customContextMenuRequested ( const QPoint & )), this, SLOT(showServicePopup(const QPoint &)));
}

ServiceTree::~ServiceTree()
{
}

void ServiceTree::showServicePopup(const QPoint &iPosition)
{
    selectedServices.clear();
    for (int i = 0; i < selectedItems().size(); ++i)
    {
        QTreeWidgetItem *item=selectedItems().at(i);
        QStringList hostAndService;
        hostAndService << item->text(0) << item->text(1);
        qDebug() << "Selected Items  " << hostAndService;
        selectedServices << hostAndService;
    }



    lastRightClickedItem = 0 ;
      lastRightClickedItem = itemAt(iPosition) ;
      if ( 0 == lastRightClickedItem )
      {
        qDebug() << "No item selected" ;
      }
      else
      {
        qDebug() << "Item clicked" + lastRightClickedItem->text(0);
        QMenu menu(this);
        QMenu* hostMenu = new QMenu(this);
        hostMenu->setTitle("&Host");

        QAction *hostSearchLogRightAction   = new QAction("&Log Search", this);
            hostSearchLogRightAction->setIcon(QPixmap(search_xpm));
            hostSearchLogRightAction->setObjectName("LogSearch");
                connect(hostSearchLogRightAction, SIGNAL(triggered(bool)), this, SLOT(menuRightSlot(bool)));
        hostMenu->addAction(hostSearchLogRightAction);

        QAction *hostDetailRightAction = new QAction("&Detail", this);
            hostDetailRightAction->setIcon(QPixmap(info_xpm));
            hostDetailRightAction->setObjectName("HostDetail");
                connect(hostDetailRightAction, SIGNAL(triggered(bool)), this, SLOT(menuRightSlot(bool)));
        hostMenu->addAction(hostDetailRightAction);

        QMenu* serviceMenu = new QMenu(this);
        serviceMenu->setTitle("&Service");

        QAction *reschedRightAction   = new QAction("&Reschedule", this);
            reschedRightAction->setIcon(QPixmap(reschedule_xpm));
            reschedRightAction->setObjectName("Reschedule");
                connect(reschedRightAction, SIGNAL(triggered(bool)), this, SLOT(menuRightSlot(bool)));
        serviceMenu->addAction(reschedRightAction);

         // not soft and not ack and not ok
        if ((lastRightClickedItem->text(4) != "0") && (lastRightClickedItem->text(5) == "0") && (lastRightClickedItem->text(2) != "0"))
        {
        QAction *ackRightAction   = new QAction("&Acknowledge", this);
            ackRightAction->setIcon(QPixmap(ack_xpm));
            ackRightAction->setObjectName("Acknowledge");
                connect(ackRightAction, SIGNAL(triggered(bool)), this, SLOT(menuRightSlot(bool)));
        serviceMenu->addAction(ackRightAction);
        }

        QAction *downtimeRightAction   = new QAction("&Downtime", this);
            downtimeRightAction->setIcon(QPixmap(downtime_xpm));
            downtimeRightAction->setObjectName("Downtime");
                connect(downtimeRightAction, SIGNAL(triggered(bool)), this, SLOT(menuRightSlot(bool)));
        serviceMenu->addAction(downtimeRightAction);

        QMenu* contactMenu = new QMenu(this);
            contactMenu->setTitle("Contacts");
        QStringList list = lastRightClickedItem->text(15).split(',');
        for (int i = 0; i < list.size(); ++i)
         {
             QAction* cAction = new QAction(list.at(i), this);
                 cAction->setEnabled(false);
             contactMenu->addAction(cAction);
         }
        menu.addMenu(hostMenu);
        menu.addMenu(serviceMenu);
    //    menu.addSeparator();
    // we dont need that anymore    menu.addMenu(contactMenu);

        menu.exec(mapToGlobal(iPosition));
      }
}


void ServiceTree::menuRightSlot(bool on)
{
    qDebug()<< "lastRightClickedItem->text(0)" << lastRightClickedItem->text(0);
    if (sender()->objectName() == "Acknowledge")
    {
        lastCmdTyp=CMD_ACKNOWLEDGE_SVC_PROBLEM;
    }
    else if (sender()->objectName() == "Reschedule")
    {
        lastCmdTyp=CMD_SCHEDULE_SVC_CHECK;
    }
    else if (sender()->objectName() == "Downtime")
    {
        lastCmdTyp=CMD_SCHEDULE_SVC_DOWNTIME;
    }
    else if (sender()->objectName() == "LogSearch")
    {
        emit signalLogSearch(lastRightClickedItem->text(0));
        return;
    }
    else if (sender()->objectName() == "HostDetail")
    {
        emit signalHostDetail(lastRightClickedItem->text(0));
        return;
    }
    else
    {
        return;
    }

    qDebug() << nagioshostname;
    QDateTime dt = QDateTime::currentDateTime ();

    menuWidget = new QWidget((QWidget*)parent());
        menuWidget->resize(250,200);
        menuWidget->setWindowTitle(sender()->objectName());
        menuWidget->setWindowFlags(Qt::Dialog);

    QGridLayout* ackGrid = new QGridLayout(menuWidget);
   //     ackGrid->addWidget(new QLabel("Hostname:", menuWidget),0,0);
   //     ackHostname = new QLineEdit(lastRightClickedItem->text(0),menuWidget);
     //           ackGrid->addWidget(ackHostname,0,1);

 //       ackGrid->addWidget(new QLabel("Service:", menuWidget),1,0);
   //     ackService = new QLineEdit(lastRightClickedItem->text(1),menuWidget);
    //R            ackGrid->addWidget(ackService,1,1);

        if ( (lastCmdTyp==CMD_SCHEDULE_SVC_CHECK) || (lastCmdTyp==CMD_SCHEDULE_SVC_DOWNTIME) || (lastCmdTyp==CMD_SCHEDULE_HOST_SVC_DOWNTIME) )
                        {
            ackGrid->addWidget(new QLabel("Start:", menuWidget),2,0);
            ackStart= new QDateTimeEdit(menuWidget);
                ackStart->setDateTime(dt);
                ackStart->setDisplayFormat("dd.MM.yy hh:mm");
                ackStart->setSelectedSection(QDateTimeEdit::MinuteSection);
            ackGrid->addWidget(ackStart,2,1);
        }

        if (lastCmdTyp==CMD_SCHEDULE_SVC_DOWNTIME)
        {
            ackGrid->addWidget(new QLabel("All Services:", menuWidget),3,0);
            ackAllServices = new QCheckBox(menuWidget);
            ackGrid->addWidget(ackAllServices,3,1);

            ackGrid->addWidget(new QLabel("Duration:", menuWidget),4,0);

            QGridLayout* durationGrid = new QGridLayout(menuWidget);
            ackGrid->addLayout(durationGrid,4,1);

            durationGrid->addWidget(new QLabel("(h/m)", menuWidget),0,0);

            ackHourDuration = new QSpinBox();
                ackHourDuration->setValue(0);
                ackHourDuration->setRange(0,100);
            durationGrid->addWidget(ackHourDuration,0,1);

            ackMinuteDuration = new QSpinBox();
                ackMinuteDuration->setValue(15);
                ackMinuteDuration->setRange(0,59);
            durationGrid->addWidget(ackMinuteDuration,0,2);

        }

        if ( (lastCmdTyp==CMD_ACKNOWLEDGE_SVC_PROBLEM) || (lastCmdTyp==CMD_SCHEDULE_SVC_DOWNTIME) )
        {
            ackGrid->addWidget(new QLabel("Comment:", menuWidget),5,0);
                ackComment = new QLineEdit("At: " + dt.toString("dd.MM.yy hh:mm"),menuWidget);
            ackGrid->addWidget(ackComment,5,1);
        }

        ackGrid->addWidget(new QLabel("Author:", menuWidget),6,0);
        ackAuthor = new QLineEdit(getenv("USERNAME"),menuWidget);
        if (!storUsername.isEmpty())
            ackAuthor->setText(storUsername);
                ackGrid->addWidget(ackAuthor,6,1);

        ackGrid->addWidget(new QLabel("Password:", menuWidget),7,0);
        ackPassword = new QLineEdit("",menuWidget);
        connect( ackPassword, SIGNAL( returnPressed()) , this, SLOT( ackSubmitSlot() ) );
            ackPassword->setEchoMode(QLineEdit::Password);
            if (!storPassword.isEmpty())
                ackPassword->setText(storPassword);
        ackGrid->addWidget(ackPassword,7,1);


        QPushButton* ackSubmitButton = new QPushButton("Submit", menuWidget);
           ackGrid->addWidget(ackSubmitButton,9,1);
           connect( ackSubmitButton, SIGNAL( pressed()) , this, SLOT( ackSubmitSlot() ) );

        ackGrid->addItem(new QSpacerItem ( 10, 10, QSizePolicy::Expanding, QSizePolicy::Expanding ),10,10);
    menuWidget->show();
}



void ServiceTree::ackSubmitSlot()
{
    QDateTime dt = QDateTime::currentDateTime ();
    if (lastCmdTyp==CMD_SCHEDULE_SVC_DOWNTIME)
    {
        if (ackAllServices->isChecked())
        {
            lastCmdTyp=CMD_SCHEDULE_HOST_SVC_DOWNTIME;
        }
    }
    menuWidget->close();

    for (int i = 0; i < selectedServices.size(); ++i)
    {
        QString selectedHost=(selectedServices.at(i))[0];
        QString selectedService=(selectedServices.at(i))[1];

    //Get Data
    QUrl nagiosUrl;
    QUrlQuery nagiosUrlQuery;

        nagiosUrl.setUrl("http://" + nagioshostname + "/nagios/cgi-bin/cmd.cgi");

        if (!ackAuthor->text().isEmpty())
            nagiosUrl.setUserName(ackAuthor->text().toLatin1());
        if (!ackPassword->text().isEmpty())
            nagiosUrl.setPassword(ackPassword->text().toLatin1());

        QByteArray postData;

            // Host
            nagiosUrlQuery.addQueryItem("host",QUrl::toPercentEncoding(selectedHost));
            postData.append("host=" + QUrl::toPercentEncoding(selectedHost) + "&");

            //Service
            nagiosUrlQuery.addQueryItem("service",QUrl::toPercentEncoding(selectedService));
            postData.append("service=" + QUrl::toPercentEncoding(selectedService) + "&");

            // cmd mode 2 = commit
            nagiosUrlQuery.addQueryItem("cmd_mode","2");
            postData.append("cmd_mod=2&");

            //cmd_typ
            nagiosUrlQuery.addQueryItem("cmd_typ",lastCmdTyp);
            postData.append("cmd_typ=" + lastCmdTyp + "&");

            //Author (Reschedule?)
            nagiosUrlQuery.addQueryItem("com_author",ackAuthor->text().toLatin1());
            postData.append("cmd_author=" + ackAuthor->text().toLatin1() + "&");


            // force check for resched
            if (lastCmdTyp==CMD_SCHEDULE_SVC_CHECK)
            {
                nagiosUrlQuery.addQueryItem("force_check","TRUE");
                postData.append("force_check=TRUE&");
            }

            // start_time
            if ( (lastCmdTyp==CMD_SCHEDULE_SVC_CHECK) || (lastCmdTyp==CMD_SCHEDULE_SVC_DOWNTIME) || (lastCmdTyp==CMD_SCHEDULE_HOST_SVC_DOWNTIME) )
            {
                nagiosUrlQuery.addQueryItem("start_time", QUrl::toPercentEncoding(ackStart->dateTime().toString("MM-dd-yyyy hh:mm:ss")));
                postData.append("start_time=" + ackStart->dateTime().toString("MM-dd-yyyy hh:mm:ss") + "&");
            }

            //downtime infos
            if ((lastCmdTyp==CMD_SCHEDULE_SVC_DOWNTIME) || (lastCmdTyp==CMD_SCHEDULE_HOST_SVC_DOWNTIME) )
            {
                nagiosUrlQuery.addQueryItem("trigger","0");
                postData.append("trigger=0&");

                nagiosUrlQuery.addQueryItem("fixed","1");
                postData.append("fixed=1&");

                nagiosUrlQuery.addQueryItem("hours",QString("%1").arg(ackHourDuration->value()));
                postData.append("hours=" + QString("%1").arg(ackHourDuration->value()) + "&");

                nagiosUrlQuery.addQueryItem("minutes",QString("%1").arg(ackMinuteDuration->value()));
                postData.append("minutes=" + QString("%1").arg(ackMinuteDuration->value()) + "&");

                nagiosUrlQuery.addQueryItem("end_time",ackStart->dateTime().addSecs((ackHourDuration->value()*3600)+(ackMinuteDuration->value()*60)).toString("MM-dd-yyyy hh:mm:ss"));
                postData.append("end_time=" + ackStart->dateTime().addSecs((ackHourDuration->value()*3600)+(ackMinuteDuration->value()*60)).toString("MM-dd-yyyy hh:mm:ss") +"&");
            }

           //Comment
            if ( (lastCmdTyp==CMD_ACKNOWLEDGE_SVC_PROBLEM) || (lastCmdTyp==CMD_SCHEDULE_SVC_DOWNTIME) || (lastCmdTyp==CMD_SCHEDULE_HOST_SVC_DOWNTIME) )
            {
                nagiosUrlQuery.addQueryItem("com_data",QUrl::toPercentEncoding(ackComment->text()));
                postData.append("com_data=" + ackComment->text().toLatin1());
            }


    qDebug() << "URL: "<< nagiosUrl.toString();

        //Data
        QNetworkRequest header;
        nagiosUrl.setQuery(nagiosUrlQuery);
        header.setUrl(nagiosUrl);
        header.setRawHeader( "User-Agent", "User Agent");
        header.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

        sendNagiosHttp(header, postData);
    }
}

void ServiceTree::sendNagiosHttp(QNetworkRequest header, QByteArray postData)
{
qDebug() << "PostData: "<< postData;

QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    reply=manager->post(header,postData);
        connect(reply, SIGNAL(readyRead()), this, SLOT(slotReadyReadHttp()));
}

void ServiceTree::slotReadyReadHttp()
{

    int start, stop;
    //<title>401 Authorization Required</title>

    QString all = reply->readAll();
        qDebug() << all;
    if (all.isEmpty())
        return;

    //Nagios Status
    start=all.indexOf("<DIV CLASS='infoMessage'>");
    if (start>0)
    {
        stop=all.indexOf("<BR>",start);
        if ( stop > start)
        {
            qDebug() << "mid" << all.mid(start+25,stop-(start+25));
            emit setStatusBar(QString(all.mid(start+25,stop-(start+25))).toLatin1());
        }
    }

    start=all.indexOf("<DIV CLASS='errorMessage'>");
    if (start>0)
    {
        stop=all.indexOf("</DIV>",start);
        if ( stop > start)
        {
            qDebug() << "mid" << all.mid(start+26,stop-(start+26));
            emit setStatusBar(QString(all.mid(start+26,stop-(start+26))).toLatin1());
        }
    }

    //Nagios Status
    start=all.indexOf("401 Auth");
    if (start>0)
    {
            emit setStatusBar("Command Error: 401 Authorization Required");
    }


    //if (!ackAuthor->text().isEmpty())

       storUsername=ackAuthor->text();
     storPassword=ackPassword->text();
    //delete menuWidget;
}

void ServiceTree::setHiddenColumns(int columnInt)
{
    qDebug()<<columnInt;
    /*
    headerLabelsService << "Hostname" << "Services" << "Status" << "Last Change" << "State Type" << "Acknowledged" << "Notification_enabled";
    headerLabelsService << "Plugin Output" << "Downtimes" << "Downtime Depth" << "host_state" << "host_state_type" << "host_acknowledged";
    headerLabelsService << "host_downtimes" << "host_scheduled_downtime_depth"  << "contacts" << "host_groups" << "last_check;

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
 */

    hideColumn ( 2 );
    hideColumn ( 5 );
    hideColumn ( 6 );
    hideColumn ( 12 );
    hideColumn ( 14 );
    hideColumn ( 15 );
    hideColumn ( 16 );
    hideColumn ( 18 );

    //Last Change
    if (columnInt & 1)
        showColumn(3);
    else
        hideColumn(3);

    //State_Type
    if (columnInt & 2)
        showColumn(4);
    else
        hideColumn(4);

    //Downtimes
    if (columnInt & 4)
        showColumn(8);
    else
        hideColumn(8);

    //Downtime_Depths
    if (columnInt & 8)
        showColumn(9);
    else
        hideColumn(9);

    //host_state
    if (columnInt & 16)
        showColumn(10);
    else
        hideColumn(10);

    //host_state_type
    if (columnInt & 32)
        showColumn(11);
    else
        hideColumn(11);
    //host_downtimes
    if (columnInt & 64)
        showColumn(13);
    else
        hideColumn(13);

    //last_check
    if (columnInt & 128)
        showColumn(17);
    else
        hideColumn(17);

}

