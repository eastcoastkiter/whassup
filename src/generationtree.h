#ifndef GENERATIONTREE_H
#define GENERATIONTREE_H

#include <QTreeWidget>
#include <QHeaderView>
#include <QJsonArray>
#include <QDebug>
#include <QGridLayout>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QCompleter>

#include "pcdialog.h"
#include "livesocketrequest.h"

class GenerationHostItem
{
public:
    GenerationHostItem(QString Host_name, QStringList Parents, QStringList Childs,int Overallstate){ host_name=Host_name; parents=Parents; everParents=Parents; childs=Childs; overallstate=Overallstate; }
    QString host_name;
    QStringList parents;
    QStringList everParents;
    QStringList childs;
    int overallstate;
    QString getHostname(){ return host_name; }
    int getParentCount(){ return parents.size(); }
    int getChildCount(){ return childs.size(); }
    QStringList getParents(){ return parents; }
    void setParents(QStringList list){ parents.clear(); parents=list; }
    void removeParent(QString p){ parents.removeAll(p); }
    QString getParentString(){ QString tmp; for (int i=0;i<everParents.size();i++){ tmp += everParents.at(i) + " "; } return tmp.trimmed(); }
    QString getChildString(){ QString tmp; for (int i=0;i<childs.size();i++){ tmp += childs.at(i) + " "; } return tmp.trimmed(); }
    int getOverallstate(){ return overallstate; }
};

class GenerationTreeWidget: public QWidget
{
    Q_OBJECT

public:
   GenerationTreeWidget(QWidget * parent = 0);
   ~GenerationTreeWidget(){}
private:
   QTreeWidget* generationTree;
   QList<GenerationHostItem*> generationHostItemList;
   PCDialog *pcdialog;
   int uid;
   QLineEdit* searchLogHost;

public slots:
   void gotGenerationData(QJsonArray);
   void gotLiveStatusJson(QJsonArray);
   void doubleClickedHost(QString);
private slots:
   void generationTreeItemDoubleClicked(QTreeWidgetItem*, int);
   void requestParents();
   void searchSlot();
signals:
   void requestLiveStatusData(int, QString, QString);
};
#endif // GENERATIONTREE_H
