#include "filterwidget.h"
using namespace std;

FilterWidget::FilterWidget(QString inputFilterText,QStringList configServiceList,QStringList configHostList, QStringList configHostgroupList, QWidget * parent, Qt::WindowFlags f): QWidget( parent, f )
{
     resize(480,640);
     setWindowTitle("Filter");
     QGridLayout* mainGrid = new QGridLayout(this);

     mainGrid->addWidget(new QLabel("Services:"),0,0);
     serviceBox=new QListWidget(this);
        serviceBox->addItems(configServiceList);
        serviceBox->setSelectionMode(QAbstractItemView::MultiSelection);
        connect( serviceBox, SIGNAL( itemSelectionChanged ()) , this, SLOT(  boxItemSelectionChanged () ));
        mainGrid->addWidget(serviceBox,1,0);


    mainGrid->addWidget(new QLabel("Hosts:"),2,0);
    hostBox=new QListWidget(this);
       hostBox->addItems(configHostList);
       hostBox->setSelectionMode(QAbstractItemView::MultiSelection);
       connect( hostBox, SIGNAL( itemSelectionChanged ()) , this, SLOT(  boxItemSelectionChanged () ));
       mainGrid->addWidget(hostBox,3,0);


   mainGrid->addWidget(new QLabel("Hostgroups:"),4,0);
   hostgroupBox=new QListWidget(this);
      hostgroupBox->addItems(configHostgroupList);
      hostgroupBox->setSelectionMode(QAbstractItemView::MultiSelection);
      connect( hostgroupBox, SIGNAL( itemSelectionChanged ()) , this, SLOT(  boxItemSelectionChanged () ));
      mainGrid->addWidget(hostgroupBox,5,0);

      mainGrid->addWidget(new QLabel("Conjunction:"),6,0);
      conjunctionBox=new QListWidget(this);
      conjunctionBox->addItems(QStringList() << "And" << "Or");
         connect( conjunctionBox, SIGNAL( itemSelectionChanged ()) , this, SLOT(  boxItemSelectionChanged () ));
         mainGrid->addWidget(conjunctionBox,7,0);

         mainGrid->addWidget(new QLabel("Regular Expression:"),8,0);

         QGridLayout *radioGrid = new QGridLayout(this);
         serviceRadio=new QRadioButton("Service",this);
            serviceRadio->setChecked(true);
            radioGrid->addWidget(serviceRadio,0,0);

         hostRadio=new QRadioButton("Host",this);
            radioGrid->addWidget(hostRadio,0,1);
         mainGrid->addLayout(radioGrid,9,0);

         regLine=new QLineEdit(this);
            connect( regLine, SIGNAL( textChanged (QString)) , this, SLOT(  boxItemSelectionChanged () ));
            mainGrid->addWidget(regLine,10,0);

      QGridLayout* buttonGrid = new QGridLayout(this);
        buttonGrid->addItem(new QSpacerItem ( 10, 10, QSizePolicy::Expanding, QSizePolicy::Expanding ),0,0);
          includeButton = new QPushButton("&>> include", this);
             connect( includeButton, SIGNAL( pressed ()) , this, SLOT(  include() ));
             buttonGrid->addWidget(includeButton,1,0);

          excludeButton = new QPushButton("&>> exclude", this);
             connect( excludeButton, SIGNAL( pressed ()) , this, SLOT(  exclude() ));
             buttonGrid->addWidget(excludeButton,2,0);

          removeButton = new QPushButton("&<<", this);
             connect( removeButton, SIGNAL( pressed ()) , this, SLOT(  remove() ));
             buttonGrid->addWidget(removeButton,3,0);

        buttonGrid->addItem(new QSpacerItem ( 10, 10, QSizePolicy::Expanding, QSizePolicy::Expanding ),4,0);
     mainGrid->addLayout(buttonGrid,3,1);

     mainGrid->addWidget(new QLabel("Filter:"),0,2);
     filterBox=new QListWidget(this);
        filterBox->setSelectionMode(QAbstractItemView::MultiSelection);
        connect( filterBox, SIGNAL( itemSelectionChanged ()) , this, SLOT(  boxItemSelectionChanged () ));
        //filterBox->setGridSize(QSize(300,500));
        mainGrid->addWidget(filterBox,1,2,8,1);

     QPushButton* hideButton = new QPushButton("&Apply Filter", this);
        mainGrid->addWidget(hideButton,10,2);
        connect( hideButton, SIGNAL( pressed ()) , this, SLOT(  end() ));

    mainGrid->addItem(new QSpacerItem ( 10, 10, QSizePolicy::Expanding, QSizePolicy::Expanding ),11,11);

    if (!inputFilterText.trimmed().isEmpty())
    {
        QStringList FilterSplit=inputFilterText.split(',');
        filterBox->addItems(FilterSplit);
    }
}

FilterWidget::~FilterWidget()
{
}

void FilterWidget::end()
{
    QString filter=QString();
    if (filterBox->count() > 0)
    {
        filter=filterBox->item(0)->text();
        for (int i=1;i<filterBox->count();i++)
        {
            filter=filter + "," + filterBox->item(i)->text();
        }
    }
    else
    {
        filter=" ";
    }
    emit gotNewFilter(filter);
}

void FilterWidget::boxItemSelectionChanged ()
{

    if (sender()!=serviceBox)
    {
        serviceBox->clearSelection();
    }
    else
    {
        includeButton->setEnabled(true);
        excludeButton->setEnabled(true);
        removeButton->setEnabled(false);
        regLine->clear();
    }

    if (sender()!=hostBox)
    {
        hostBox->clearSelection();
    }
    else
    {
        includeButton->setEnabled(true);
        excludeButton->setEnabled(true);
        removeButton->setEnabled(false);
        regLine->clear();
    }

    if (sender()!=hostgroupBox)
    {
        hostgroupBox->clearSelection();
    }
    else
    {
        includeButton->setEnabled(true);
        excludeButton->setEnabled(true);
        removeButton->setEnabled(false);
        regLine->clear();
    }

    if (sender()!=conjunctionBox)
    {
        conjunctionBox->clearSelection();
    }
    else
    {
        if (filterBox->count() > 1)
        {
            //And|Or just if 2 Items above
            int conjuntionCounter=0;
            for (int j=filterBox->count()-1;j>-1;j--)
            {
                if (!( (filterBox->item(j)->text().startsWith("And")) || (filterBox->item(j)->text().startsWith("Or")) ) )
                {
                    conjuntionCounter++;
                }
                else
                {
                    break;
                }
            }
            qDebug() << "conjuntionCounter" << conjuntionCounter;
            if (conjuntionCounter>1)
            {
                includeButton->setEnabled(true);
            }
            else
            {
                includeButton->setEnabled(false);
            }
        }
        else
        {
            includeButton->setEnabled(false);
        }
        excludeButton->setEnabled(false);
        removeButton->setEnabled(false);
        regLine->clear();
    }

    if (sender()!=regLine)
    {
        regLine->clear();
    }
    else
    {
        includeButton->setEnabled(true);
        excludeButton->setEnabled(true);
        removeButton->setEnabled(false);
    }

    if (sender()!=filterBox)
    {
        filterBox->clearSelection();
    }
    else
    {
        includeButton->setEnabled(false);
        excludeButton->setEnabled(false);
        removeButton->setEnabled(true);
        regLine->clear();
    }
}

void FilterWidget::include()
{
        for (int i=0;i<serviceBox->selectedItems().size();i++)
        {
            filterBox->addItem(QString("description = " + serviceBox->selectedItems().at(i)->text()));
        }
        for (int i=0;i<hostBox->selectedItems().size();i++)
        {
            filterBox->addItem(QString("host_name = " + hostBox->selectedItems().at(i)->text()));
        }
        for (int i=0;i<hostgroupBox->selectedItems().size();i++)
        {
            filterBox->addItem(QString("host_groups >= " + hostgroupBox->selectedItems().at(i)->text()));
        }
        for (int i=0;i<conjunctionBox->selectedItems().size();i++)
        {
            int conjuntionCounter=0;
            for (int j=filterBox->count()-1;j>-1;j--)
            {
                if (!( (filterBox->item(j)->text().startsWith("And")) || (filterBox->item(j)->text().startsWith("Or")) ) )
                {
                    conjuntionCounter++;
                }
                else
                {
                    break;
                }

            }
            filterBox->addItem(QString(conjunctionBox->selectedItems().at(i)->text()) + ": " + QString("%1").arg(conjuntionCounter));
        }
        if (!regLine->text().isEmpty())
        {
            if (serviceRadio->isChecked())
            {
                filterBox->addItem("description ~~ " + regLine->text());
            }
            else if (hostRadio->isChecked())
            {
                filterBox->addItem("host_name ~~ " + regLine->text());
            }
        }
    regLine->clear();
    boxItemSelectionChanged ();
}

void FilterWidget::exclude()
{
    for (int i=0;i<serviceBox->selectedItems().size();i++)
    {
        filterBox->addItem(QString("description != " + serviceBox->selectedItems().at(i)->text()));
    }
    for (int i=0;i<hostBox->selectedItems().size();i++)
    {
        filterBox->addItem(QString("host_name !~ " + hostBox->selectedItems().at(i)->text()));
    }
    for (int i=0;i<hostgroupBox->selectedItems().size();i++)
    {
        filterBox->addItem(QString("host_groups !>= " + hostgroupBox->selectedItems().at(i)->text()));
    }
    if (!regLine->text().isEmpty())
    {
        if (serviceRadio->isChecked())
        {
            filterBox->addItem("description !~~ " + regLine->text());
        }
        else if (hostRadio->isChecked())
        {
            filterBox->addItem("host_name !~~ " + regLine->text());
        }
    }

    regLine->clear();
    boxItemSelectionChanged ();
}

void FilterWidget::remove()
{
/*    for (int i=0;i<filterBox->selectedItems().size();i++)
    {
        delete filterBox->selectedItems().at(i);
    }
*/
    for (int i=0;i<filterBox->count();i++)
    {
        if (filterBox->item(i)->isSelected())
        {
            delete filterBox->item(i);
            i--;
        }
    }

    regLine->clear();
    boxItemSelectionChanged ();
}

