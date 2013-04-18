HEADERS       = application.h \
                servicetree.h \
                livesocket.h \
                helpwidget.h \
                infoWidget.h \
    closeEvent.h

SOURCES       = application.cpp \
                helpwidget.cpp \
                livesocket.cpp \
                servicetree.cpp \
                infoWidget.cpp \
                main.cpp

CODECFORTR    = ISO-8859-1
RESOURCES     += application.qrc

# install
target.path = .
#sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS tools.pro README
#sources.path = $$[QT_INSTALL_EXAMPLES]/tools
INSTALLS += target
QT += network
#CONFIG += qt ordered debug
CONFIG += qt ordered staticlib static release
LFLAGS = -static-libgcc
QMAKE_LFLAGS += -static-libgcc
DEFINES += QT_STATIC_BUILD

OTHER_FILES += \
    README.txt \
    whassup_server.pl



