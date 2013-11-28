#include <QApplication>
#include "whassup.h"


int main( int argc, char ** argv ) {

    Q_INIT_RESOURCE(whassup);


    QApplication window( argc, argv );
        window.setQuitOnLastWindowClosed(false);

    Whassup *mw = new Whassup();

    QIcon mwIcon(":/images/ok.png");
        mw->setWindowIcon(mwIcon);
        mw->show();

        window.connect( &window, SIGNAL(lastWindowClosed()), &window, SLOT(quit()) );
    return window.exec();
}
