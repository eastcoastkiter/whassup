#ifndef CLOSEEVENT_H
#define CLOSEEVENT_H

#include <QEvent>
#include <QApplication>
#include <QMessageBox>


// Just ignore close events and minimze
class myEventFilter: public QObject
{
  public:
  myEventFilter():QObject()
  {};
  ~myEventFilter(){};

  bool eventFilter(QObject* object,QEvent* event)
  {
      if(event->type() == QEvent::Close)
      {
        event->ignore();
        qApp->activeWindow()->hide();
        return true;
      }
      else
      {
        return QObject::eventFilter(object,event);
      }
  };
};

#endif // CLOSEEVENT_H
