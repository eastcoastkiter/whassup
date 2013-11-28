#include "configwidget.h"


ConfigWidget::ConfigWidget(QWidget* parent) : QWidget(parent)
{

    QCoreApplication::setOrganizationName("Monitoring");;
    QCoreApplication::setApplicationName("whassup");

    path = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
            path += "/whassup.ini";

    settings = new QSettings(path, QSettings::IniFormat);
    settings->setFallbacksEnabled(false);
    qDebug() << "setting" << settings->scope() << settings->format() << path;

    hideInputFilter=false;
    ClientServiceFilters=30;
    ServerCommandType=0;
    commandStringList << "livestatus(unsecure)" << "fcgi";
    ClientHideOnClose=false;
    //CONFIG
    QGridLayout* configgrid = new QGridLayout(this);
        configgrid->addWidget(new QLabel("Server:", this),0,0);
        configgrid->addWidget(new QLabel("Refresh (Sec.):", this),1,0);
        configgrid->addWidget(new QLabel("Timezone (Hour):", this),2,0);
        configgrid->addWidget(new QLabel("Timeformat:", this),3,0);
        configgrid->addWidget(new QLabel("Log Lines:", this),4,0);
        configgrid->addWidget(new QLabel("Filter:", this),5,0);
        configgrid->addWidget(new QLabel("Font:", this),6,0);
        configgrid->addWidget(new QLabel("Browser Url:", this),7,0);

        QPushButton* openServerConfig = new QPushButton("Server Config",this);
            connect(openServerConfig, SIGNAL(clicked()), this, SLOT(serverConfig()));
        configgrid->addWidget(openServerConfig,0,1);

        inputAktualisierung = new QSpinBox(this);
            inputAktualisierung->setRange(1,65536);
            inputAktualisierung->setValue(20);
            connect( inputAktualisierung, SIGNAL( valueChanged ( int )) , this, SLOT( aktualisierungChanged(int) ) );
        configgrid->addWidget(inputAktualisierung,1,1);

        inputTimezone = new QSpinBox(this);
            inputTimezone->setRange(-23,+23);
            inputTimezone->setValue(1);
        configgrid->addWidget(inputTimezone,2,1);

        QRegExp tfrx("(d|M|y|h|H|m|s|z|:|/|-|\\s)+");
        QValidator *timeFormatValidator = new QRegExpValidator(tfrx, this);
        inputTimeFormat = new QLineEdit("dd-MM-yy HH:mm",this);
            inputTimeFormat->setValidator(timeFormatValidator);
        configgrid->addWidget(inputTimeFormat,3,1);

        inputLogLines = new QSpinBox(this);
            inputLogLines->setRange(0,5000);
            inputLogLines->setValue(500);
        configgrid->addWidget(inputLogLines,4,1);

        QGridLayout* configFilterGrid = new QGridLayout(this);
            inputFilter=new QLineEdit(this);
            configFilterGrid->addWidget(inputFilter,0,0);

            QPushButton* inputFilterButton = new QPushButton("..",this);
                connect( inputFilterButton, SIGNAL( pressed()) , this, SLOT( inputFilterButtonPressed() ) );
            configFilterGrid->addWidget(inputFilterButton,0,1);
        inputFilter->home(false);
        configgrid->addItem(configFilterGrid,5,1);

        QGridLayout* configFontGrid = new QGridLayout(this);

            inputConfigFont = new QLineEdit("Segoe UI,9,-1,5,50,0,0,0,0,0",this);
            inputConfigFont->setEnabled(false);
            configFontGrid->addWidget(inputConfigFont,0,0);

            QPushButton* inputFont = new QPushButton("Font",this);
                connect( inputFont, SIGNAL( pressed()) , this, SLOT( setTreeFont() ) );
            configFontGrid->addWidget(inputFont,0,1);

        configgrid->addItem(configFontGrid,6,1);

        QHBoxLayout* browsergrid = new QHBoxLayout(0);
        QPushButton* browserButton = new QPushButton("Url Config", this);
            connect(browserButton, SIGNAL(clicked()), this,SLOT(browserConfig()));
            browsergrid->addWidget(browserButton);
            browsergrid->addItem(new QSpacerItem ( 10, 0, QSizePolicy::Expanding, QSizePolicy::Fixed ));
        configgrid->addLayout(browsergrid,7,1);

        hideOnclose = new QCheckBox("Hide on Close",this);
        configgrid->addWidget(hideOnclose,8,1);

        blinkSystemTray = new QCheckBox("Blinking SystemTray");
        configgrid->addWidget(blinkSystemTray,9,1);

        sortServiceByState = new QCheckBox("Sort Service by State");
        configgrid->addWidget(sortServiceByState,10,1);

        QVBoxLayout *serviceTreeColumns = new QVBoxLayout(this);

        serviceTreeColumns->addWidget(new QLabel("Additional Columns:",this));

        cb_Last_Change = new QCheckBox("Last Change",this);
            cb_Last_Change->setChecked(true);
            connect( cb_Last_Change, SIGNAL( stateChanged (int)) , this, SLOT( columnsStateChange(int) ) );
            cb_Last_Change->setObjectName("cb_Last_Change");
            serviceTreeColumns->addWidget(cb_Last_Change);

        cb_State_Type = new QCheckBox("State Type",this);
            connect( cb_State_Type, SIGNAL( stateChanged (int)) , this, SLOT( columnsStateChange(int) ) );
            cb_State_Type->setObjectName("cb_State_Type");
            serviceTreeColumns->addWidget(cb_State_Type);

        cb_Downtimes = new QCheckBox("Downtimes",this);
            connect( cb_Downtimes, SIGNAL( stateChanged (int)) , this, SLOT( columnsStateChange(int) ) );
            cb_Downtimes->setObjectName("cb_Downtimes");
            serviceTreeColumns->addWidget(cb_Downtimes);

        cb_Downtime_Depth = new QCheckBox("Downtime Depth",this);
           connect( cb_Downtime_Depth, SIGNAL( stateChanged (int)) , this, SLOT( columnsStateChange(int) ) );
           cb_Downtime_Depth->setObjectName("cb_Downtime_Depth");
           serviceTreeColumns->addWidget(cb_Downtime_Depth);

        cb_host_state = new QCheckBox("Host State",this);
            connect( cb_host_state, SIGNAL( stateChanged (int)) , this, SLOT( columnsStateChange(int) ) );
            cb_host_state->setObjectName("cb_host_state");
            serviceTreeColumns->addWidget(cb_host_state);

        cb_host_state_type = new QCheckBox("Host State Type",this);
            connect( cb_host_state_type, SIGNAL( stateChanged (int)) , this, SLOT( columnsStateChange(int) ) );
            cb_host_state_type->setObjectName("cb_host_state_type");
            serviceTreeColumns->addWidget(cb_host_state_type);

        cb_host_downtimes= new QCheckBox("Host Downtimes",this);
            connect( cb_host_downtimes, SIGNAL( stateChanged (int)) , this, SLOT( columnsStateChange(int) ) );
            cb_host_downtimes->setObjectName("cb_last_check");
            serviceTreeColumns->addWidget(cb_host_downtimes);

        cb_last_check = new QCheckBox("Last Check",this);
            connect( cb_last_check, SIGNAL( stateChanged (int)) , this, SLOT( columnsStateChange(int) ) );
            cb_last_check->setObjectName("cb_last_check");
            serviceTreeColumns->addWidget(cb_last_check);


        configgrid->addLayout(serviceTreeColumns,0,5,7,2);

        configgrid->addItem(new QSpacerItem ( 100, 0, QSizePolicy::Fixed, QSizePolicy::Fixed ),11,2);

         QPushButton* configSubmitButton = new QPushButton("Save Config", this);
            configgrid->addWidget(configSubmitButton,10,5);
            connect(configSubmitButton,SIGNAL(clicked()),this,SLOT(sendConfigChange()));

        configgrid->addItem(new QSpacerItem ( 10, 0, QSizePolicy::Expanding, QSizePolicy::Expanding ),11,11);


// End of Config
}

ConfigWidget::~ConfigWidget(){}

void ConfigWidget::columnsStateChange(int c)
{
    (void) c;
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
    //globalColumnInt=columnInt;
    ClientAdditionalColumns=columnInt;
        qApp->setProperty("ClientServiceFilters",ClientServiceFilters);
    emit setHiddenColumns(columnInt);

}

void ConfigWidget::writeConfig()
{
        /* we saved, set Properties
         * Server Properties will be handeld by the Server QDialog accept
         */

            settings->beginGroup("Server");
                    settings->setValue("ServerHostname",ServerHostname);
                    settings->setValue("ServerPort",ServerPort);
                    settings->setValue("ServerSsl",ServerSsl);;
                    settings->setValue("ServerCgiUrl",ServerCgiUrl);
                    settings->setValue("ServerCommandType",ServerCommandType);
                    settings->setValue("ServerAuthUser",ServerAuthUser);
            settings->endGroup();

            /* Client Settings are handled in this widget
             * we must set propertys here
             */
            ClientRefresh=inputAktualisierung->value();
                qApp->setProperty("ClientRefresh",ClientRefresh);
            ClientTimezone=inputTimezone->value();
                qApp->setProperty("ClientTimezone",ClientTimezone);
            ClientTimeFormat=inputTimeFormat->text();
                qApp->setProperty("ClientTimeFormat",ClientTimeFormat);
            ClientLogLines=inputLogLines->value();
                qApp->setProperty("ClientLogLines",ClientLogLines);
            ClientFilter=inputFilter->text();
                qApp->setProperty("ClientFilter",ClientFilter);
            ClientFont=inputConfigFont->text();
                qApp->setProperty("ClientFont",ClientFont);
            ClientAdditionalColumns=getAdditionalColumns();
                qApp->setProperty("ClientAdditionalColumns",ClientAdditionalColumns);
            ClientHideOnClose=hideOnclose->isChecked();
                qApp->setProperty("ClientHideOnClose",ClientHideOnClose);
            ClientBlinkSystemTray=blinkSystemTray->isChecked();
                qApp->setProperty("ClientBlinkSystemTray",ClientBlinkSystemTray);
            ClientSortServiceByState=sortServiceByState->isChecked();
                qApp->setProperty("ClientSortServiceByState",ClientSortServiceByState);
                //
            columnsStateChange(0);
                qApp->setProperty("ClientServiceFilters",ClientServiceFilters);

                // set by Dialog ClientHostOpenUrl ClientServiceOpenUrl

            settings->beginGroup("Client");
                settings->setValue("ClientRefresh",ClientRefresh);
                settings->setValue("ClientTimezone",ClientTimezone);
                settings->setValue("ClientTimeFormat",ClientTimeFormat);
                settings->setValue("ClientLogLines",ClientLogLines);
                settings->setValue("ClientFilter",ClientFilter);
                settings->setValue("ClientFont",ClientFont);
                settings->setValue("ClientAdditionalColumns",ClientAdditionalColumns);
                settings->setValue("ClientServiceFilters",ClientServiceFilters);
                settings->setValue("ClientHideTabs",ClientHideTabs);
                settings->setValue("ClientHideOnClose",ClientHideOnClose);
                settings->setValue("ClientBlinkSystemTray",ClientBlinkSystemTray);
                settings->setValue("ClientSortServiceByState",ClientSortServiceByState);
                settings->setValue("ClientHostOpenUrl",ClientHostOpenUrl);
                settings->setValue("ClientServiceOpenUrl",ClientServiceOpenUrl);
            settings->endGroup();

        emit writeSettings(settings);
        settings->sync();
        if (settings->status() == 0 )
        {
            emit setStatusBar( "Successfully load config from " + path, 0);
        }
        else
        {
            emit setStatusBar( "Error loading config from " + path, settings->status());
        }
}

bool ConfigWidget::loadConfig()
{
    settings->beginGroup("Server");
        ServerHostname=settings->value("ServerHostname","monitoring.local").toString();
        ServerPort=settings->value("ServerPort",6557).toInt();
        ServerSsl=settings->value("ServerSsl","false").toString();
        ServerCgiUrl=settings->value("ServerCgiUrl","http://monitoring.local/monitoring/cgi-bin/").toString();
        ServerCommandType=settings->value("ServerCommandType",0).toInt();
        ServerAuthUser=settings->value("ServerAuthUser",getenv("USERNAME")).toString();
    settings->endGroup();

    // set "global" Properties
    qApp->setProperty("ServerHostname",ServerHostname);
    qApp->setProperty("ServerPort",ServerPort);
    qApp->setProperty("ServerSsl",ServerSsl);
    qApp->setProperty("ServerCgiUrl",ServerCgiUrl);
    qApp->setProperty("ServerCommandType",ServerCommandType);
    qApp->setProperty("ServerAuthUser",ServerAuthUser);
    // forms will be filled in server qdialog

    settings->beginGroup("Client");
        ClientRefresh=settings->value("ClientRefresh",20).toInt();
        ClientTimezone=settings->value("ClientTimezone",-1).toInt();
        ClientTimeFormat=settings->value("ClientTimeFormat","dd-MM-yy HH:mm").toString();
        ClientLogLines=settings->value("ClientLogLines",500).toInt();
        ClientFilter=settings->value("ClientFilter","").toString();
        ClientFont=settings->value("ClientFont","Segoe UI,9,-1,5,50,0,0,0,0,0").toString();
        ClientAdditionalColumns=settings->value("ClientAdditionalColumns",1).toInt();
        ClientServiceFilters=settings->value("ClientServiceFilters",30).toInt();
        ClientHideTabs=settings->value("ClientHideTabs").toStringList();
        ClientHideOnClose=settings->value("ClientHideOnClose",false).toBool();
        ClientBlinkSystemTray=settings->value("ClientBlinkSystemTray",true).toBool();
        ClientSortServiceByState=settings->value("ClientSortServiceByState",false).toBool();
        ClientHostOpenUrl=settings->value("ClientHostOpenUrl").toString(); // we set default in dialog
        ClientServiceOpenUrl=settings->value("ClientServiceOpenUrl").toString(); // we set default in dialog
    settings->endGroup();


    // set "global" Properties
    qApp->setProperty("ClientRefresh",ClientRefresh);
        inputAktualisierung->setValue(ClientRefresh);
    qApp->setProperty("ClientTimezone",ClientTimezone);
        inputTimezone->setValue(ClientTimezone);
    qApp->setProperty("ClientTimeFormat",ClientTimeFormat);
        inputTimeFormat->setText(ClientTimeFormat);
    qApp->setProperty("ClientLogLines",ClientLogLines);
        inputLogLines->setValue(ClientLogLines);
    qApp->setProperty("ClientFilter",ClientFilter);
        inputFilter->setText(ClientFilter);
    qApp->setProperty("ClientFont",ClientFont);
        inputConfigFont->setText(ClientFont);
    qApp->setProperty("ClientAdditionalColumns",ClientAdditionalColumns);
        setAdditionalColumns(ClientAdditionalColumns);
        setCheckBoxes(ClientAdditionalColumns);
    qApp->setProperty("ClientServiceFilters",ClientServiceFilters);
        setServiceFilters(ClientServiceFilters);
    qApp->setProperty("ClientHideTabs",ClientHideTabs);
    qApp->setProperty("ClientHideOnClose",ClientHideOnClose);
        hideOnclose->setChecked(ClientHideOnClose);
    qApp->setProperty("ClientBlinkSystemTray",ClientBlinkSystemTray);
        blinkSystemTray->setChecked(ClientBlinkSystemTray);
    qApp->setProperty("ClientSortServiceByState",ClientSortServiceByState);
        sortServiceByState->setChecked(ClientSortServiceByState);
    qApp->setProperty("ClientHostOpenUrl",ClientHostOpenUrl);
    qApp->setProperty("ClientServiceOpenUrl",ClientServiceOpenUrl);

    for (int i=0;i<ClientHideTabs.size();i++)
    {
        emit setHiddenColumns(ClientHideTabs.at(i).toInt());
    }
    // Info on the qApp
    emit loadSettings(settings);

    settings->sync();
    if (settings->status() == 0 )
    {
        emit setStatusBar( "Successfully load config from " + path, 0);
    }
    else
    {
        emit setStatusBar( "Error loading config from " + path, settings->status());
    }

    return true;
}

QString ConfigWidget::getInputCgiURL()
{
    if (qApp->property("ServerCgiUrl").toString().trimmed().endsWith("/"))
        return qApp->property("ServerCgiUrl").toString();
    else
    {
        qApp->setProperty("ServerCgiUrl", qApp->property("ServerCgiUrl").toString() + "/");
        return qApp->property("ServerCgiUrl").toString();

    }
}

void ConfigWidget::setTreeFont()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, getFont(), this);
    if (ok)
    {
        setFont(font.toString());
        qDebug() << "Font" << font.toString();
    }
}

void ConfigWidget::gotNewFilter(QString newFilter)
{
    qDebug() << newFilter;
        inputFilter->setText(newFilter);
        filterWidget->hide();
}

QString ConfigWidget::getFilters()
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
        //qDebug() << "Filter" << Filter;
        //Filter="Filter: description != 100_WINDOWS-hosts\n";

    }
    return Filter;
}

void ConfigWidget::setCheckBoxes(int columnInt)
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

void ConfigWidget::inputFilterButtonPressed()
{
    if (!hideInputFilter)
    {
    filterWidget = new FilterWidget(getFilter(),configServiceList, configHostList, configHostList,this, Qt::Dialog);
        connect( filterWidget, SIGNAL( gotNewFilter(QString)) , this, SLOT( gotNewFilter(QString)) );
        hideInputFilter=true;
    }
    else
    {
        filterWidget->setNewFilter(configServiceList, configHostList, configHostList);
    }
    filterWidget->show();
}

void ConfigWidget::serverConfig()
{
    ConfigDialog *serverDialog = new ConfigDialog(this);

        serverDialog->setWindowTitle("Server Configuration");

    QGridLayout *configgrid =new QGridLayout(serverDialog);

    configgrid->addWidget(new QLabel("Communication:", this),0,0);

    hostnameLabel = new QLabel("Hostname:", this);
    configgrid->addWidget(hostnameLabel,1,0);

    serverOptionLabel = new QLabel("Port:", this);
    configgrid->addWidget(serverOptionLabel,2,0);

    configgrid->addWidget(new QLabel("Auth User:", this),3,0);


    configCommandBox = new QComboBox(this);
        configCommandBox->addItems(commandStringList);
        configCommandBox->setCurrentIndex(ServerCommandType);
        connect(configCommandBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(configCommandBoxChanged(QString)));
        configgrid->addWidget(configCommandBox,0,1);
        configgrid->addItem(new QSpacerItem ( 100, 30, QSizePolicy::Expanding, QSizePolicy::Fixed ),1,2,1,2);

        //hostname, only needed if xinedt or whassup
        QRegExp rx("(\\w*|\\.|\\d*|\\-*)+");
        QValidator *hostnameValidator = new QRegExpValidator(rx, this);
        inputHostname = new QLineEdit(ServerHostname,this);
            inputHostname->setValidator(hostnameValidator);
        configgrid->addWidget(inputHostname,1,1,1,3);

    //We set both on the same grid point, buzt only one visible
    inputPort = new QSpinBox(this);
        inputPort->setRange(1,65536);
        inputPort->setValue(ServerPort);
    configgrid->addWidget(inputPort,2,1);

    inputCgiURL = new QLineEdit(ServerCgiUrl,this);
    configgrid->addWidget(inputCgiURL,2,1,1,3);

    //hide whats needed
    configCommandBoxChanged(configCommandBox->currentText());

    //still not working so visible = false
                                    QCheckBox* checkUseSsl = new QCheckBox("Use SSL", serverDialog);
                                    configgrid->addWidget(checkUseSsl,1,2);

                                    if (ServerSsl == "true")
                                        checkUseSsl->setChecked(true);
                                    else
                                        checkUseSsl->setChecked(false);

                                    checkUseSsl->setVisible(false); //Still not working



    if(ServerCgiUrl.isEmpty())
    {
        QString cgiURL="http://" + ServerHostname + "/whassup/index.fcgi";
        qApp->setProperty("ServerCgiUrl",ServerCgiUrl);
        inputCgiURL->setText(cgiURL);
    }

    configAuthUser = new QLineEdit(this);

        if (ServerAuthUser.isEmpty())
        {
            configAuthUser->setText(getenv("USERNAME"));
        }
        else
        {
            //QSettings settings;
            configAuthUser->setText(qApp->property("ServerAuthUser").toString());
            //configAuthUser->setText(ServerAuthUser);
        }
        configgrid->addWidget(configAuthUser,3,1);
        configgrid->addItem(new QSpacerItem ( 100, 30, QSizePolicy::Expanding, QSizePolicy::Fixed ),3,2,1,2);


    QHBoxLayout* buttonBox = new QHBoxLayout(serverDialog);
    QPushButton* serverDialogOK = new QPushButton( "Ok", serverDialog );
        connect(serverDialogOK, SIGNAL(clicked()),serverDialog, SLOT(accept()));
        buttonBox->addWidget(serverDialogOK);

    QPushButton* serverDialogCancel = new QPushButton( "Cancel", serverDialog );
        connect(serverDialogCancel, SIGNAL(clicked()),serverDialog, SLOT(reject()));
        buttonBox->addWidget(serverDialogCancel);

    configgrid->addLayout(buttonBox,5,1);

    configgrid->addItem(new QSpacerItem ( 200, 30, QSizePolicy::Expanding, QSizePolicy::Fixed ),4,1);

    if (serverDialog->exec() == QDialog::Accepted)
    {
        ServerHostname=inputHostname->text();
            qApp->setProperty("ServerHostname",ServerHostname);
        ServerPort=inputPort->value();
            qApp->setProperty("ServerPort",ServerPort);
        ServerCgiUrl=inputCgiURL->text();
            qApp->setProperty("ServerCgiUrl",ServerCgiUrl);
        ServerAuthUser=configAuthUser->text();
            qApp->setProperty("ServerAuthUser",ServerAuthUser);
        ServerCommandType=configCommandBox->currentIndex();
            qApp->setProperty("ServerCommandType",ServerCommandType);
        ServerAuthUser=configAuthUser->text();
        if (checkUseSsl->isChecked()) ServerSsl="true"; else ServerSsl="false";
            qApp->setProperty("ServerAuthUser",ServerAuthUser);


        writeConfig();

    }
}

void ConfigWidget::gotConfigDataJson(QJsonArray jsonUpperArray)
{

//These QStringLists could be used to create selectors/choosers
        for (int k = 0; k < jsonUpperArray.size()-1; ++k)
        {
            QJsonArray jsonArray=jsonUpperArray.at(k).toArray();
            qDebug() << "hmm"<< jsonArray;

            configServiceList << jsonArray.at(0).toString();
            configHostList << jsonArray.at(1).toString();

            QJsonArray hostgroupsJsonArray=jsonArray.at(2).toArray();
            for (int i = 0; i < hostgroupsJsonArray.size(); ++i)
            {
                configHostgroupList << hostgroupsJsonArray.at(i).toString();
            }
        }
        //qDebug() << "configServiceList" << configServiceList;

        configServiceList.sort();
        configServiceList.removeDuplicates();

        configHostList.sort();
        configHostList.removeDuplicates();

        configHostgroupList.sort();
        configHostgroupList.removeDuplicates();

}

void ConfigWidget::configCommandBoxChanged(QString text)
{

    qDebug() << "ServerCommandType" << ServerCommandType;

    if (text == commandStringList.first())
    {
        serverOptionLabel->setText("Port");
        hostnameLabel->setVisible(true);
        inputCgiURL->setVisible(false);
        inputPort->setVisible(true);
        inputHostname->setVisible(true);
    }
    else
    {
        serverOptionLabel->setText("fcgi Path");
        hostnameLabel->setVisible(false);
        inputHostname->setVisible(false);
        inputPort->setVisible(false);
        inputCgiURL->setVisible(true);
    }
}

void ConfigWidget::timeformatCommandBoxChanged(QString text)
{
    (void) text;
}

void ConfigWidget::browserConfig()
{
    ConfigDialog *browserDialog = new ConfigDialog(this);
        browserDialog->setMinimumWidth(300);
        browserDialog->setWindowTitle("Url Configuration");

    QGridLayout *configgrid =new QGridLayout(browserDialog);

    configgrid->addWidget(new QLabel("Host Url:", this),0,0);
    configgrid->addWidget(new QLabel("Service Url:", this),1,0);

    QLineEdit *hostBrowserUrl = new QLineEdit(browserDialog);
        ClientHostOpenUrl=qApp->property("ClientHostOpenUrl").toString();
        if (ClientHostOpenUrl.isEmpty())
            hostBrowserUrl->setText("http://monitoring.local/monitoring/cgi-bin/extinfo.cgi?type=1&host=%HOST%");
        else
            hostBrowserUrl->setText(ClientHostOpenUrl);
    hostBrowserUrl->home(false);
    configgrid->addWidget(hostBrowserUrl,0,1);

    QLineEdit *serviceBrowserUrl = new QLineEdit(browserDialog);
        ClientServiceOpenUrl=qApp->property("ClientServiceOpenUrl").toString();
        if (ClientServiceOpenUrl.isEmpty())
            serviceBrowserUrl->setText("http://monitoring.local/monitoring/cgi-bin/extinfo.cgi?type=2&host=%HOST%&service=%SERVICE%");
        else
            serviceBrowserUrl->setText(ClientServiceOpenUrl);
    serviceBrowserUrl->home(false);
    configgrid->addWidget(serviceBrowserUrl,1,1);

    QHBoxLayout* buttonBox = new QHBoxLayout(browserDialog);
    QPushButton* serverDialogOK = new QPushButton( "Ok", browserDialog );
        connect(serverDialogOK, SIGNAL(clicked()),browserDialog, SLOT(accept()));
        buttonBox->addWidget(serverDialogOK);

    QPushButton* serverDialogCancel = new QPushButton( "Cancel", browserDialog );
        connect(serverDialogCancel, SIGNAL(clicked()),browserDialog, SLOT(reject()));
        buttonBox->addWidget(serverDialogCancel);

    configgrid->addLayout(buttonBox,2,1);

    if (browserDialog->exec() == QDialog::Accepted)
    {
        ClientHostOpenUrl=hostBrowserUrl->text();
            qApp->setProperty("ClientHostOpenUrl",ClientHostOpenUrl);
        ClientServiceOpenUrl=serviceBrowserUrl->text();
            qApp->setProperty("ClientServiceOpenUrl",ClientServiceOpenUrl);
    }
   // browserDialog->show();
}
