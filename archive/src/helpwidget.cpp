#include "helpwidget.h"
using namespace std;

HelpWidget::HelpWidget(QWidget * parent, Qt::WindowFlags f): QWidget( parent, f )
{
     resize(300,200);
     setWindowTitle("Help");
     QGridLayout* mainGrid = new QGridLayout(this);

     QList<QPixmap> pixmaps;
     pixmaps << QPixmap(ok_xpm) << QPixmap(warning_xpm) << QPixmap(critical_xpm);
     pixmaps << QPixmap(warning_ack_xpm) << QPixmap(critical_ack_xpm);
     pixmaps << QPixmap(pacman_xpm);
     pixmaps << QPixmap(soft_xpm) << QPixmap(ndisabled_xpm) << QPixmap(downtime_xpm);

     QStringList Labels;
     Labels << "Service is Ok" << "Service is Warning" << "Service is Critical";
     Labels << "Service is Warning, but Achnowledged" << "Service is Critical, but Achnowledged";
     Labels << "Acknowledge this";
     Labels << "Service is in Soft state" << "Notifications are disabled" << "Service is in Downtime";

     for (int i=0; i<pixmaps.size();i++)
     {
         QLabel* L = new QLabel();
             L->setPixmap ( pixmaps.at(i) );
         mainGrid->addWidget(L,i,0);
         mainGrid->addWidget(new QLabel(Labels.at(i), this),i,1);
     }

     QPushButton* hideButton = new QPushButton("&Close", this);
        mainGrid->addWidget(hideButton,9,3);
        connect( hideButton, SIGNAL( pressed ()) , this, SLOT(  close() ));

    mainGrid->addItem(new QSpacerItem ( 10, 10, QSizePolicy::Expanding, QSizePolicy::Expanding ),10,10);


}

HelpWidget::~HelpWidget()
{
}

