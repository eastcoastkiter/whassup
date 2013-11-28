#include "logviewitem.h"


LogViewItem::LogViewItem(int State, QGraphicsItem * parent): QGraphicsItem ( parent )
{
    state=State;
}

void LogViewItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    (void) option;
/*
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::darkGray);
    painter->drawEllipse(-7, -7, 20, 20);



    painter->setBrush(Qt::green);

    painter->setPen(QPen(Qt::black, 0));
    painter->drawEllipse(-10, -10, 20, 20);
 */
    QPixmap pix;
    if (state == 0)
        pix = QPixmap(":/images/ok.png");
    else if (state == 1 )
        pix = QPixmap(":/images/warning.png");
    else
        pix = QPixmap(":/images/critical.png");

    painter->drawPixmap(pix.rect(),pix);
}

QRectF LogViewItem::boundingRect() const
{
    qreal adjust = 2;
    return QRectF( -10 - adjust, -10 - adjust, 23 + adjust, 23 + adjust);
}

