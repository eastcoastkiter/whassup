#include "generationtree.h"

GenerationTreeWidget::GenerationTreeWidget(QWidget* parent) : QWidget(parent)
{
    uid=55555;
    QGridLayout *grid= new QGridLayout(this);
    generationTree = new QTreeWidget(this);

    QStringList headerLabelsGeneration;
      generationTree->setColumnCount(5);
      generationTree->setContextMenuPolicy(Qt::CustomContextMenu);
      generationTree->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
      headerLabelsGeneration  << "host_name" << "parents" << "childs" << "childcount" << "overallstate";
      generationTree->setHeaderLabels(headerLabelsGeneration);
      generationTree->header()->setVisible(false);
      generationTree->setSortingEnabled (false);
      generationTree->hideColumn(1);
      generationTree->hideColumn(2);
      generationTree->hideColumn(3);
      generationTree->hideColumn(4);

      connect(generationTree, SIGNAL( itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(generationTreeItemDoubleClicked(QTreeWidgetItem*, int)));

      grid->addWidget(generationTree,0,0,2,1);

    QHBoxLayout* logGrid = new QHBoxLayout(this);
        logGrid->addWidget(new QLabel("Host:"));

        searchLogHost = new QLineEdit(this);
            connect( searchLogHost, SIGNAL( returnPressed()) , this, SLOT( searchSlot() ) );
            logGrid->addWidget(searchLogHost);



                //searchLogHostCompleter->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
           // searchLogHost->setCompleter(searchLogHostCompleter);



        QPushButton* searchLogHostButton = new QPushButton("Search", this);
            connect( searchLogHostButton, SIGNAL( pressed()) , this, SLOT( searchSlot() ) );
            logGrid->addWidget(searchLogHostButton);

           // logGrid->addItem(new QSpacerItem ( 100, 0, QSizePolicy::Expanding, QSizePolicy::Fixed ));
      grid->addLayout(logGrid,2,0);

      QGridLayout *topButtonGrid = new QGridLayout(this);

      QPushButton* getParentsButton = new QPushButton("Next Generation", this);
         connect( getParentsButton, SIGNAL( pressed()) , this, SLOT( requestParents() ) );
      //mainGrid->addWidget(getParentsButton,r,5);
         topButtonGrid->addWidget(getParentsButton,0,0);

      pcdialog = new PCDialog("",127,this);
     // pcdialog->setWindowFlags(Qt::Dialog);
        pcdialog->setMinimumWidth(550);
        connect( pcdialog, SIGNAL(doubleClickedHost(QString)) , this, SLOT( doubleClickedHost(QString) ) );
  //      pcdialog->show();
      QPushButton* scaleButton = new QPushButton("Scale", this);
        connect( scaleButton, SIGNAL( pressed()) , pcdialog, SLOT( autoScale() ) );
      topButtonGrid->addWidget(scaleButton,0,1);

      topButtonGrid->addItem(new QSpacerItem ( 100, 10, QSizePolicy::Expanding, QSizePolicy::Fixed ),0,2);

      grid->addLayout(topButtonGrid,2,1);
      grid->addWidget(pcdialog,0,1,2,1);
}

void GenerationTreeWidget::gotGenerationData(QJsonArray jsonArray)
{
    generationTree->clear();
    generationTree->update();
    //qDebug() << "gotGenerationData" << jsonArray;
    for (int i=0;i<jsonArray.size();i++)
    {
        QJsonArray hostArray=jsonArray.at(i).toArray();
        QString hname=hostArray.at(0).toString();
        QJsonArray parentArray = hostArray.at(1).toArray();
        QStringList parentList;
        for (int p=0;p<parentArray.size();p++)
        {
            parentList << parentArray.at(p).toString();
        }
        QJsonArray childArray = hostArray.at(2).toArray();
        QStringList childList;
        for (int p=0;p<childArray.size();p++)
        {
            childList << childArray.at(p).toString();
        }

        //qDebug() << "gotGenerationData" << hname << "parents" << parentArray << parentList;
        int overallState=jsonArray.at(3).toDouble()+jsonArray.at(4).toDouble();
        GenerationHostItem *generationHostItem = new GenerationHostItem(hname,parentList,childList,overallState);
        generationHostItemList.append(generationHostItem);

    }

    //Lets add host without parents //toplevelitems
    for (int i=0;i<generationHostItemList.size();i++)
    {
        if (generationHostItemList.at(i)->getParentCount() == 0)
        {
            QString tmpHost = generationHostItemList.at(i)->getHostname();
            QTreeWidgetItem *Item = new QTreeWidgetItem(QStringList() << tmpHost  ,QTreeWidgetItem::Type);
            generationTree->insertTopLevelItem(generationTree->topLevelItemCount(),Item);
            Item->setText(2,generationHostItemList.at(i)->getChildString() );
            Item->setText(4,QString("%1").arg(generationHostItemList.at(i)->getOverallstate()));
            generationHostItemList.takeAt(i);
            i--;
        }
    }

    //lets got through the rest
    int z=0;
    while (generationHostItemList.size() > 0)
    {

        //go throu all left hosts from json
        for (int a=0;a<generationHostItemList.size();a++)
        {
            GenerationHostItem* gItem=generationHostItemList.at(a);
           //QString tmpHost = generationHostItemList.at(a)->getHostname();
            QString tmpHost = gItem->getHostname();
           //QStringList tmpParentList=generationHostItemList.at(a)->getParents();
            QStringList tmpParentList=gItem->getParents();

           //go throu the parents of the current item
            for (int p=0;p<tmpParentList.size();p++)
            {
               QString parent=tmpParentList.at(p);
               //qDebug() << tmpHost << "Parent" <<parent<< "z" << z;
               QList<QTreeWidgetItem *> found = generationTree->findItems(parent,Qt::MatchExactly| Qt::MatchRecursive,0);
               for (int f=0;f<found.size();f++)
               {
                   //qDebug() << "z" << z << "found parent for" << tmpHost << "Parent" <<parent << "pcount" << tmpParentList;
                   QTreeWidgetItem *Item = new QTreeWidgetItem(QStringList() << tmpHost ,QTreeWidgetItem::Type);
                   found.at(f)->addChild(Item);
                   Item->setText(1,gItem->getParentString() );
                   Item->setText(2,gItem->getChildString() );
                   Item->setText(4,QString("%1").arg(gItem->getOverallstate()));
                   gItem->removeParent(parent);
               }

            }
            if (generationHostItemList.at(a)->getParentCount() == 0)
            {
               // qDebug() << tmpHost << "has no more Parents" << "there are hostitems left" << generationHostItemList.size();
                generationHostItemList.takeAt(a);
                a--;
            }
   /*         else
            {
                qDebug() << tmpHost << "has Parent" <<generationHostItemList.at(a)->getParentCount();
            }
*/
        }
        qDebug() <<"z"<< z;
        z++;
        generationTree->update();
        if (z>10)
            break;
    }

    //set Child Cound
    QTreeWidgetItemIterator it(generationTree);
    //set Child Counter and Completer
    QStringList completerList;
    while (*it)
    {
       // (*it)->setText(3,QString("%1").arg((*it)->childCount()));
        completerList << (*it)->text(0);
        (*it)->setData(3,Qt::DisplayRole, (*it)->childCount());
        ++it;
    }
    generationTree->sortByColumn(3);
    completerList.removeDuplicates();
    completerList.sort(Qt::CaseInsensitive);
    QCompleter* searchLogHostCompleter = new QCompleter(completerList, this);
        searchLogHostCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    searchLogHost->setCompleter(searchLogHostCompleter);
}

void GenerationTreeWidget::generationTreeItemDoubleClicked(QTreeWidgetItem* item, int col)
{
    (void) col;
    pcdialog->setCenterNode(item->text(0),item->text(4).toInt());
    QStringList parents=item->text(1).trimmed().split(" ");
    for (int i=0;i<parents.size();i++)
    {
        if (!parents.at(i).isEmpty())
        pcdialog->addParent(parents.at(i));
    }
    QStringList childs=item->text(2).trimmed().split(" ");
    for (int i=0;i<childs.size();i++)
    {
        if (!childs.at(i).isEmpty())
        pcdialog->addChild(childs.at(i));
    }
}

void GenerationTreeWidget::requestParents()
{

    LiveSocketRequest ls;
    ls.setTable("hosts");
    ls.addColumns(QStringList() << "host_name" << "state" << "worst_service_state" << "parents" << "childs");

    QStringList currentGenerationNodes=pcdialog->getCurrentGenerationStringList();
    qDebug() << "request gewneration" << currentGenerationNodes;
    // There are no more child or parents
    if (currentGenerationNodes.size() == 0)
    {
        return;
    }

    for (int i=0;i<currentGenerationNodes.size();i++)
    {
        ls.addFilter("host_name = " + currentGenerationNodes.at(i));
    }
    if (currentGenerationNodes.size()>1)
    {
        ls.addRawText(QString("Or: %1\n").arg(currentGenerationNodes.size()));
    }
    ls.setOutputFormat("json");
    emit requestLiveStatusData(uid,"json",ls.getLiveStatusRequest());

}

void GenerationTreeWidget::gotLiveStatusJson(QJsonArray jsonUpperArray)
{
    qDebug() << "generation  data received";
    //We add new generation, so ++
    pcdialog->setGeneration();


    for (int k = 0; k < jsonUpperArray.size(); ++k)
    {
        QJsonArray jsonArray=jsonUpperArray.at(k).toArray();
    qDebug() << "gotLiveStatusJson" << jsonArray;
        QString host = jsonArray.at(0).toString();
        if (host.trimmed().isEmpty())
        {
            continue;
        }
        int state=jsonArray.at(1).toDouble() + jsonArray.at(2).toDouble();
        QStringList parents;
        for (int i=0;i<jsonArray.at(3).toArray().size();i++)
        {
            parents << jsonArray.at(3).toArray().at(i).toString();
        }
        parents.sort();

        QStringList childs;
        for (int i=0;i<jsonArray.at(4).toArray().size();i++)
        {
            childs << jsonArray.at(4).toArray().at(i).toString();
        }
        childs.sort();

        //Lets find out, if host is child or parent
        Node* node=pcdialog->nodeFromNodename(host);
        qDebug() << "Recieved for HostInfo" << host << parents << childs;

            if (node->getGeneration()>0)
            {
                //it is a parent request
               //hostname always exists, could be parent, could be child
                for (int i=0;i<parents.size();i++)
                {
                   // if (!parents.at(i).trimmed().isEmpty())
                        pcdialog->addNode(parents.at(i), host, 127, "parent");
                }
            }
            else
            {
                //its a cild request
                for (int i=0;i<childs.size();i++)
                {
                   if (!childs.at(i).trimmed().isEmpty())
                        pcdialog->addNode(childs.at(i),host,127, "child");
                }
            }
            node->setState(state);

    }


}

void GenerationTreeWidget::searchSlot()
{
    QList<QTreeWidgetItem *> found = generationTree->findItems(searchLogHost->text(),Qt::MatchExactly| Qt::MatchRecursive,0);
    generationTree->setCurrentItem(found.first());
    generationTreeItemDoubleClicked(found.first(), 0);
}

void GenerationTreeWidget::doubleClickedHost(QString host)
{
    searchLogHost->setText(host);
    searchSlot();
}
