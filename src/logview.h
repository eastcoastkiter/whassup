#ifndef LOGVIEW_H
#define LOGVIEW_H


#include <QGraphicsView>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QColor>
#include <QDebug>
#include <QTreeWidgetItem>

#include "logviewitem.h"

class LogViewItem;
class LogView : public QGraphicsView
{
    Q_OBJECT

public:
    LogView(QWidget * parent = 0);
    ~LogView(){}

public slots:
private:
protected:
        void mouseDoubleClickEvent(QMouseEvent *);
signals:
        void doubleClickedLogItem(QPoint);
};
#endif
