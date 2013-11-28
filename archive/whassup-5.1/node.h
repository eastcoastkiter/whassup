#ifndef NODE_H
#define NODE_H


#include <QGraphicsItem>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QColor>
#include <QDebug>

#include "pcdialog.h"
#include "edge.h"
class Edge;
class PCDialog;
class Node : public QGraphicsItem
{
public:
    Node(QString hname=QString::null, int status=127,PCDialog* parent=0);
    enum { Type = 22222 };
      int type() const { return Type; }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    QString Hostname;
    int generation;

    void addEdge(Edge *edge);
    QList<Edge *> edges() const;
    QPainterPath shape() const;
    void calculateForces();
    bool advance();
    PCDialog* pcdialog;
    int state;
public slots:
    QString getHostname(){ return Hostname; }
    int getGeneration(){ return generation; }
    void setGeneration(int PA){ generation=PA; }
    void setState(int s){ state=s; update();}
private:
    QList<Edge *> edgeList;
    QPointF newPos;
//    QColor fc;
//    QColor sc;
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

};
#endif
