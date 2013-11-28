HEADERS       = application.h \
                servicetree.h \
                livesocket.h \
                helpwidget.h \
                infoWidget.h \
                filterwidget.h \
                hostinfowidget.h \
                pcdialog.h \
                node.h \
    edge.h \
    pcscene.h \
    livesocketrequest.h

SOURCES       = application.cpp \
                helpwidget.cpp \
                livesocket.cpp \
                servicetree.cpp \
                infoWidget.cpp \
                main.cpp \
                filterwidget.cpp \
                hostinfowidget.cpp \
                pcdialog.cpp \
                node.cpp \
    edge.cpp \
    pcscene.cpp \
    livestatusrequest.cpp

CODECFORTR    = ISO-8859-1
RESOURCES     += application.qrc

# install
target.path = .
#sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS tools.pro README
#sources.path = $$[QT_INSTALL_EXAMPLES]/tools
INSTALLS += target
QT += network core gui widgets
CONFIG += qt ordered debug
#CONFIG += qt ordered staticlib static release
LFLAGS = -static-libgcc
#QMAKE_LFLAGS += -static-libgcc
#DEFINES += QT_STATIC_BUILD
QMAKE_CXXFLAGS = -Wno-unused-variable

OTHER_FILES += \
    README.txt \
    whassup_server.pl


