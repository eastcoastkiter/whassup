#include <QApplication> 
#include "application.h"

int main( int argc, char ** argv ) {
    QApplication window( argc, argv );
    ApplicationWindow *mw = new ApplicationWindow();
   
    mw->show();
    window.connect( &window, SIGNAL(lastWindowClosed()), &window, SLOT(quit()) );
    return window.exec();
}
