#ifndef PCDIALOG_H
#define PCDIALOG_H

#include <QWidget>
#include <QObject>
#include <QGraphicsView>
#include <QDebug>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QList>
#include <QMouseEvent>

#include "pcscene.h"
#include "node.h"
#include "icons.h"
class Node;
class PCDialog: public QGraphicsView
{
	Q_OBJECT
	
public:
    PCDialog(QString Hostname=QString::null,int  overallState=0, QWidget * parent = 0);
    ~PCDialog();
public:
        PCScene *pcscene;
        void itemMoved();
private:
        QGraphicsRectItem *rectItem;
        QList<QGraphicsItem*> parentNodes;
        QList<QGraphicsItem*> childNodes;
        QList<Node*> nodesList;
        Node* centernode;
        int timerId;
        int generation;
        bool forcing;
private slots:

public slots:
        void addParent(QString);
        void addChild(QString);
        void getParents();
        void force();
        void addNode(QString, QString, int, QString);
        void setGeneration(){ generation++; }
        QStringList getParentStringList();
        QStringList getCurrentGenerationStringList();
        QList<Node*> getCurrentNodeList(){ return nodesList; }
        bool nodeExistsFromNodename(QString);
        Node* nodeFromNodename(QString);
        void autoScale();
protected:
        void mouseDoubleClickEvent(QMouseEvent *);
        void wheelEvent(QWheelEvent* event);
        void timerEvent(QTimerEvent *event);
signals:
        void doubleClickedHost(QString);
};
#endif

