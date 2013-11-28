#ifndef PCSCENE_H
#define PCSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

class PCScene: public QGraphicsScene
{
    Q_OBJECT

public:
    PCScene(QObject * parent = 0);
    ~PCScene();
    bool mouseIsPressed;
    bool firstMouseEvent;
    QPointF oldMousePosition;
protected:
    void mousePressEvent ( QGraphicsSceneMouseEvent *);
    void mouseReleaseEvent ( QGraphicsSceneMouseEvent *);
    void mouseMoveEvent ( QGraphicsSceneMouseEvent *);
};
#endif // PCSCENE_H
