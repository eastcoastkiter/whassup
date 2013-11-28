#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QMouseEvent>
#include <QWhatsThis>
#include <QEvent>
#include <QWhatsThisClickedEvent>
#include <QHelpEvent>

class ConfigWidget;
#include <configwidget.h>

class ConfigDialog : public QDialog
{
    Q_OBJECT
public:
    ConfigDialog(QWidget *parent, Qt::WindowFlags f=0);
    ~ConfigDialog();
protected:
};

#endif // CONFIGDIALOG_H
