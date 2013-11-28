#include "statusbar.h"

StatusBar::StatusBar(QWidget * parent) :QStatusBar(parent)
{

    pixlabel = new QLabel(this);
        pixlabel->setFrameStyle(QFrame::Plain);
        pixlabel->setAlignment(Qt::AlignLeft);

    addWidget(pixlabel,1);

    timer = new QTimer(this);
        timer->setSingleShot(true);
    connect (timer, SIGNAL(timeout()), this, SLOT(clearLabel()));
}

StatusBar::~StatusBar(){}

void StatusBar::setMessage(QString message, int status)
{
    // Don't overwrite crit messeges with ok messages
    if (timer->isActive())
    {
        if (status < laststatus)
        {
            return;
        }
    }

    qDebug() << "StatusBar::setMessage(QString message, int status)" << message << status;

    QImage pic;
    if (status == 1)
    {
        pic = QImage(":/images/warning.png");
    }
    else if (status == 2)
    {
        pic = QImage(":/images/critical.png");
    }
    else
    {
        pic = QImage(":/images/ok.png");
    }

    QImage image(QSize(width()-30,16),QImage::Format_RGB16);
    QPalette palette = pixlabel->palette();
    QColor bg = palette.color(QPalette::Active,pixlabel->backgroundRole());
    image.fill(bg);


    // tell the painter to draw on the QImage
    QPainter* painter = new QPainter(&image);
        painter->drawImage(QRect(0,0,16,16),pic);
        painter->setPen(Qt::black);
        painter->setFont(QFont(qApp->property("ClientFont").toString()));
        qDebug() << "image.rect()" << image.rect()<< "styleSheet" << qApp->styleSheet();
        painter->drawText(QRect(QPoint(20,0),QSize(image.width()-50,16)), message);


    pixlabel->setPixmap(QPixmap::fromImage(image));




    // status could be zero
    int time=(status+1)*4000;

    // remove message later on
    timer->start(time);

    //remember
    laststatus=status;
    update();
}

void StatusBar::clearLabel()
{
    // set by force
    timer->stop();
    clearMessage();
    pixlabel->setPixmap(QPixmap());
}
