#ifndef HELPWIDGET_H
#define HELPWIDGET_H

#include <QWidget>
#include <QObject>
#include <QGridLayout>
#include <QLabel>
#include <QList>
#include <QPushButton>
#include <QDebug>

#include "icons.h"

class HelpWidget: public QWidget
{
	Q_OBJECT
	
public:
    HelpWidget(QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~HelpWidget();
private:
private slots:
public slots:
signals:
};
#endif

