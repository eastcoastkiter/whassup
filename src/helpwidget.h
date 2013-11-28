#ifndef HELPWIDGET_H
#define HELPWIDGET_H

#include <QWidget>
#include <QObject>
#include <QGridLayout>
#include <QLabel>
#include <QList>
#include <QPushButton>
#include <QDebug>
#include <QTreeWidget>
#include <QTextBrowser>
#include <QFile>
#include <QDir>
#include <QString>
#include <QLineEdit>
#include <QPushButton>
#include <QHeaderView>
#include <QMimeType>


#include "icons.h"

class HelpWidget: public QWidget
{
	Q_OBJECT
	
public:
    HelpWidget(QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~HelpWidget();

    QTextBrowser* webview;
private:
    QTreeWidget* helpTree;
private slots:
    void addHelpFiles();
    void helpTreeDoubleClicked(QModelIndex);
    void showHtml(QString);
public slots:
signals:
};
#endif

