#include "helpwidget.h"
using namespace std;

HelpWidget::HelpWidget(QWidget * parent, Qt::WindowFlags f): QWidget( parent, f )
{
    resize(800,480);
    setWindowTitle("Help");
    QGridLayout* mainGrid = new QGridLayout(this);

    helpTree = new QTreeWidget(this);
        helpTree->setColumnCount(4);
        helpTree->hideColumn(1);
        helpTree->hideColumn(2);
        helpTree->hideColumn(3);
        helpTree->header()->close();
        helpTree->setMinimumWidth(150);
        helpTree->setMaximumWidth(150);
        connect(helpTree,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(helpTreeDoubleClicked(QModelIndex)));
    mainGrid->addWidget(helpTree,0,0);

    webview = new QTextBrowser(this);

    mainGrid->addWidget(webview,0,1);


    showHtml(":/helpfiles/Welcome.html");
    webview->show();

    addHelpFiles();
}

HelpWidget::~HelpWidget()
{
}

void HelpWidget::addHelpFiles()
{
    QList<QTreeWidgetItem *> items;
    foreach( const QString &filename, QDir(":/helpfiles").entryList() )
    {
        qDebug() << "file" << filename;
        if (filename.trimmed().endsWith(".html"))
        {
            //search for Parents
          /*<!--Title: FilterWidget-->
            <!--Tag: Filter-->
            <!--Tag: Config-->
            <!--Parent: Config-->
            <!--EndOfHead-->*/
            QFile testFile(":/helpfiles/" + filename);
            qDebug() << "Parse file" << filename;
            if (testFile.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                QString Title;
                QString Parent;
                QString Sort;
                qDebug() << "Opened file" << filename;
                while (!testFile.atEnd())
                {
                    QString line = testFile.readLine();
                    if (line.startsWith("<!--Title:"))
                    {
                        Title=QString(line.split(":")[1]).remove("-->").trimmed();
                    }
                    else if (line.startsWith("<!--Parent:"))
                    {
                        Parent=QString(line.split(":")[1]).remove("-->").trimmed();
                        //Search, if Parent Exist
                    }
                    else if (line.startsWith("<!--Sort:"))
                    {
                        Sort=QString(line.split(":")[1]).remove("-->").trimmed();
                    }
                    else if (line.startsWith("<!--EndOfHead"))
                    {
                        //stop reading header
                        continue;
                    }
                }

                QTreeWidgetItem * Item = new QTreeWidgetItem(QStringList() << Title << Parent << ":/helpfiles/" +filename << Sort,QTreeWidgetItem::Type);
                QList<QTreeWidgetItem *> parentList = helpTree->findItems(Parent,Qt::MatchExactly|Qt::MatchRecursive,0);

                if (Parent.isEmpty())
                {
                    // no Parents, ok you are a topLevelItem like Welcome
                    items.append(Item);
                }
                else
                {
                    if (parentList.size()>0)
                    {
                        // we found a Parent, add item
                        parentList.first()->addChild(Item);
                    }
                    else
                    {
                        //create tree parent, if not exists
                        qDebug() << "Find parent" << Parent;
                        QTreeWidgetItem * Par = new QTreeWidgetItem(QStringList() << Parent << "" << "" << Sort,QTreeWidgetItem::Type);
                        items.append(Par);
                        //and add the Item to it
                        Par->addChild(Item);
                    }
                }
            }
        }
        helpTree->addTopLevelItems(items);
    }
    helpTree->sortByColumn(3,Qt::AscendingOrder);
}

void HelpWidget::helpTreeDoubleClicked(QModelIndex model)
{
    (void) model;
    if (!helpTree->currentItem()->text(2).isEmpty())
    {
        showHtml(helpTree->currentItem()->text(2));
    }
}

void HelpWidget::showHtml(QString filename)
{
    QFile File(filename);
    QByteArray html;
    if (File.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        while (!File.atEnd())
        {
            QByteArray line = File.readLine();
            if (!line.startsWith("<!--"))
            {
                html.append(line);
            }
        }
    }
    else
    {
        qDebug() << "Could not open file" <<filename;
    }
    webview->setText(html);
}
