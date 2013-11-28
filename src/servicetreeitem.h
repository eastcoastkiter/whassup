#ifndef SERVICETREEITEM_H
#define SERVICETREEITEM_H

#include <QApplication>
#include <QTreeWidget>
#include <QTreeWidgetItem>

class ServiceTreeItem : public QTreeWidgetItem
{
public:
   ServiceTreeItem(QTreeWidget *tree) : QTreeWidgetItem(tree)  {}
   ServiceTreeItem(QTreeWidgetItem * parent, int type = Type) : QTreeWidgetItem(parent,type){}
   ServiceTreeItem(const QStringList & strings, int type = Type) : QTreeWidgetItem(strings,type){}
   ServiceTreeItem(QTreeWidget * parent, const QStringList & strings)
                   : QTreeWidgetItem (parent,strings)  {}
    bool operator< (const QTreeWidgetItem &other) const
    {

        int sortCol = treeWidget()->sortColumn();
        // Sort last_changes a bit differnt
        //qDebug() << "sortcol" << sortCol;
        if ((sortCol == 1) && (qApp->property("ClientSortServiceByState").toBool()))
        {
                //sort service by state
            int myNumber = text(2).toInt();
            int otherNumber = other.text(2).toInt();
            //show unknoeen etc at the end
            if (myNumber < 3)
                return myNumber < otherNumber;
            else
                return otherNumber < myNumber;
        }
        else if (sortCol == 3)
        {
            // sort last_chnage by epoch in 20
            int myNumber = text(20).toInt();
            int otherNumber = other.text(20).toInt();
            return myNumber < otherNumber;
        }
        else
        {
            return text(sortCol).toLower() < other.text(sortCol).toLower();
        }

    }
};

#endif // SERVICETREEITEM_H
