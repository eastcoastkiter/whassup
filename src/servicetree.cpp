#include "servicetree.h"
using namespace std;

ServiceTree::ServiceTree(QWidget * parent): QTreeWidget( parent )
{
    sortOrder=0;
    sortIndicator=3;
    foundIncidents=false;
    setSelectionMode(QAbstractItemView::MultiSelection);
    setColumnCount(21);
    setContextMenuPolicy(Qt::CustomContextMenu);
    header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    QStringList headerLabelsService;
        headerLabelsService << "Hostname" << "Service" << "Status" << "Last Change" << "State Typ" << "Acknowledged" << "Notification_enabled";
        headerLabelsService << "Plugin Output" << "Downtimes" << "Downtime Depth" << "host_state" << "host_state_type" << "host_acknowledged";
        headerLabelsService << "host_downtimes" << "host_scheduled_downtime_depth"  << "contacts" << "host_address" << "host_notifications_enabled" << "active_checks_enabled" << "host_active_checks_enabled" << "epoch";
//write last_change epoch in 20
//write state to 2
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
        hideColumn ( 19 );
        hideColumn ( 20 );


        connect(this, SIGNAL(customContextMenuRequested ( const QPoint & )), this, SLOT(showServicePopup(const QPoint &)));
        connect(header(), SIGNAL(sortIndicatorChanged(int,Qt::SortOrder)), this, SLOT(sortIndicatorChanged(int,Qt::SortOrder)));
}

ServiceTree::~ServiceTree()
{}

void ServiceTree::showServicePopup(const QPoint &iPosition)
{
    emit changeTimer(false);
    //should be Select
    //lastRightClickedItem->setSelected(true);
    itemAt(iPosition)->setSelected(true);
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

        //host_state                                    host_state_type                             host_acknowledge (already)
        if ((lastRightClickedItem->text(10) != "0") && (lastRightClickedItem->text(11) != "0") && (lastRightClickedItem->text(12) == "0"))
        {
        QAction *hostAckRightAction   = new QAction("&Acknowledge", this);
            hostAckRightAction->setIcon(QPixmap(ack_xpm));
            hostAckRightAction->setObjectName("HostAcknowledge");
                connect(hostAckRightAction, SIGNAL(triggered(bool)), this, SLOT(menuRightSlot(bool)));
        hostMenu->addAction(hostAckRightAction);
        }

        QAction *hostDowntimeRightAction   = new QAction("&Downtime", this);
            hostDowntimeRightAction->setIcon(QPixmap(downtime_xpm));
            hostDowntimeRightAction->setObjectName("HostDowntime");
                connect(hostDowntimeRightAction, SIGNAL(triggered(bool)), this, SLOT(menuRightSlot(bool)));
        hostMenu->addAction(hostDowntimeRightAction);

        hostMenu->addSeparator();

        // 19 == host_active_checks_enabled
        if (lastRightClickedItem->text(19) == "0")
        {
        QAction *hostEnableActiveChecks   = new QAction("&Enable Active Checks", this);
            hostEnableActiveChecks->setIcon(QIcon(":/images/checks_enabled.png"));
            hostEnableActiveChecks->setObjectName("HostEnableActiveChecks");
                connect(hostEnableActiveChecks, SIGNAL(triggered(bool)), this, SLOT(menuRightSlot(bool)));
        hostMenu->addAction(hostEnableActiveChecks);
        }
        else
        {
            QAction *hostDisableActiveChecks   = new QAction("&Disable Active Checks", this);
                hostDisableActiveChecks->setIcon(QIcon(":/images/checks_disabled.png"));
                hostDisableActiveChecks->setObjectName("HostDisableActiveChecks");
                    connect(hostDisableActiveChecks, SIGNAL(triggered(bool)), this, SLOT(menuRightSlot(bool)));
            hostMenu->addAction(hostDisableActiveChecks);
        }

        // 17 == host_notifications_enabled
        if (lastRightClickedItem->text(17) == "0")
        {
        QAction *hostEnableNotifications   = new QAction("&Enable Notifications", this);
            hostEnableNotifications->setIcon(QIcon(":/images/notification_enabled.png"));
            hostEnableNotifications->setObjectName("HostEnableNotifications");
                connect(hostEnableNotifications, SIGNAL(triggered(bool)), this, SLOT(menuRightSlot(bool)));
        hostMenu->addAction(hostEnableNotifications);
        }
        else
        {
            QAction *hostDisableNotifications   = new QAction("&Disable Notifications", this);
                hostDisableNotifications->setIcon(QIcon(":/images/notification_disabled.png"));
                hostDisableNotifications->setObjectName("HostDisableNotifications");
                    connect(hostDisableNotifications, SIGNAL(triggered(bool)), this, SLOT(menuRightSlot(bool)));
            hostMenu->addAction(hostDisableNotifications);
        }

        hostMenu->addSeparator();

        QAction *hostSearchLogRightAction   = new QAction("&Log Search", this);
            hostSearchLogRightAction->setIcon(QIcon(":/images/log.png"));
            hostSearchLogRightAction->setObjectName("LogSearch");
                connect(hostSearchLogRightAction, SIGNAL(triggered(bool)), this, SLOT(menuRightSlot(bool)));
        hostMenu->addAction(hostSearchLogRightAction);

        QAction *hostDetailRightAction = new QAction("&Detail", this);
            hostDetailRightAction->setIcon(QIcon(":/images/info.png"));
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

        serviceMenu->addSeparator();

        // 18 == active_checks_enabled
        if (lastRightClickedItem->text(18) == "0")
        {
        QAction *serviceEnableActiveChecks   = new QAction("&Enable Active Checks", this);
            serviceEnableActiveChecks->setIcon(QIcon(":/images/checks_enabled.png"));
            serviceEnableActiveChecks->setObjectName("ServiceEnableActiveChecks");
                connect(serviceEnableActiveChecks, SIGNAL(triggered(bool)), this, SLOT(menuRightSlot(bool)));
        serviceMenu->addAction(serviceEnableActiveChecks);
        }
        else
        {
            QAction *serviceDisableActiveChecks   = new QAction("&Disable Active Checks", this);
                serviceDisableActiveChecks->setIcon(QIcon(":/images/checks_disabled.png"));
                serviceDisableActiveChecks->setObjectName("ServiceDisableActiveChecks");
                    connect(serviceDisableActiveChecks, SIGNAL(triggered(bool)), this, SLOT(menuRightSlot(bool)));
            serviceMenu->addAction(serviceDisableActiveChecks);
        }

        // 6 == service_notifications_enabled
        if (lastRightClickedItem->text(6) == "0")
        {
        QAction *serviceEnableNotifications   = new QAction("&Enable Notifications", this);
            serviceEnableNotifications->setIcon(QIcon(":/images/notification_enabled.png"));
            serviceEnableNotifications->setObjectName("ServiceEnableNotifications");
                connect(serviceEnableNotifications, SIGNAL(triggered(bool)), this, SLOT(menuRightSlot(bool)));
        serviceMenu->addAction(serviceEnableNotifications);
        }
        else
        {
            QAction *serviceDisableNotifications   = new QAction("&Disable Notifications", this);
                serviceDisableNotifications->setIcon(QIcon(":/images/notification_disabled.png"));
                serviceDisableNotifications->setObjectName("ServiceDisableNotifications");
                    connect(serviceDisableNotifications, SIGNAL(triggered(bool)), this, SLOT(menuRightSlot(bool)));
            serviceMenu->addAction(serviceDisableNotifications);
        }

        serviceMenu->addSeparator();

        QAction *serviceDetailRightAction = new QAction("&Detail", this);
        serviceDetailRightAction->setIcon(QIcon(":/images/info.png"));
            serviceDetailRightAction->setObjectName("ServiceDetail");
                connect(serviceDetailRightAction, SIGNAL(triggered(bool)), this, SLOT(menuRightSlot(bool)));
        serviceMenu->addAction(serviceDetailRightAction);

        menu.addMenu(hostMenu);
        menu.addMenu(serviceMenu);
    //    menu.addSeparator();

        menu.exec(mapToGlobal(iPosition));
      }
}


void ServiceTree::menuRightSlot(bool on)
{
    emit changeTimer(true);
    (void) on;
    qDebug()<< "lastRightClickedItem->text(0)" << lastRightClickedItem->text(0) << lastRightClickedItem->text(1);

    if (sender()->objectName() == "LogSearch")
    {
        emit signalLogSearch(lastRightClickedItem->text(0));
        return;
    }
    else if (sender()->objectName() == "HostDetail")
    {
        emit signalHostDetail(lastRightClickedItem->text(0));
        return;
    }
    else if (sender()->objectName() == "ServiceDetail")
    {
        emit signalServiceDetail(lastRightClickedItem->text(0),lastRightClickedItem->text(1));
        return;
    }
    else
    {
        CommandWidget* commandWidget = new CommandWidget(this);
            connect(commandWidget, SIGNAL(setStatusBar(QString, int)), this,SLOT(receivedStatusBar(QString, int)));
            connect(commandWidget, SIGNAL(sendLiveStatusData(QString, QString)), this,SLOT(slotSendLiveStatusData(QString, QString)));

        if (sender()->objectName() == "Acknowledge")
        {
            // why a new Qlist, this creates a shallow copy, which deeps if qrignal changes
            //so if timer changes orginal list, copy stays, great!
            commandWidget->acknowledgeService(QList <QTreeWidgetItem* > (selectedItems()));
            return;
        }
        else if (sender()->objectName() == "Reschedule")
        {
            commandWidget->scheduleServiceCheck(QList <QTreeWidgetItem* > (selectedItems()));
            return;
        }
        else if (sender()->objectName() == "HostAcknowledge")
        {
            commandWidget->acknowledgeHost(QList <QTreeWidgetItem* > (selectedItems()));
            return;
        }
        else if (sender()->objectName() == "Downtime")
        {
            commandWidget->scheduleServiceDowntime(QList <QTreeWidgetItem* > (selectedItems()));
            return;
        }
        else if (sender()->objectName() == "HostDowntime")
        {
            commandWidget->scheduleHostDowntime(QList <QTreeWidgetItem* > (selectedItems()));
            return;
        }
        else if (sender()->objectName() == "HostEnableActiveChecks")
        {
            commandWidget->enableHostCheck(QList <QTreeWidgetItem* > (selectedItems()));
            return;
        }
        else if (sender()->objectName() == "HostDisableActiveChecks")
        {
            commandWidget->disableHostCheck(QList <QTreeWidgetItem* > (selectedItems()));
            return;
        }
        else if (sender()->objectName() == "HostEnableNotifications")
        {
            commandWidget->enableHostNotification(QList <QTreeWidgetItem* > (selectedItems()));
            return;
        }
        else if (sender()->objectName() == "HostDisableNotifications")
        {
            commandWidget->disableHostNotification(QList <QTreeWidgetItem* > (selectedItems()));
            return;
        }
        else if (sender()->objectName() == "ServiceEnableActiveChecks")
        {
            commandWidget->enableServiceCheck(QList <QTreeWidgetItem* > (selectedItems()));
            return;
        }
        else if (sender()->objectName() == "ServiceDisableActiveChecks")
        {
            commandWidget->disableServiceCheck(QList <QTreeWidgetItem* > (selectedItems()));
            return;
        }
        else if (sender()->objectName() == "ServiceEnableNotifications")
        {
            commandWidget->enableServiceNotification(QList <QTreeWidgetItem* > (selectedItems()));
            return;
        }
        else if (sender()->objectName() == "ServiceDisableNotifications")
        {
            commandWidget->disableServiceNotification(QList <QTreeWidgetItem* > (selectedItems()));
            return;
        }

        else
        {
            return;
        }
    }
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
    hideColumn ( 19 );
    hideColumn ( 20 );


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

void ServiceTree::gotServiceDataJson(QJsonArray jsonUpperArray)
{
    QList<QTreeWidgetItem *> items;
    clear();
    update();
    int serviceBadStates=0;

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
    qDebug() << jsonUpperArray.at(0);
    for (int k = 0; k < jsonUpperArray.size()-1; ++k)
    {
        QJsonArray jsonArray=jsonUpperArray.at(k).toArray();
        QStringList itemValues;

        int c=0;
        QString jhost_name=jsonArray.at(c).toString();
            itemValues << jhost_name;
        c++;
        QString jdescription=jsonArray.at(c).toString();
            itemValues << jdescription;
        c++;
        int jstate=jsonArray.at(c).toDouble();
            itemValues << QString("%1").arg(jstate);
        c++;
        int jlast_state_change=jsonArray.at(c).toDouble();
            itemValues << QString("%1").arg(jlast_state_change);
        c++;
        int jstate_type=jsonArray.at(c).toDouble();
            itemValues << QString("%1").arg(jstate_type);
        c++;
        int jacknowledged=jsonArray.at(c).toDouble();
            itemValues << QString("%1").arg(jacknowledged);
        c++;
        int jnotifications_enabled=jsonArray.at(c).toDouble();
            itemValues << QString("%1").arg(jnotifications_enabled);
        c++;
        QString jplugin_output=jsonArray.at(c).toString();
            itemValues << jplugin_output;
        c++;
        int jdowntimes=jsonArray.at(c).toDouble();
            itemValues << QString("%1").arg(jdowntimes);
        c++;
        int jscheduled_downtime_depth=jsonArray.at(c).toDouble();
            itemValues << QString("%1").arg(jscheduled_downtime_depth);
        c++;
        int jhost_state=jsonArray.at(c).toDouble();
            itemValues << QString("%1").arg(jhost_state);
        c++;
        int jhost_state_type=jsonArray.at(c).toDouble();
            itemValues << QString("%1").arg(jhost_state_type);
        c++;
        int jhost_acknowledge=jsonArray.at(c).toDouble();
            itemValues << QString("%1").arg(jhost_acknowledge);
        c++;
        int jhost_downtimes=jsonArray.at(c).toDouble();
            itemValues << QString("%1").arg(jhost_downtimes);
        c++;
        int jhost_scheduled_downtime_depth=jsonArray.at(c).toDouble();
            itemValues << QString("%1").arg(jhost_scheduled_downtime_depth);
        c++;
        QString jcontacts=jsonArray.at(c).toString();
            itemValues << QString("%1").arg(jcontacts);
        c++;
        QString jhost_address=jsonArray.at(c).toString();
            itemValues << QString("%1").arg(jhost_address);

        c++;
        int jhost_notifications_enabled=jsonArray.at(c).toDouble();
            itemValues << QString("%1").arg(jhost_notifications_enabled);

        c++;
        int jactive_checks_enabled=jsonArray.at(c).toDouble();
            itemValues << QString("%1").arg(jactive_checks_enabled);

        c++;
        int jhost_active_checks_enabled=jsonArray.at(c).toDouble();
            itemValues << QString("%1").arg(jhost_active_checks_enabled);

            // check if new
            if ( (jstate != 0) && (!foundIncidents) && (!lastServices.isEmpty()) )
            {
                //qDebug() << "comparing";
                compareIncidents(jhost_name, jdescription);
            }
            // fill list to compare later
        currentServices << jhost_name + jdescription;


       // qDebug() << "hname" << jhost_name;
        QList<QTreeWidgetItem *> hostList;
        hostList=findItems(jhost_name,Qt::MatchExactly,0);

        ServiceTreeItem* Item = new ServiceTreeItem(itemValues,QTreeWidgetItem::Type);
        int timezone=qApp->property("ClientTimezone").toInt();
       // qDebug() << "timezone" << timezone;
        QString TimeFormat=qApp->property("ClientTimeFormat").toString();
        int epochSecs=(jlast_state_change);
                QDateTime dt = QDateTime::fromString("01-01-70 00:00:00", "dd-MM-yy HH:mm:ss").addSecs(epochSecs+(3600*timezone));

                //Item->setText(3,dt.toString("HH:mm dd-MM-yy"));
        Item->setText(3,dt.toString(TimeFormat));

        // set epoch for sorting
        Item->setText(20,QString("%1").arg(epochSecs));

        //state for sorting is col2

        if (hostList.size() > 0)
        {
            //host exists, append next service
            hostList.first()->addChild(Item);
        }
        else
        {
            items.append(Item);
        }
        insertTopLevelItems(0, items);


        // lets do the color for hosts
        if (jhost_state == 1)
        {
            if (jhost_acknowledge == 0)
              Item->setIcon(0, QIcon(QPixmap(critical_xpm)) );
            else
              Item->setIcon(0, QIcon(QPixmap(critical_ack_xpm)) );
        }
        else if (jhost_state == 0)
        {
                //Item->setIcon(0, QIcon(ok_xpm) );
        }
        else
        {
             Item->setIcon(0, QIcon(QPixmap(unknown_xpm)) );
             serviceBadStates++;
        }

        if (jstate_type == 0)
        {
            Item->setIcon(3, QIcon(QPixmap(soft_xpm)) );
        }

        if (jnotifications_enabled == 0)
        {
            Item->setIcon(1, QIcon(QPixmap(ndisabled_xpm)) );
        }


        if (jhost_notifications_enabled == 0)
        {
            Item->setIcon(0, QIcon(QPixmap(ndisabled_xpm)) );
        }

        if (jstate_type == 0)
        {
            Item->setIcon(3, QIcon(QPixmap(soft_xpm)) );
        }

        if (jhost_active_checks_enabled == 0)
        {
            Item->setIcon(0, QIcon(":/images/checks_disabled.png") );
        }

        if (jactive_checks_enabled == 0)
        {
            Item->setIcon(3, QIcon(":/images/checks_disabled.png") );
        }

        // lets do the color fro services
        if (jstate == 2)
        {
            if (jacknowledged == 0)
            {
              Item->setIcon(1, QIcon(QPixmap(critical_xpm)) );
              serviceBadStates++;
            }
            else
            {
              Item->setIcon(1, QIcon(QPixmap(critical_ack_xpm)) );
            }
        }
        else if (jstate == 1)
        {
            if (jacknowledged == 0)
            {
                 Item->setIcon(1, QIcon(QPixmap(warning_xpm)) );
                 serviceBadStates++;
            }
            else
            {
                 Item->setIcon(1, QIcon(QPixmap(warning_ack_xpm)) );
            }
        }
        else if (jstate == 0)
        {
                Item->setIcon(1, QIcon(QPixmap(ok_xpm)) );
        }
        else
        {
             Item->setIcon(1, QIcon(QPixmap(unknown_xpm)) );
             serviceBadStates++;
        }

        //notification
        if (jnotifications_enabled == 0)
        {
            Item->setIcon(3, QIcon(QPixmap(ndisabled_xpm)) );
        }

        //soft state
        if ( (jstate_type == 0) || (jhost_state_type == 0) )
        {
            Item->setIcon(3, QIcon(QPixmap(soft_xpm)) );
        }


        //downtime service
        if ( (jdowntimes != 0) || (jscheduled_downtime_depth != 0) )
        {
            Item->setIcon(1, QIcon(QPixmap(downtime_xpm)) );
        }

        //downtime host
        if ( (jhost_downtimes != 0) || (jhost_scheduled_downtime_depth != 0) )
        {
            Item->setIcon(0, QIcon(QPixmap(downtime_xpm)) );
        }

  }

        //ugly hack, sequentiell
        //sortByColumn(0,Qt::AscendingOrder);
        setSortingEnabled (true);
       // serviceTree->expandAll();

        emit signalSetTraySystemIcon(serviceBadStates);

        /* Remember the last errors */
        lastServices=currentServices;
        currentServices.clear();
        //set to false for next run
        foundIncidents=false;
}

void ServiceTree::compareIncidents(QString chost_name, QString cdescription)
{
/* lets do a trick here
 * to compare, if there are new faulty service, we create a Stringlist
 *but what do we do with hostname and description
 *ok just add them, we don't need them afterwards anymore
*/
    if (!qApp->property("ClientBlinkSystemTray").toBool())
        return;

    QString compareTmpString = chost_name + cdescription;
    if (!lastServices.contains(compareTmpString))
    {
        foundIncidents=true;
        qDebug() << "new Error" << chost_name << cdescription;
        emit foundIncident();
    }
}

void ServiceTree::sortIndicatorChanged(int col,Qt::SortOrder sOrder)
{
    sortOrder=sOrder;
    sortIndicator=col;
}

void ServiceTree::writeSettings(QSettings* settings)
{
    settings->beginGroup("ServiceTree");
        settings->setValue("SortOrder", sortOrder);
        settings->setValue("SortIndicator", sortIndicator);
    settings->endGroup();
    settings->sync();
}

void ServiceTree::loadSettings(QSettings* settings)
{
    settings->beginGroup("ServiceTree");
        sortOrder=settings->value("SortOrder").toInt();
        sortIndicator=settings->value("SortIndicator").toInt();
        header()->setSortIndicator(sortIndicator,(Qt::SortOrder) sortOrder);
    settings->endGroup();
}


