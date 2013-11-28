#ifndef RESULTLABEL_H
#define RESULTLABEL_H

#include <QLabel>
#include <QEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QWidget>
#include <QClipboard>
#include <QApplication>
#include <QRegExp>

class ResultLabel : public QLabel
{
    Q_OBJECT

    public:
    ResultLabel(const QString & text, QWidget * parent = 0, Qt::WindowFlags f = 0);
    ResultLabel(QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~ResultLabel(){}

protected:
    void mouseDoubleClickEvent(QMouseEvent * event);
};

#endif // RESULTLABEL_H
