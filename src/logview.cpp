#include "logview.h"


LogView::LogView(QWidget * parent): QGraphicsView ( parent )
{

}

void LogView::mouseDoubleClickEvent(QMouseEvent * mouseEvent)
{

    int sx=mapToScene(mouseEvent->pos()).x();
    int sy=mapToScene(mouseEvent->pos()).y();
    emit doubleClickedLogItem(QPoint(sx,sy));
//    qDebug() << "itemclicked" <<sx << sy;
/*
    if (scene()->items(QPointF(sx,sy)).count() == 1)
    {
        if ((scene()->items(QPointF(sx,sy)).last())->type() == 22223)
        {
            LogViewItem * item = dynamic_cast<LogViewItem*>(scene()->items(QPointF(sx,sy)).last());
             (scene()->items(QPointF(sx,sy)).last())->type();
       //     if (!item->getLogItem())
        //    {
                emit doubleClickedLogItem(item->getLogItem());
         //   }
        }
    }
    //mouseEvent->accept();
*/
}
