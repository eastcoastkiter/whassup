#include "pcscene.h"

PCScene::PCScene(QObject * parent) : QGraphicsScene (parent)
{
    mouseIsPressed=false;
    firstMouseEvent=false;
}

PCScene::~PCScene(){}

void PCScene::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    QGraphicsScene::mousePressEvent(event);
         QGraphicsScene::mousePressEvent ( event );
         if( event->isAccepted() )
         {
              return;
         }
         else
         {
             mouseIsPressed=true;
        /*     qDebug() << event->scenePos();
             qDebug() << this->width();
             qDebug() << this->height();*/
            // update();

         }
         //if the event has not been processed by an item create a new item here
}

void PCScene::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
    QGraphicsScene::mouseReleaseEvent(event);
         QGraphicsScene::mouseReleaseEvent ( event );
         if( event->isAccepted() )
         {
              return;
         }
         else
         {
            mouseIsPressed=false;
            update();
         }

}

void PCScene::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
    QPointF newMousePosition=event->scenePos();
    QGraphicsScene::mouseMoveEvent(event);
         QGraphicsScene::mouseMoveEvent ( event );
         if( event->isAccepted() )
         {
              return;
         }
         else
         {
             if (mouseIsPressed)
             {


                if (firstMouseEvent)
                {
                   int xMove=oldMousePosition.x()-newMousePosition.x();
                   int yMove=oldMousePosition.y()-newMousePosition.y();
                 //   qDebug()<< "mouseMove" << event->scenePos() << xMove << yMove;
                        //setSceneRect(qreal x, qreal y, qreal w, qreal h)
                    setSceneRect((sceneRect().x()+xMove),(sceneRect().y()+yMove),sceneRect().width(),sceneRect().height());

                }
                    firstMouseEvent=true;

            }
             oldMousePosition=newMousePosition;
             update();
         }
}
