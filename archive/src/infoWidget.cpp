#include "infoWidget.h"
using namespace std;

InfoWidget::InfoWidget(QString message, QWidget * parent, Qt::WindowFlags f): QWidget( parent, f )
{
   Parent=parent;
   timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(show()));
     resize(100,50);
  move (Parent->rect().center());
         setWindowTitle(message);
     QGridLayout* mainGrid = new QGridLayout(this);

        messageLabel = new QLabel(message, this);
qDebug()<< "make a iWidget";

/*
     QPushButton* hideButton = new QPushButton("&Close", this);
        mainGrid->addWidget(hideButton,9,3);
        connect( hideButton, SIGNAL( pressed ()) , this, SLOT(  close() ));
*/
    mainGrid->addItem(new QSpacerItem ( 10, 10, QSizePolicy::Expanding, QSizePolicy::Expanding ),0,0);
    mainGrid->addWidget(messageLabel,1,1);
    mainGrid->addItem(new QSpacerItem ( 10, 10, QSizePolicy::Expanding, QSizePolicy::Expanding ),2,2);


}

InfoWidget::~InfoWidget()
{
}

void InfoWidget::showInfoMessage(QString message)
{
    qDebug()<< "showing infoWindow in 500ms";
    timer->start(500);
    messageLabel->setText(message);
    move (Parent->rect().center());
}

void InfoWidget::hideInfoMessage()
{
    timer->stop();
    hide();
}
