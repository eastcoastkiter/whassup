#include "pcdialog.h"
using namespace std;

PCDialog::PCDialog(QString Hostname, int  overallState, QWidget * parent): QGraphicsView( parent )
{
    (void) Hostname;
    ostate=overallState;
    forcing=false;
    generation=1;
   // scene = new QGraphicsScene(this);
   //    pcscene = new PCScene(this);
   //    setScene(pcscene);

/*    QBrush redBrush(Qt::red);
    QBrush blackBrush(Qt::black);
    QPen blackPen(Qt::black);
        blackPen.setWidth(6);
    QPen bluePen(Qt::blue);
        bluePen.setWidth(4);
*/
/*     centernode = new Node(Hostname, overallState,this);
        centernode->setPos(0, 0);
        centernode->setGeneration(0);
        pcscene->addItem(centernode);
        nodesList.append(centernode);
*/       // centernode->setState(1);
        //qDebug() << "Overall State:" << overallState;
}

PCDialog::~PCDialog()
{
    delete pcscene;
}

void PCDialog::setCenterNode(QString Hostname, int state)
{
    //pcscene->clear();
    pcscene = new PCScene(this);
    setScene(pcscene);
   // pcscene->setSceneRect(0,0,viewport()->geometry().width()-10,viewport()->geometry().height()-10);
    parentNodes.clear();
    childNodes.clear();
    nodesList.clear();
    generation=1;

    centernode = new Node(Hostname, state,this);
       centernode->setPos(0, 0);
       centernode->setGeneration(0);
       pcscene->addItem(centernode);
       nodesList.append(centernode);
}

void PCDialog::addParent(QString ParentName)
{
    int y=-100*generation;
    Node* parentnode = new Node(ParentName,127, this);
    parentnode->setGeneration(generation);
        parentnode->setPos(0,y);
        pcscene->addItem(parentnode);
    pcscene->addItem(new Edge(parentnode,centernode));

    parentNodes.append(parentnode);
    nodesList.append(parentnode);

    //Lets move the first Parents, if more than one
    if (parentNodes.size()>1)
    {
        for (int i=0;i<parentNodes.size();i++)
        {
            //sizeDiff is the distance between two Nodes
            int sizeDiff=(250*(parentNodes.size()-1))/(parentNodes.size());
            int x=(((sizeDiff*(parentNodes.size()-1))/2)-(sizeDiff*(i)));
//            Node* tmp=(Node*) parentNodes.at(i);
                qDebug() << i << "sizeDiff" << sizeDiff << x ;

            parentNodes.at(i)->setPos(x,y);
        }

    }
}

/*
bool PCDialog::nodeExistsFromNodename(QString nodeName)
{
    bool exist=false;
//    Node* nodeNode;
    for (int i=0;i<nodesList.size();i++)
    {
        if ( nodeName == nodesList.at(i)->getHostname() )
        {
  //          nodeNode=nodesList.at(i);
            exist=true;
        }
    }
    return exist;
}
*/
Node* PCDialog::nodeFromNodename(QString nodeName)
{
    bool exist=false;
    Node* nodeNode;
    for (int i=0;i<nodesList.size();i++)
    {
        if ( nodeName == nodesList.at(i)->getHostname() )
        {
            nodeNode=nodesList.at(i);
            exist=true;
        }
    }
    if (exist)
    {
        return nodeNode;
    }
    else
    {
       return 0;
    }
}

void PCDialog::addNode(QString SourceName, QString DestinationName, int state, QString Type)
{
    if (SourceName.trimmed().isEmpty())
    return;
if (DestinationName.trimmed().isEmpty())
    return;

    int y;
    if (Type == "child")
    {
        y=100*generation;
    }
    else
    {
        y=-100*generation;
    }

    qDebug()<< "Add Node" << SourceName << DestinationName << state << Type << y;
    // Lets see, if Source exists already
    bool exist=false;
    Node* sourceNode;
    //get the propper destination
    Node* destinationNode;
    for (int i=0;i<nodesList.size();i++)
    {
        if ( SourceName == nodesList.at(i)->getHostname() )
        {
            sourceNode=nodesList.at(i);
            exist=true;
        }
    }

        if (!exist)
        {
             sourceNode = new Node(SourceName, state,this);
             if(Type == "parent")
             {
                 sourceNode->setGeneration(generation);
             }
             else if(Type == "child")
             {
                 sourceNode->setGeneration(generation*-1);
             }
               //  sourceNode->setPos(0,y);
                   sourceNode->setPos(0,y);
                 pcscene->addItem(sourceNode);
                 nodesList.append(sourceNode);
        }


        for (int i=0;i<nodesList.size();i++)
        {
            if ( DestinationName == nodesList.at(i)->getHostname() )
            {
                destinationNode=nodesList.at(i);
            }
        }
        //set Color
        destinationNode->setState(state);

        if (Type == "parent")
        {
            pcscene->addItem(new Edge(sourceNode,destinationNode));
        }
        else
        {
            pcscene->addItem(new Edge(destinationNode,sourceNode));
        }

        //Lets move the first Parents, if more than one
        if (Type=="parent")
        {
            if (nodesList.size()>1)
            {
                parentNodes.clear();
                QList<Node*> currentParents;
                for (int i=0;i<nodesList.size();i++)
                {
                    // only current generation (and positiv so its a parent)
                    if (nodesList.at(i)->getGeneration() == generation)
                    {
                        qDebug() << "currentParents.append(nodesList.at(i))" << nodesList.at(i)->getHostname() <<nodesList.at(i)->getGeneration();
                        currentParents.append(nodesList.at(i));
                        //Next Generation of parents
                        parentNodes.append(nodesList.at(i));
                    }
                }

                for (int i=0;i<currentParents.size();i++)
                {
                    //sizeDiff is the distance between two Nodes
                    int sizeDiff=(250*(currentParents.size()-1))/(currentParents.size());
                    int x=(((sizeDiff*(currentParents.size()-1))/2)-(sizeDiff*(i)));
        //            Node* tmp=(Node*) parentNodes.at(i);
                        //qDebug() << i << "sizeDiff" << sizeDiff << x ;

                    currentParents.at(i)->setPos(x,y);
                }

            }
        }
        else if (Type == "child")
        {
            if (nodesList.size()>1)
            {
                childNodes.clear();
                QList<Node*> currentChilds;
                for (int i=0;i<nodesList.size();i++)
                {
                    // only current generation
                    if (nodesList.at(i)->getGeneration() == generation*-1)
                    {
                        currentChilds.append(nodesList.at(i));
                        //Next Generation of parents
                        childNodes.append(nodesList.at(i));
                    }
                }

                for (int i=0;i<currentChilds.size();i++)
                {
                    //sizeDiff is the distance between two Nodes
                    int sizeDiff=(250*(currentChilds.size()-1))/(currentChilds.size());
                    int x=(((sizeDiff*(currentChilds.size()-1))/2)-(sizeDiff*(i)));
        //            Node* tmp=(Node*) parentNodes.at(i);
                        //qDebug() << i << "sizeDiff" << sizeDiff << x ;

                    currentChilds.at(i)->setPos(x,y);
                }

            }
        }
       // autoScale();
}

void PCDialog::addChild(QString ChildName)
{

    int y=100;
    Node* childnode = new Node(ChildName, 127,this);
        childnode->setPos(0,y);
        childnode->setGeneration(generation*-1);
        pcscene->addItem(childnode);
    pcscene->addItem(new Edge(centernode, childnode));

    childNodes.append(childnode);
    nodesList.append(childnode);

    //Lets move the first Parents, if more than one
    if (childNodes.size()>1)
    {
        for (int i=0;i<childNodes.size();i++)
        {
            //sizeDiff is the distance between two Nodes
            int sizeDiff=(250*(childNodes.size()-1))/(childNodes.size());
            int x=(((sizeDiff*(childNodes.size()-1))/2)-(sizeDiff*(i)));
         //   qDebug() << i << "sizeDiff" << sizeDiff;
            childNodes.at(i)->setPos(x,y);
        }

    }
}


//Think it over
void PCDialog::getParents()
{
    generation++;
}

void PCDialog::mouseDoubleClickEvent(QMouseEvent * mouseEvent)
{

    int sx=mapToScene(mouseEvent->pos()).x();
    int sy=mapToScene(mouseEvent->pos()).y();

    if (pcscene->items(QPointF(sx,sy)).count() == 1)
    {
        if ((pcscene->items(QPointF(sx,sy)).last())->type() == 22222)
        {
            Node * item = dynamic_cast<Node*>(pcscene->items(QPointF(sx,sy)).last());
            qDebug() << (pcscene->items(QPointF(sx,sy)).last())->type();
            qDebug() << item->getHostname();
            if (!item->getHostname().isEmpty())
            {
                emit doubleClickedHost(item->getHostname());
            }
        }
    }
    //mouseEvent->accept();
}


void PCDialog::wheelEvent(QWheelEvent* event)
{

    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    // Scale the view / do the zoom
    double scaleFactor = 1.15;
    if(event->delta() > 0) {
        // Zoom in
        scale(scaleFactor, scaleFactor);
    } else {
        // Zooming out
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }

    // Don't call superclass handler here
    // as wheel is normally used for moving scrollbars
}

void PCDialog::itemMoved()
{
    if (!timerId)
        timerId = startTimer(500 / 25);
}
//! [2]

void PCDialog::force()
{
    qDebug()<< "forcing" << forcing;
    if (forcing==false)
        forcing=true;
    else
        forcing=false;

    itemMoved();

}

void PCDialog::timerEvent(QTimerEvent *event)
{
    if (forcing == false) return;

    Q_UNUSED(event);

    QList<Node *> nodes;
    foreach (QGraphicsItem *item, scene()->items()) {
        if (Node *node = qgraphicsitem_cast<Node *>(item))
            nodes << node;
    }

    foreach (Node *node, nodes)
        node->calculateForces();

    bool itemsMoved = false;
    foreach (Node *node, nodes) {
        if (node->advance())
            itemsMoved = true;
    }

    if (!itemsMoved) {
        killTimer(timerId);
        timerId = 0;
    }
}

QStringList PCDialog::getParentStringList()
{
    QStringList tmpList;
    for (int i=0;i<parentNodes.size();i++)
    {
        tmpList << qgraphicsitem_cast<Node *>(parentNodes.at(i))->getHostname();
    }
    return tmpList;
    qDebug() << "currentParentStringList" << tmpList;
}


QStringList PCDialog::getCurrentGenerationStringList()
{
    QStringList tmpList;
    for (int i=0;i<nodesList.size();i++)
    {
        if ( (nodesList.at(i)->getGeneration() == generation) || (nodesList.at(i)->getGeneration() == (generation*-1)) )
        {
            tmpList << (nodesList.at(i))->getHostname();
        }
    }
    tmpList.sort();
    return tmpList;
    qDebug() << "currentGenerationStringList" << tmpList;
}

void PCDialog::autoScale()
{
    QRectF iRect=scene()->itemsBoundingRect();
    QRectF sRect = QPolygonF(mapToScene( viewport()->geometry())).boundingRect();
 /*     qDebug() << "irect" << iRect << sRect;
    qDebug() << "width" <<iRect.width()<<sRect.width();
    qDebug() << "MapToScene" << QPolygonF(mapToScene( viewport()->geometry())).boundingRect() ;
*/
    qreal wfactor=sRect.width()/iRect.width();
    wfactor=wfactor*0.85;
    qreal hfactor=sRect.height()/iRect.height();
    hfactor=hfactor*0.85;


    qDebug() << "wfactor" << wfactor << hfactor;

    if (wfactor < hfactor)
    {
        scale(wfactor,wfactor);
    }
    else
    {
        scale (hfactor,hfactor);
    }
    //if (sRect.height()/iRect.height() >1)
      //      scale(1,sRect.height()/iRect.height());
}
