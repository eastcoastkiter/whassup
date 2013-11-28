#include <QApplication>
#include "application.h"


int main( int argc, char ** argv ) {

    Q_INIT_RESOURCE(application);

    QApplication window( argc, argv );
        window.setQuitOnLastWindowClosed(false);

    ApplicationWindow *mw = new ApplicationWindow();

    QIcon mwIcon(":/ok.png");
        mw->setWindowIcon(mwIcon);
        mw->show();

        window.connect( &window, SIGNAL(lastWindowClosed()), &window, SLOT(quit()) );
    return window.exec();
}
