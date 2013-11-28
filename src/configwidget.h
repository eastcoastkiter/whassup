#ifndef CONFIGWIDGET_H
#define CONFIGWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QStringList>
#include <QSpinBox>
#include <QLayout>
#include <QDebug>
#include <QFile>
#include <QFontDialog>
#include <QJsonArray>
#include <QComboBox>
#include <QSettings>
#include <QApplication>
#include <QLabel>
#include <QStandardPaths>
#include <QWhatsThisClickedEvent>
#include <QEvent>
#include <QMouseEvent>

#include "filterwidget.h"
#include "configdialog.h"

class ConfigWidget: public QWidget
{
    Q_OBJECT

public:
   ConfigWidget(QWidget * parent = 0);
    ~ConfigWidget();
public:
private:

   //set in qApp->property
QString ServerHostname;
    int ServerPort;
QString ServerAuthUser;
QString ServerAuthPassword;
QString ServerCgiUrl;
    int ServerCommandType;
    int ServerTimeZone;

    int ClientRefresh;
    int ClientTimezone;
QString ClientTimeFormat;
    int ClientLogLines;
QString ClientFilter;
QString ClientFont;
    int ClientAdditionalColumns;
    int ClientServiceFilters;
QStringList ClientHideTabs;
bool ClientHideOnClose;
bool ClientBlinkSystemTray;
bool ClientSortServiceByState;
QString ClientHostOpenUrl;
QString ClientServiceOpenUrl;
   // end of globals

    QLineEdit* inputHostname;
    QLineEdit* inputConfigStorage;
    QLineEdit* inputConfigFont;
    QSpinBox* inputAktualisierung;
    QSpinBox* inputTimezone;
    QSpinBox* inputLogLines;
    QLineEdit* inputFilter;
    QLineEdit* inputTimeFormat;
    QCheckBox *cb_Last_Change;
    QCheckBox *cb_State_Type;
    QCheckBox *cb_Downtimes;
    QCheckBox *cb_Downtime_Depth;
    QCheckBox *cb_host_state;
    QCheckBox *cb_host_state_type;
    QCheckBox *cb_host_downtimes;
    QCheckBox *cb_last_check;
    QStringList configServiceList;
    QStringList configHostList;
    QStringList configHostgroupList;
    QString configLocation;
    QString configHideTabString;
    QLineEdit *browserOpenUrl;

    QString ServerSsl;
    FilterWidget * filterWidget;
    bool hideInputFilter;
    QComboBox * configCommandBox;
    QStringList commandStringList;

    QLabel *serverOptionLabel;
    QLabel *hostnameLabel;
    QSpinBox* inputPort;
    QLineEdit* inputCgiURL;

    QLineEdit *configAuthUser;
    QSettings *settings;

    QCheckBox *hideOnclose;
    QCheckBox *blinkSystemTray;
    QString path;

    QCheckBox* sortServiceByState;
public slots:
    int getAdditionalColumns(){ return ClientAdditionalColumns; }
    void setAdditionalColumns(int aC) { ClientAdditionalColumns=aC; }
    int getServerPort(){ return ServerPort; }
    QString getServerHostname() { return ServerHostname; }
    QString getFont(){ return inputConfigFont->text(); }
    int getTimezone(){ return inputTimezone->value(); }
    QString getTimeFormat(){ return inputTimeFormat->text(); }
    QString getConfigHideTabString(){ return configHideTabString; }
    QString getServerSsl(){ return ServerSsl; }
    QString getServerCgiUrl(){ return ServerCgiUrl; }
    void setFont(QString f){ inputConfigFont->setText(f); }
    int getTimerValue(){ return inputAktualisierung->value(); }
    int getLogLines(){ return inputLogLines->value(); }
    QString getFilter(){ return inputFilter->text(); }
    //ServiceFilter not needed later
    void setServiceFilters(int sf){ ClientServiceFilters=sf; }
    int getServiceFilters(){ return ClientServiceFilters; }
    //
    QString getAuthUser(){ return ServerAuthUser; }
    QString getAuthPassword(){ return ServerAuthPassword; }
    void writeConfig();
    bool loadConfig();
    QString getInputCgiURL();
    QString getFilters();
    void gotConfigDataJson(QJsonArray);
    void setAuthentication(QString an, QString ap){ ServerAuthUser=an; ServerAuthPassword=ap; }
private slots:
    void columnsStateChange(int);
    void setTreeFont();
    void setCheckBoxes(int);
    void gotNewFilter(QString);
    void inputFilterButtonPressed();
    void serverConfig();
    void sendConfigChange(){ emit configChanged(); }
    void configCommandBoxChanged(QString);
    void timeformatCommandBoxChanged(QString);
    void browserConfig();
signals:
    void setHiddenColumns(int);
    void setStatusBar(QString, int);
    void configChanged();
    void writeSettings(QSettings*);
    void loadSettings(QSettings *);
protected:

};
#endif // CONFIGWIDGET_H
