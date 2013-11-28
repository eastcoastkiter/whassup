#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <QWidget>
#include <QObject>
#include <QGridLayout>
#include <QLabel>
#include <QList>
#include <QPushButton>
#include <QTimer>
#include <QRect>
#include <QDebug>

#include "icons.h"

class InfoWidget: public QWidget
{
	Q_OBJECT
	
public:
    InfoWidget(QString message = "Loading", QWidget * parent = 0, Qt::WindowFlags f = Qt::Popup);
    ~InfoWidget();
private:
    QLabel* messageLabel;
    QTimer* timer;
    QWidget* Parent;
private slots:
public slots:
    void showInfoMessage(QString);
    void hideInfoMessage();
signals:
};
#endif

