#ifndef COMMANDWIDGET_H
#define COMMANDWIDGET_H
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QStringList>
#include <QSpinBox>
#include <QLayout>
#include <QDebug>
#include <QDialog>
#include <QList>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QUrl>
#include <QUrlQuery>
#include <QCheckBox>
#include <QSlider>
#include <QDateTime>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSettings>
#include <QComboBox>

class ServiceTree;
#include "whassup.h"
#include "livesocketcommand.h"
#include "livesocket.h"
#include "globals.h"


class CommandWidget: public QWidget
{
    Q_OBJECT

public:
    CommandWidget(QWidget * parent = 0);
    ~CommandWidget();
public:
private:
    QDialog *authDialog;
    QLineEdit* authUser;
    QLineEdit* authPassword;
    QPushButton* authDialogOK;
    QPushButton* authDialogCancel;
    QCheckBox* ackAllServices;
    QDateTimeEdit* ackStart;
    QSpinBox* ackMinuteDuration;
    QSpinBox* ackHourDuration;
    QSlider *timeSlider;
    QLineEdit* ackComment;
    QNetworkReply * reply;
    QTreeWidget *confirmServiceTree;
    QComboBox *commandBox;
    QString comboBoxHost;
    QString comboBoxDescription;

public slots:
    bool showAuthCommandWidget();
    void scheduleServiceDowntime(QList<QTreeWidgetItem*>);
    void scheduleHostDowntime(QList<QTreeWidgetItem*>);
    void scheduleServiceCheck(QList<QTreeWidgetItem*>);
    void acknowledgeService(QList<QTreeWidgetItem*>);
    void acknowledgeHost(QList<QTreeWidgetItem*>);
    void enableHostCheck(QList<QTreeWidgetItem*>);
    void disableHostCheck(QList<QTreeWidgetItem*>);
    void enableServiceCheck(QList<QTreeWidgetItem*>);
    void disableServiceCheck(QList<QTreeWidgetItem*>);
    void enableHostNotification(QList<QTreeWidgetItem*>);
    void disableHostNotification(QList<QTreeWidgetItem*>);
    void enableServiceNotification(QList<QTreeWidgetItem*>);
    void disableServiceNotification(QList<QTreeWidgetItem*>);
    QComboBox* getCommandComboBox(QString,QString, int, int);

private slots:
    QWidget* authWidget();
    QWidget* downtimeWidget();
    QWidget* scheduleTimeWidget();
    QWidget* buttonsWidget();
    QWidget* commentWidget();
    QWidget* allServiceWidget();
    QTreeWidget* confirmTree(QList<QTreeWidgetItem*> selectedServices);
    void sendToCgi(QString, QList<QTreeWidgetItem*>);
    void sendMonitoringHttp(QNetworkRequest, QByteArray);
    void sendToLiveSocket(QString, QList<QTreeWidgetItem*>);
    void timeSliderChanged(int);
    void slotReadyReadHttp();
    void commandBoxIndexChanged(int);
signals:
    void setStatusBar(QString, int);
    void sendLiveStatusData(QString, QString);

protected:

};
#endif // COMMANDWIDGET_H
