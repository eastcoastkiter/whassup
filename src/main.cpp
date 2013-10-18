#include <QApplication> 
#include "application.h"
#include "closeEvent.h"

int main( int argc, char ** argv ) {

    Q_INIT_RESOURCE(application);

    QApplication window( argc, argv );
        window.setQuitOnLastWindowClosed(false);
        window.installEventFilter(new myEventFilter());

    ApplicationWindow *mw = new ApplicationWindow();

    QIcon mwIcon(":/ok.png");
        mw->setWindowIcon(mwIcon);
        mw->show();

        window.connect( &window, SIGNAL(lastWindowClosed()), &window, SLOT(quit()) );
    return window.exec();
}
