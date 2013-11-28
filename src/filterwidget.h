#ifndef FILTERWIDGET_H
#define FILTERWIDGET_H

#include <QWidget>
#include <QObject>
#include <QGridLayout>
#include <QLabel>
#include <QList>
#include <QPushButton>
#include <QDebug>
#include <QListWidget>
#include <QLineEdit>
#include <QRadioButton>

#include "icons.h"

class FilterWidget: public QWidget
{
	Q_OBJECT
	
public:
    FilterWidget(QString inputFilterText="",QStringList configServiceList=QStringList(),QStringList configHostList=QStringList(),QStringList configHostgroupList=QStringList(), QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~FilterWidget();
private:
    QListWidget *serviceBox;
    QListWidget *hostBox;
    QListWidget *hostgroupBox;
    QListWidget *conjunctionBox;
    QLineEdit *regLine;
    QListWidget *filterBox;
    QPushButton *includeButton;
    QPushButton *excludeButton;
    QPushButton *removeButton;
    QRadioButton *serviceRadio;
    QRadioButton *hostRadio;
private slots:
    void end();
    void boxItemSelectionChanged();
    void include();
    void exclude();
    void remove();

public slots:
    void setNewFilter(QStringList, QStringList, QStringList);
signals:
    void gotNewFilter(QString);
};
#endif
