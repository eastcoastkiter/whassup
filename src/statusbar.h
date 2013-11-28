#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QStatusBar>
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QFrame>
#include <QTimer>
#include <QDebug>
#include <QPainter>
#include <QApplication>
#include <QPalette>
#include <QPalette>
#include <QColor>

class StatusBar : public QStatusBar
{
        Q_OBJECT
public:
    StatusBar(QWidget * parent = 0);
    ~StatusBar();

    QLabel* pixlabel;
    QLabel *messagelabel;
    QTimer *timer;
    int laststatus;
public slots:
    void setMessage(QString message, int status);
    void clearLabel();
private slots:

};
#endif // STATUSBAR_H
