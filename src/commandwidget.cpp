#include "commandwidget.h"

CommandWidget::CommandWidget(QWidget * parent) : QWidget(parent)
{
    confirmServiceTree = new QTreeWidget(0);
    ackComment = new QLineEdit(0);
    authUser = new QLineEdit(0);
    authPassword = new QLineEdit(0);
    ackStart= new QDateTimeEdit(0);
    ackMinuteDuration = new QSpinBox(0);
    ackHourDuration = new QSpinBox(0);
    ackAllServices = new QCheckBox(0);
    comboBoxHost="";
    comboBoxDescription="";
}

CommandWidget::~CommandWidget()
{
    qDebug() << "deleting CommandWidget";
    deleteLater();
}

bool CommandWidget::showAuthCommandWidget()
{
    authDialog = new QDialog(this);
        authDialog->setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);
        authDialog->setWindowTitle("Server Authentication");

    QGridLayout *authgrid =new QGridLayout(authDialog);

    authgrid->addWidget(authWidget(),0,0,1,3);
    authgrid->addWidget(buttonsWidget(),1,1,1,2);
    authPassword->setFocus();
    if (authDialog->exec() == QDialog::Accepted)
    {
        qDebug() << "authDialog Accepted" << authUser->text();
        qApp->setProperty("ServerAuthUser",authUser->text());
        qApp->setProperty("ServerAuthPassword",authPassword->text());
        deleteLater();
        return true;
    }
    else
    {
        qDebug() << "authDialog Not accepted";
        deleteLater();
        return false;
    }
    authPassword->setFocus();
    authDialog->show();
    authDialog->raise();
    qApp->setActiveWindow(authDialog);
}

void CommandWidget::scheduleServiceDowntime(QList<QTreeWidgetItem*> selectedServices)
{
    qDebug () << "scheduleServiceDowntime(QList<QTreeWidgetItem*> selectedServices)" << selectedServices.size();

    authDialog = new QDialog(this);
        authDialog->setWindowTitle("Schedule Service Downtime");

    QGridLayout *authgrid =new QGridLayout(authDialog);


    authgrid->addWidget(confirmTree(selectedServices),0,0,1,3);


    authgrid->addWidget(allServiceWidget(),2,0,1,3);
    authgrid->addWidget(downtimeWidget(),3,0,1,3);
    authgrid->addWidget(commentWidget(),4,0,1,3);
    authgrid->addWidget(authWidget(),5,0,1,3);
    authgrid->addWidget(buttonsWidget(),6,1,1,3);

    //authgrid->addItem(new QSpacerItem ( 85, 0, QSizePolicy::Fixed, QSizePolicy::Fixed ),7,0);

    if (authDialog->exec() == QDialog::Accepted)
    {
        qDebug() << "authDialog Accepted" << authUser->text();
        qApp->setProperty("ServerAuthUser",authUser->text());
        qApp->setProperty("ServerAuthPassword",authPassword->text());
        QString cmd;
        if (ackAllServices->isChecked())
            cmd=CMD_SCHEDULE_HOST_SVC_DOWNTIME;
        else
            cmd=CMD_SCHEDULE_SVC_DOWNTIME;

        if (qApp->property("ServerCommandType") == 0)
        {
            sendToCgi(cmd,selectedServices);
        }
        else
        {
            sendToLiveSocket(cmd,selectedServices);
        }
        //deleteLater();
    }
    else
    {
        qDebug() << "authDialog Not accepted";
        //deleteLater();
    }
    show();
}

void CommandWidget::scheduleHostDowntime(QList<QTreeWidgetItem*> selectedServices)
{
    qDebug () << "scheduleHostDowntime" << selectedServices.size();

    authDialog = new QDialog(this);
        authDialog->setWindowTitle("Schedule Host Downtime");

    QGridLayout *authgrid =new QGridLayout(authDialog);


    authgrid->addWidget(confirmTree(selectedServices),0,0,1,3);
        confirmServiceTree->hideColumn(1);
    authgrid->addWidget(allServiceWidget(),2,0,1,3);
    authgrid->addWidget(downtimeWidget(),3,0,1,3);
    authgrid->addWidget(commentWidget(),4,0,1,3);
    authgrid->addWidget(authWidget(),5,0,1,3);
    authgrid->addWidget(buttonsWidget(),6,1,1,3);

    //authgrid->addItem(new QSpacerItem ( 85, 0, QSizePolicy::Fixed, QSizePolicy::Fixed ),7,0);

    if (authDialog->exec() == QDialog::Accepted)
    {
        qDebug() << "authDialog Accepted" << authUser->text();
        qApp->setProperty("ServerAuthUser",authUser->text());
        qApp->setProperty("ServerAuthPassword",authPassword->text());
        QString cmd;
        if (ackAllServices->isChecked())
            cmd=CMD_SCHEDULE_HOST_SVC_DOWNTIME;
        else
            cmd=CMD_SCHEDULE_HOST_DOWNTIME;

        if (qApp->property("ServerCommandType") == 0)
        {
            sendToCgi(cmd,selectedServices);
        }
        else
        {
            sendToLiveSocket(cmd,selectedServices);
        }
        //deleteLater();
    }
    else
    {
        qDebug() << "authDialog Not accepted";
        //deleteLater();
    }
    show();
}

void CommandWidget::scheduleServiceCheck(QList<QTreeWidgetItem*> selectedServices)
{
    authDialog = new QDialog(this);
        authDialog->setWindowTitle("Schedule Service Check");

    QGridLayout *authgrid =new QGridLayout(authDialog);


    authgrid->addWidget(confirmTree(selectedServices),0,0,1,3);

    authgrid->addWidget(allServiceWidget(),2,0,1,3);
    authgrid->addWidget(scheduleTimeWidget(),3,0,1,3);
    //authgrid->addWidget(commentWidget(),4,0,1,3);
    authgrid->addWidget(authWidget(),5,0,1,3);
    authgrid->addWidget(buttonsWidget(),6,1,1,3);

    //authgrid->addItem(new QSpacerItem ( 85, 0, QSizePolicy::Fixed, QSizePolicy::Fixed ),7,0);

    if (authDialog->exec() == QDialog::Accepted)
    {
        qDebug() << "authDialog Accepted" << authUser->text();
        qApp->setProperty("ServerAuthUser",authUser->text());
        qApp->setProperty("ServerAuthPassword",authPassword->text());
        QString cmd;
        if (ackAllServices->isChecked())
            cmd=CMD_SCHEDULE_FORCED_HOST_SVC_CHECKS;
        else
            cmd=CMD_SCHEDULE_FORCED_SVC_CHECK;

        if (qApp->property("ServerCommandType") == 0)
        {
            sendToCgi(cmd,selectedServices);
        }
        else
        {
            sendToLiveSocket(cmd,selectedServices);
        }
        //deleteLater();
    }
    else
    {
        qDebug() << "authDialog Not accepted";
        //deleteLater();
    }
    show();
}

void CommandWidget::enableHostCheck(QList<QTreeWidgetItem*> selectedServices)
{
    authDialog = new QDialog(this);
        authDialog->setWindowTitle("Enable Host Check");

    QGridLayout *authgrid =new QGridLayout(authDialog);

    authgrid->addWidget(confirmTree(selectedServices),0,0,1,3);
    authgrid->addWidget(authWidget(),5,0,1,3);
    authgrid->addWidget(buttonsWidget(),6,1,1,3);

    if (authDialog->exec() == QDialog::Accepted)
    {
        qApp->setProperty("ServerAuthUser",authUser->text());
        qApp->setProperty("ServerAuthPassword",authPassword->text());
        QString cmd;
            cmd=CMD_ENABLE_HOST_CHECK;

        if (qApp->property("ServerCommandType") == 0)
        {
            sendToCgi(cmd,selectedServices);
        }
        else
        {
            sendToLiveSocket(cmd,selectedServices);
        }
        //deleteLater();
    }
    else
    {
        qDebug() << "authDialog Not accepted";
        //deleteLater();
    }
    show();
}

void CommandWidget::disableHostCheck(QList<QTreeWidgetItem*> selectedServices)
{
    authDialog = new QDialog(this);
        authDialog->setWindowTitle("Disable Host Check");

    QGridLayout *authgrid =new QGridLayout(authDialog);

    authgrid->addWidget(confirmTree(selectedServices),0,0,1,3);
    authgrid->addWidget(authWidget(),5,0,1,3);
    authgrid->addWidget(buttonsWidget(),6,1,1,3);

    if (authDialog->exec() == QDialog::Accepted)
    {
        qApp->setProperty("ServerAuthUser",authUser->text());
        qApp->setProperty("ServerAuthPassword",authPassword->text());
        QString cmd;
            cmd=CMD_DISABLE_HOST_CHECK;

        if (qApp->property("ServerCommandType") == 0)
        {
            sendToCgi(cmd,selectedServices);
        }
        else
        {
            sendToLiveSocket(cmd,selectedServices);
        }
        //deleteLater();
    }
    else
    {
        qDebug() << "authDialog Not accepted";
        //deleteLater();
    }
    show();
}

void CommandWidget::enableServiceCheck(QList<QTreeWidgetItem*> selectedServices)
{
    authDialog = new QDialog(this);
        authDialog->setWindowTitle("Enable Service Check");

    QGridLayout *authgrid =new QGridLayout(authDialog);

    authgrid->addWidget(confirmTree(selectedServices),0,0,1,3);
    authgrid->addWidget(authWidget(),5,0,1,3);
    authgrid->addWidget(buttonsWidget(),6,1,1,3);

    if (authDialog->exec() == QDialog::Accepted)
    {
        qApp->setProperty("ServerAuthUser",authUser->text());
        qApp->setProperty("ServerAuthPassword",authPassword->text());
        QString cmd;
            cmd=CMD_ENABLE_SVC_CHECK;

        if (qApp->property("ServerCommandType") == 0)
        {
            sendToCgi(cmd,selectedServices);
        }
        else
        {
            sendToLiveSocket(cmd,selectedServices);
        }
        //deleteLater();
    }
    else
    {
        qDebug() << "authDialog Not accepted";
        //deleteLater();
    }
    show();
}

void CommandWidget::disableServiceCheck(QList<QTreeWidgetItem*> selectedServices)
{
    authDialog = new QDialog(this);
        authDialog->setWindowTitle("Disable Service Check");

    QGridLayout *authgrid =new QGridLayout(authDialog);

    authgrid->addWidget(confirmTree(selectedServices),0,0,1,3);
    authgrid->addWidget(authWidget(),5,0,1,3);
    authgrid->addWidget(buttonsWidget(),6,1,1,3);

    if (authDialog->exec() == QDialog::Accepted)
    {
        qApp->setProperty("ServerAuthUser",authUser->text());
        qApp->setProperty("ServerAuthPassword",authPassword->text());
        QString cmd;
            cmd=CMD_DISABLE_SVC_CHECK;

        if (qApp->property("ServerCommandType") == 0)
        {
            sendToCgi(cmd,selectedServices);
        }
        else
        {
            sendToLiveSocket(cmd,selectedServices);
        }
        //deleteLater();
    }
    else
    {
        qDebug() << "authDialog Not accepted";
        //deleteLater();
    }
    show();
}

void CommandWidget::enableHostNotification(QList<QTreeWidgetItem*> selectedServices)
{
    authDialog = new QDialog(this);
        authDialog->setWindowTitle("Enable Host Notification");

    QGridLayout *authgrid =new QGridLayout(authDialog);

    authgrid->addWidget(confirmTree(selectedServices),0,0,1,3);
    authgrid->addWidget(authWidget(),5,0,1,3);
    authgrid->addWidget(buttonsWidget(),6,1,1,3);

    if (authDialog->exec() == QDialog::Accepted)
    {
        qApp->setProperty("ServerAuthUser",authUser->text());
        qApp->setProperty("ServerAuthPassword",authPassword->text());
        QString cmd;
            cmd=CMD_ENABLE_HOST_NOTIFICATIONS;

        if (qApp->property("ServerCommandType") == 0)
        {
            sendToCgi(cmd,selectedServices);
        }
        else
        {
            sendToLiveSocket(cmd,selectedServices);
        }
        //deleteLater();
    }
    else
    {
        qDebug() << "authDialog Not accepted";
        //deleteLater();
    }
    show();
}

void CommandWidget::disableHostNotification(QList<QTreeWidgetItem*> selectedServices)
{
    authDialog = new QDialog(this);
        authDialog->setWindowTitle("Disable Host Notification");

    QGridLayout *authgrid =new QGridLayout(authDialog);

    authgrid->addWidget(confirmTree(selectedServices),0,0,1,3);
    authgrid->addWidget(authWidget(),5,0,1,3);
    authgrid->addWidget(buttonsWidget(),6,1,1,3);

    if (authDialog->exec() == QDialog::Accepted)
    {
        qApp->setProperty("ServerAuthUser",authUser->text());
        qApp->setProperty("ServerAuthPassword",authPassword->text());
        QString cmd;
            cmd=CMD_DISABLE_HOST_NOTIFICATIONS;

        if (qApp->property("ServerCommandType") == 0)
        {
            sendToCgi(cmd,selectedServices);
        }
        else
        {
            sendToLiveSocket(cmd,selectedServices);
        }
        //deleteLater();
    }
    else
    {
        qDebug() << "authDialog Not accepted";
        //deleteLater();
    }
    show();
}

void CommandWidget::enableServiceNotification(QList<QTreeWidgetItem*> selectedServices)
{
    authDialog = new QDialog(this);
        authDialog->setWindowTitle("Enable Service Notification");

    QGridLayout *authgrid =new QGridLayout(authDialog);

    authgrid->addWidget(confirmTree(selectedServices),0,0,1,3);
    authgrid->addWidget(authWidget(),5,0,1,3);
    authgrid->addWidget(buttonsWidget(),6,1,1,3);

    if (authDialog->exec() == QDialog::Accepted)
    {
        qApp->setProperty("ServerAuthUser",authUser->text());
        qApp->setProperty("ServerAuthPassword",authPassword->text());
        QString cmd;
            cmd=CMD_ENABLE_SVC_NOTIFICATIONS;

        if (qApp->property("ServerCommandType") == 0)
        {
            sendToCgi(cmd,selectedServices);
        }
        else
        {
            sendToLiveSocket(cmd,selectedServices);
        }
        //deleteLater();
    }
    else
    {
        qDebug() << "authDialog Not accepted";
        //deleteLater();
    }
    show();
}

void CommandWidget::disableServiceNotification(QList<QTreeWidgetItem*> selectedServices)
{
    authDialog = new QDialog(this);
        authDialog->setWindowTitle("Disable Service Notification");

    QGridLayout *authgrid =new QGridLayout(authDialog);

    authgrid->addWidget(confirmTree(selectedServices),0,0,1,3);
    authgrid->addWidget(authWidget(),5,0,1,3);
    authgrid->addWidget(buttonsWidget(),6,1,1,3);

    if (authDialog->exec() == QDialog::Accepted)
    {
        qApp->setProperty("ServerAuthUser",authUser->text());
        qApp->setProperty("ServerAuthPassword",authPassword->text());
        QString cmd;
            cmd=CMD_DISABLE_SVC_NOTIFICATIONS;

        if (qApp->property("ServerCommandType") == 0)
        {
            sendToCgi(cmd,selectedServices);
        }
        else
        {
            sendToLiveSocket(cmd,selectedServices);
        }
        //deleteLater();
    }
    else
    {
        qDebug() << "authDialog Not accepted";
        //deleteLater();
    }
    show();
}

void CommandWidget::acknowledgeService(QList<QTreeWidgetItem*> selectedServices)
{
    authDialog = new QDialog(this);
        authDialog->setWindowTitle("Acknowledge Service Problem");

    QGridLayout *authgrid =new QGridLayout(authDialog);


    authgrid->addWidget(confirmTree(selectedServices),0,0,1,3);

    authgrid->addWidget(scheduleTimeWidget(),3,0,1,3);
    authgrid->addWidget(commentWidget(),4,0,1,3);
    authgrid->addWidget(authWidget(),5,0,1,3);
    authgrid->addWidget(buttonsWidget(),6,1,1,3);

    //authgrid->addItem(new QSpacerItem ( 85, 0, QSizePolicy::Fixed, QSizePolicy::Fixed ),7,0);

    if (authDialog->exec() == QDialog::Accepted)
    {
        qDebug() << "authDialog Accepted" << authUser->text();
        qApp->setProperty("ServerAuthUser",authUser->text());
        qApp->setProperty("ServerAuthPassword",authPassword->text());
        qApp->setProperty("ServerAuthPassword",authPassword->text());

        if (qApp->property("ServerCommandType") == 0)
        {
            sendToCgi(CMD_ACKNOWLEDGE_SVC_PROBLEM,selectedServices);
        }
        else
        {
            sendToLiveSocket(CMD_ACKNOWLEDGE_SVC_PROBLEM,selectedServices);
        }
        //deleteLater();
    }
    else
    {
        qDebug() << "authDialog Not accepted";
        //deleteLater();
    }
    show();
}

void CommandWidget::acknowledgeHost(QList<QTreeWidgetItem*> selectedServices)
{
    authDialog = new QDialog(this);
        authDialog->setWindowTitle("Acknowledge Host Problem");

    QGridLayout *authgrid =new QGridLayout(authDialog);


    authgrid->addWidget(confirmTree(selectedServices),0,0,1,3);
        //don't show service
        confirmServiceTree->hideColumn(1);
    authgrid->addWidget(scheduleTimeWidget(),3,0,1,3);
    authgrid->addWidget(commentWidget(),4,0,1,3);
    authgrid->addWidget(authWidget(),5,0,1,3);
    authgrid->addWidget(buttonsWidget(),6,1,1,3);

    //authgrid->addItem(new QSpacerItem ( 85, 0, QSizePolicy::Fixed, QSizePolicy::Fixed ),7,0);

    if (authDialog->exec() == QDialog::Accepted)
    {
        qDebug() << "authDialog Accepted" << authUser->text();
        qApp->setProperty("ServerAuthUser",authUser->text());
        qApp->setProperty("ServerAuthPassword",authPassword->text());
        if (qApp->property("ServerCommandType") == 0)
        {
            sendToCgi(CMD_ACKNOWLEDGE_HOST_PROBLEM,selectedServices);
        }
        else
        {
            sendToLiveSocket(CMD_ACKNOWLEDGE_HOST_PROBLEM,selectedServices);
        }
        //deleteLater();
    }
    else
    {
        qDebug() << "authDialog Not accepted";
        //deleteLater();
    }
    show();
}


QTreeWidget* CommandWidget::confirmTree(QList<QTreeWidgetItem*> selectedServices)
{
//confirmServiceTree = new QTreeWidget(authDialog);
    confirmServiceTree->clear();
    confirmServiceTree->setColumnCount(2);
    confirmServiceTree->setContextMenuPolicy(Qt::CustomContextMenu);
    confirmServiceTree->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    confirmServiceTree->setHeaderLabels(QStringList() << "host_name" << "description");
    confirmServiceTree->header()->setVisible(false);
    confirmServiceTree->setMaximumHeight(60);
    confirmServiceTree->setSelectionMode(QAbstractItemView::NoSelection);

    for (int i=0; i<selectedServices.size();i++)
    {
        QTreeWidgetItem *item = new QTreeWidgetItem(QStringList() << selectedServices.at(i)->text(0)<< selectedServices.at(i)->text(1));
        confirmServiceTree->insertTopLevelItem(i,item);
        // looks bad item->setSelected(true);
     }
       // confirmServiceTree->insertTopLevelItems(0,selectedServices);
        confirmServiceTree->update();
 return confirmServiceTree;
}


void CommandWidget::timeSliderChanged(int val)
{
    int hours=val/60;
    int minutes=val-(hours*60);
    ackHourDuration->setValue(hours);
    ackMinuteDuration->setValue(minutes);

    qDebug() << "timeSlider"<<hours << minutes;

}

QWidget* CommandWidget::commentWidget()
{
    QWidget* commentWidget = new QWidget(0);
    QGridLayout *commentgrid = new QGridLayout(commentWidget);

    commentgrid->addWidget(new QLabel("Comment:",commentWidget),0,0);

    QDateTime dt = QDateTime::currentDateTime ();
    //ackComment = new QLineEdit("At: " + dt.toString("dd.MM.yy hh:mm"),commentWidget);
    ackComment->setText("At: " + dt.toString("dd.MM.yy hh:mm"));
    commentgrid->addWidget(ackComment,0,1);

    commentgrid->addItem(new QSpacerItem ( 90, 0, QSizePolicy::Fixed, QSizePolicy::Fixed ),1,0);
    return commentWidget;
}

// helper widget with user + pass
QWidget* CommandWidget::authWidget()
{

    QString ServerAuthUser=qApp->property("ServerAuthUser").toString();
    QString ServerAuthPassword=qApp->property("ServerAuthPassword").toString();

    QWidget *authWidget = new QWidget(0);

    QGridLayout *authgrid =new QGridLayout(authWidget);

    authgrid->addWidget(new QLabel("Auth User:", authWidget),0,0);
    authgrid->addWidget(new QLabel("Auth Password:", authWidget),1,0);

    //authUser = new QLineEdit(authWidget);
    if (ServerAuthUser.isEmpty())
    {
        authUser->setText(getenv("USERNAME"));
    }
    else
    {
        authUser->setText(ServerAuthUser);
    }
        authgrid->addWidget(authUser,0,1);

    //authPassword = new QLineEdit(authWidget);
        authPassword->setEchoMode(QLineEdit::Password);
        authPassword->setText(ServerAuthPassword);
        authgrid->addWidget(authPassword,1,1);
        authPassword->setFocus();

    authgrid->addItem(new QSpacerItem ( 90, 0, QSizePolicy::Fixed, QSizePolicy::Fixed ),2,0);
    return authWidget;
}

// Helper Widget with the Buttons
QWidget* CommandWidget::buttonsWidget()
{
    QWidget *buttonWidget = new QWidget(0);
    QGridLayout* authgrid = new QGridLayout(buttonWidget);
        authDialogOK = new QPushButton( "Ok", buttonWidget );
            connect(authDialogOK, SIGNAL(clicked()),authDialog, SLOT(accept()));
            authgrid->addWidget(authDialogOK,0,0);

        authDialogCancel = new QPushButton( "Cancel", buttonWidget );
            connect(authDialogCancel, SIGNAL(clicked()),authDialog, SLOT(reject()));
            authgrid->addWidget(authDialogCancel,0,1);

    authgrid->addItem(new QSpacerItem ( 90, 0, QSizePolicy::Fixed, QSizePolicy::Fixed ),1,0);
    return buttonWidget;
}

QWidget* CommandWidget::downtimeWidget()
{
    QDateTime dt = QDateTime::currentDateTime ();
    QWidget *timeWidget = new QWidget(0);
    QGridLayout *timeGrid = new QGridLayout(timeWidget);
    timeGrid->addWidget(new QLabel("Start:", timeWidget),0,0);

    //ackStart= new QDateTimeEdit(timeWidget);
        ackStart->setDateTime(dt);
        ackStart->setDisplayFormat("dd.MM.yy hh:mm");
        ackStart->setSelectedSection(QDateTimeEdit::MinuteSection);
    timeGrid->addWidget(ackStart,0,1,1,2);

    timeGrid->addWidget(new QLabel("Duration (h/m):", timeWidget),1,0);
    //timeGrid->addWidget(new QLabel("(h/m)", timeWidget),1,2);

    //ackHourDuration = new QSpinBox(timeWidget);
        ackHourDuration->setValue(0);
        ackHourDuration->setRange(0,100);
    timeGrid->addWidget(ackHourDuration,1,1);

    //ackMinuteDuration = new QSpinBox(timeWidget);
        ackMinuteDuration->setValue(15);
        ackMinuteDuration->setRange(0,59);
    timeGrid->addWidget(ackMinuteDuration,1,2);

    timeGrid->addItem(new QSpacerItem ( 90, 0, QSizePolicy::Expanding, QSizePolicy::Fixed ),1,3);

    timeSlider = new QSlider(Qt::Horizontal,timeWidget);
        timeSlider->setMinimum(10);
        timeSlider->setMaximum(2880);
        timeSlider->setTickInterval(288);
        connect( timeSlider, SIGNAL( valueChanged(int)) , this, SLOT( timeSliderChanged(int) ) );
     timeGrid->addWidget(timeSlider,2,1,1,3);

     timeGrid->addItem(new QSpacerItem ( 90, 0, QSizePolicy::Fixed, QSizePolicy::Fixed ),3,0);

    return timeWidget;
}

QWidget* CommandWidget::allServiceWidget()
{
    QWidget *asWidget = new QWidget(0);
    QGridLayout *asGrid = new QGridLayout(asWidget);

    asGrid->addItem(new QSpacerItem ( 90, 0, QSizePolicy::Fixed, QSizePolicy::Fixed ),0,0);

    //ackAllServices = new QCheckBox("Host and all services",asWidget);
    ackAllServices->setText("Host and all services");
    asGrid->addWidget(ackAllServices,0,1);

    return asWidget;
}

QWidget* CommandWidget::scheduleTimeWidget()
{
    QDateTime dt = QDateTime::currentDateTime ();
    QWidget *timeWidget = new QWidget(0);
    QGridLayout *timeGrid = new QGridLayout(timeWidget);
    timeGrid->addWidget(new QLabel("Start:", timeWidget),0,0);

    //ackStart= new QDateTimeEdit(timeWidget);
        ackStart->setDateTime(dt);
        ackStart->setDisplayFormat("dd.MM.yy hh:mm");
        ackStart->setSelectedSection(QDateTimeEdit::MinuteSection);
    timeGrid->addWidget(ackStart,0,1,1,2);

     timeGrid->addItem(new QSpacerItem ( 90, 0, QSizePolicy::Fixed, QSizePolicy::Fixed ),1,0);

    return timeWidget;
}


/*  send to nagios/thruk etc over cgi
 *  This is old stuff
 *
 *
 */

void CommandWidget::sendToCgi(QString lastCmdTyp, QList<QTreeWidgetItem*> selectedServices)
{

    QString ServerTimeFormat=qApp->property("ServerTimeFormat").toString();

    qDebug() << "start sending to monitoring server over http" << lastCmdTyp << "selectedServices.size()" <<selectedServices.size();
    qDebug() << "qApp->property(ServerCgiUrl).toString()" << qApp->property("ServerCgiUrl").toString();
    QDateTime dt = QDateTime::currentDateTime ();
    if ( (lastCmdTyp==CMD_SCHEDULE_SVC_DOWNTIME ) || (lastCmdTyp==CMD_SCHEDULE_HOST_DOWNTIME) )
    {
        if (ackAllServices->isChecked())
        {
            lastCmdTyp=CMD_SCHEDULE_HOST_SVC_DOWNTIME;
        }
    }

    for (int i = 0; i < selectedServices.size(); ++i)
    {
        QString selectedHost=((selectedServices.at(i)))->text(0);
        QString selectedService=((selectedServices.at(i)))->text(1);

    //Get Data
    QUrl monitoringUrl;
    QUrlQuery monitoringUrlQuery;

    monitoringUrl.setUrl(qApp->property("ServerCgiUrl").toString() + "cmd.cgi");

        if (!authUser->text().isEmpty())
        {
            monitoringUrl.setUserName(authUser->text().toLatin1());
        }
        if (!authPassword->text().isEmpty())
        {
            monitoringUrl.setPassword(authPassword->text().toLatin1());
        }

        QByteArray postData;

            // Host
            monitoringUrlQuery.addQueryItem("host",QUrl::toPercentEncoding(selectedHost));
            postData.append("host=" + QUrl::toPercentEncoding(selectedHost) + "&");

            //Service
            monitoringUrlQuery.addQueryItem("service",QUrl::toPercentEncoding(selectedService));
            postData.append("service=" + QUrl::toPercentEncoding(selectedService) + "&");

            // cmd mode 2 = commit
            monitoringUrlQuery.addQueryItem("cmd_mode","2");
            postData.append("cmd_mod=2&");

            //cmd_typ
            monitoringUrlQuery.addQueryItem("cmd_typ",lastCmdTyp);
            postData.append("cmd_typ=" + lastCmdTyp + "&");

            //Author (Reschedule?)
            monitoringUrlQuery.addQueryItem("com_author",authUser->text().toLatin1());
            postData.append("cmd_author=" + authUser->text().toLatin1() + "&");


            // force check for resched
            if (lastCmdTyp==CMD_SCHEDULE_SVC_CHECK)
            {
                monitoringUrlQuery.addQueryItem("force_check","TRUE");
                postData.append("force_check=TRUE&");
            }

            // start_time
            if ( (lastCmdTyp==CMD_SCHEDULE_SVC_CHECK) || (lastCmdTyp==CMD_SCHEDULE_SVC_DOWNTIME) || (lastCmdTyp==CMD_SCHEDULE_HOST_SVC_DOWNTIME) || (lastCmdTyp==CMD_SCHEDULE_HOST_DOWNTIME) )
            {
                monitoringUrlQuery.addQueryItem("start_time", QUrl::toPercentEncoding(ackStart->dateTime().toString(ServerTimeFormat)));
                postData.append("start_time=" + ackStart->dateTime().toString(ServerTimeFormat) + "&");
            }

            //downtime infos
            if ( (lastCmdTyp==CMD_SCHEDULE_SVC_DOWNTIME) || (lastCmdTyp==CMD_SCHEDULE_HOST_SVC_DOWNTIME) || (lastCmdTyp==CMD_SCHEDULE_HOST_DOWNTIME) )
            {
                monitoringUrlQuery.addQueryItem("trigger","0");
                postData.append("trigger=0&");

                monitoringUrlQuery.addQueryItem("fixed","1");
                postData.append("fixed=1&");

                monitoringUrlQuery.addQueryItem("hours",QString("%1").arg(ackHourDuration->value()));
                postData.append("hours=" + QString("%1").arg(ackHourDuration->value()) + "&");

                monitoringUrlQuery.addQueryItem("minutes",QString("%1").arg(ackMinuteDuration->value()));
                postData.append("minutes=" + QString("%1").arg(ackMinuteDuration->value()) + "&");

                monitoringUrlQuery.addQueryItem("end_time",QUrl::toPercentEncoding(ackStart->dateTime().addSecs((ackHourDuration->value()*3600)+(ackMinuteDuration->value()*60)).toString(ServerTimeFormat)));
                postData.append("end_time=" + ackStart->dateTime().addSecs((ackHourDuration->value()*3600)+(ackMinuteDuration->value()*60)).toString(ServerTimeFormat) +"&");
            }

           //Comment
            if ( (lastCmdTyp==CMD_ACKNOWLEDGE_SVC_PROBLEM) || (lastCmdTyp==CMD_SCHEDULE_SVC_DOWNTIME) || (lastCmdTyp==CMD_SCHEDULE_HOST_SVC_DOWNTIME) || (lastCmdTyp==CMD_SCHEDULE_HOST_DOWNTIME) )
            {
                monitoringUrlQuery.addQueryItem("com_data",QUrl::toPercentEncoding(ackComment->text()));
                postData.append("com_data=" + ackComment->text().toLatin1());
            }

    qDebug() << "Postdata" << postData;
    qDebug() << "monitoringUrl URL: "<< monitoringUrl.toString(QUrl::None);
    qDebug() << "URL Query: "<< monitoringUrlQuery.toString();

        //Data
        QNetworkRequest header;
        monitoringUrl.setQuery(monitoringUrlQuery);
        header.setUrl(monitoringUrl);
        header.setRawHeader( "User-Agent", "User Agent");
        header.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
        qDebug() << "send Http" << monitoringUrl;
        sendMonitoringHttp(header, postData);
    }
}

void CommandWidget::sendMonitoringHttp(QNetworkRequest header, QByteArray postData)
{
qDebug() << "PostData: "<< postData;

QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    reply=manager->post(header,postData);
        connect(reply, SIGNAL(readyRead()), this, SLOT(slotReadyReadHttp()));
}

void CommandWidget::slotReadyReadHttp()
{
    qDebug() << "slotReadyReadHttp";
    int start, stop;
    //<title>401 Authorization Required</title>

    QString all = reply->readAll();
        qDebug() << all;
    if (all.isEmpty())
        return;

    //monitoring Status
    start=all.indexOf("<DIV CLASS='infoMessage'>");
    if (start>0)
    {
        stop=all.indexOf("<BR>",start);
        if ( stop > start)
        {
            qDebug() << "mid" << all.mid(start+25,stop-(start+25));
            emit setStatusBar(QString(all.mid(start+25,stop-(start+25))).toLatin1(),0);
        }
    }

    start=all.indexOf("<DIV CLASS='errorMessage'>");
    if (start>0)
    {
        stop=all.indexOf("</DIV>",start);
        if ( stop > start)
        {
            qDebug() << "mid" << all.mid(start+26,stop-(start+26));
            emit setStatusBar(QString(all.mid(start+26,stop-(start+26))).toLatin1(),2);
        }
    }

    //monitoring Status
    start=all.indexOf("401 Auth");
    if (start>0)
    {
            emit setStatusBar("Command Error: 401 Authorization Required",2);
    }
}

void CommandWidget::sendToLiveSocket(QString lastCmdTyp, QList<QTreeWidgetItem*> selectedServices)
{
    qDebug()<< "sendToLiveSocket" << lastCmdTyp;
    for (int i = 0; i < selectedServices.size(); ++i)
    {
        QString selectedHost=((selectedServices.at(i)))->text(0);
        QString selectedService=((selectedServices.at(i)))->text(1);

        LiveSocketCommand lc;
        lc.setCommandType(lastCmdTyp);
        qDebug() << "lc.setCommandType(lastCmdTyp" << lastCmdTyp;
        lc.setHostName(selectedHost);

        //set all you know LiveSocketCommand will build it together
        if (!selectedService.isEmpty())
        {
            lc.setServiceDescription(selectedService);
        }

        if (ackStart)
        {
            lc.setCheckTime(ackStart->dateTime().toTime_t());
            lc.setStartTime(ackStart->dateTime().toTime_t());

            if ( (ackHourDuration) && (ackMinuteDuration) )
            {
                lc.setEndTime(ackStart->dateTime().addSecs((ackHourDuration->value()*3600)+(ackMinuteDuration->value()*60)).toTime_t());
                lc.setDuration((int)(ackHourDuration->value()*3600)+(ackMinuteDuration->value()*60));
                qDebug () << "(ackHourDuration->value()*3600)+(ackMinuteDuration->value()*60)" << (ackHourDuration->value()*3600)+(ackMinuteDuration->value()*60);
            }
        }

        if (authUser)
        {
            lc.setAuthor(authUser->text());
        }

        if(ackComment)
        {
            lc.setComment(ackComment->text());
        }

         QString request=lc.getLiveSocketCommand();

         qDebug() << "Sending Command Request" << request;
         emit setStatusBar("Sending Command Request", 0);
         emit sendLiveStatusData("Command", request);

    }
}

QComboBox* CommandWidget::getCommandComboBox(QString host_name, QString description, int State, int ServiceDetails)
{

    int state_type;
    int notifications_enabled;
    int host_notifications_enabled;
    int active_checks_enabled;
    int host_active_checks_enabled;

    if (ServiceDetails & 1)  state_type=1;                  else    state_type=0;
    if (ServiceDetails & 2)  notifications_enabled=1;       else    notifications_enabled=0;
    if (ServiceDetails & 4)  host_notifications_enabled=1;  else    host_notifications_enabled=0;
    if (ServiceDetails & 8)  active_checks_enabled=1;       else    active_checks_enabled=0;
    if (ServiceDetails & 16) host_active_checks_enabled=1;  else    host_active_checks_enabled=0;


    // if it has a description, its a service, if it has no host_name return empty combo

    commandBox = new QComboBox(this);
        comboBoxHost=host_name;
        comboBoxDescription=description;

        //connect(commandBox, SIGNAL(activated(QString)), this, SLOT(commandBoxTextChanged(QString)));

        commandBox->addItem("Enter Command:");
        commandBox->insertSeparator(commandBox->count());

    if (!description.isEmpty()) // We don't reschedule hosts
        commandBox->addItem(QPixmap(reschedule_xpm),"Reschedule");

    if ( (State > 0) &&(state_type > 0))
        commandBox->addItem(QPixmap(ack_xpm),"Acknowlegde");

        commandBox->addItem(QPixmap(downtime_xpm),"Schedule Downtime");

        commandBox->insertSeparator(commandBox->count());

    if (!description.isEmpty())
    {
        if (notifications_enabled == 0)
            commandBox->addItem(QIcon(":/images/notification_enabled.png"),"Enable Notifications");
        else
            commandBox->addItem(QIcon(":/images/notification_disabled.png"),"Disable Notifications");

        if (active_checks_enabled == 0)
            commandBox->addItem(QIcon(":/images/checks_enabled.png"),"Enable Active Checks");
        else
            commandBox->addItem(QIcon(":/images/checks_disabled.png"),"Disable Active Checks");

    }
    else
    {
    if (host_notifications_enabled == 0)
        commandBox->addItem(QIcon(":/images/notification_enabled.png"),"Host Enable Notifications");
    else
        commandBox->addItem(QIcon(":/images/notification_disabled.png"),"Host Disable Notifications");

    if (host_active_checks_enabled == 0)
        commandBox->addItem(QIcon(":/images/checks_enabled.png"),"Host Enable Active Checks");
    else
        commandBox->addItem(QIcon(":/images/checks_disabled.png"),"Host Disable Active Checks");
    }

    // ok hosts have nothing to do, so just hide it (downtime should go everytime)
    if ((commandBox->count() == 0) || (host_name.isEmpty()))
        commandBox->setVisible(false);

    // first add items and then connect
    connect(commandBox, SIGNAL(currentIndexChanged(int)), this, SLOT(commandBoxIndexChanged(int)));

    return commandBox;
}

void CommandWidget::commandBoxIndexChanged(int index)
{
    QString text=commandBox->currentText();

    qDebug() << text << index;
    // Little bit stupid, but we need a QTreewidgetitem List to call
    QTreeWidgetItem* Item = new QTreeWidgetItem(QStringList() << comboBoxHost << comboBoxDescription, 0);
    QList <QTreeWidgetItem*> ilist;
    ilist.append(Item);
    //Lets start with services
   if (!comboBoxDescription.isEmpty())
   {
       if (text.toLower().contains("acknowlegde"))
            acknowledgeService(ilist);
       else if (text.toLower().contains("reschedule"))
            scheduleServiceCheck(ilist);
       else if  (text.toLower().contains("downtime"))
           scheduleServiceDowntime(ilist);

       else if  (text.toLower().contains("enable notifications"))
           enableServiceNotification(ilist);
       else if  (text.toLower().contains("disable notifications"))
           disableServiceNotification(ilist);
       else if  (text.toLower().contains("enable active checks"))
           enableServiceCheck(ilist);
       else if  (text.toLower().contains("disable active checks"))
           disableServiceCheck(ilist);

       else if  (text.toLower().contains("host enable notifications"))
           enableHostNotification(ilist);
       else if  (text.toLower().contains("host disable notifications"))
           disableHostNotification(ilist);
       else if  (text.toLower().contains("host enable active checks"))
           enableHostCheck(ilist);
       else if  (text.toLower().contains("host disable active checks"))
           disableHostCheck(ilist);
   }
   else
   {
       if (text.toLower().contains("acknowlegde"))
            acknowledgeHost(ilist);
       else if  (text.toLower().contains("downtime"))
           scheduleHostDowntime(ilist);
       else if  (text.toLower().contains("host enable notifications"))
           enableHostNotification(ilist);
       else if  (text.toLower().contains("host disable notifications"))
           disableHostNotification(ilist);
       else if  (text.toLower().contains("host enable active checks"))
           enableHostCheck(ilist);
       else if  (text.toLower().contains("host disable active checks"))
           disableHostCheck(ilist);
   }
}
