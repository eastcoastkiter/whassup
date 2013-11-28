#ifndef LOGVIEWITEM_H
#define LOGVIEWITEM_H


#include <QGraphicsItem>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QColor>
#include <QDebug>
#include <QTreeWidgetItem>
#include <QMouseEvent>

class LogViewItem : public QGraphicsItem
{
public:
    LogViewItem(int State=0, QGraphicsItem *parent=0);
    enum { Type = 22223 };
      int type() const { return Type; }
      int state;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
public slots:
private:
protected:
};
#endif
