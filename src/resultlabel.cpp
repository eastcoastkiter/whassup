#include "resultlabel.h"

ResultLabel::ResultLabel(const QString & text, QWidget * parent, Qt::WindowFlags f) : QLabel(text,parent,f)
{}
ResultLabel::ResultLabel(QWidget * parent, Qt::WindowFlags f) : QLabel(parent,f)
{}

void ResultLabel::mouseDoubleClickEvent(QMouseEvent * event)
{
    //remove () for example at host_address
    QString t=text().trimmed();
    if (accessibleName() == "L_Host_Address")
    {
        t.remove("(");
        t.remove(")");
    }
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(t);
event->accept();
}
