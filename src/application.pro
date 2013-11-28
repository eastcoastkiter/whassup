HEADERS       = \
                servicetree.h \
                livesocket.h \
                helpwidget.h \
                filterwidget.h \
                hostinfowidget.h \
                pcdialog.h \
                node.h \
                edge.h \
                pcscene.h \
                livesocketrequest.h \
                serviceinfowidget.h \
                logviewitem.h \
                logview.h \
                generationtree.h \
                livesocketssl.h \
                configwidget.h \
                commandwidget.h \
                livesocketcommand.h \
                globals.h \
                resultlabel.h \
                whassup.h \
                statusbar.h \
                servicetreeitem.h \
                configdialog.h \
                webcommunication.h

SOURCES       = \
                helpwidget.cpp \
                livesocket.cpp \
                servicetree.cpp \
                main.cpp \
                filterwidget.cpp \
                hostinfowidget.cpp \
                pcdialog.cpp \
                node.cpp \
                edge.cpp \
                pcscene.cpp \
                livestatusrequest.cpp \
                serviceinfowidget.cpp \
                logviewitem.cpp \
                logview.cpp \
                generationtree.cpp \
                livesocketssl.cpp \
                configwidget.cpp \
                commandwidget.cpp \
                livesocketcommand.cpp \
                resultlabel.cpp \
                whassup.cpp \
                statusbar.cpp \
                servicetreeitem.cpp \
                configdialog.cpp \
                webcommunication.cpp

CODECFORTR    = ISO-8859-1
RESOURCES     += whassup.qrc

DEFINES += VERSION="\"1.2\""
TARGET = whassup
# install
target.path = .
#sources.files = $$SOURCES $$HEADERS $$RESOURCES $$FORMS tools.pro README
#sources.path = $$[QT_INSTALL_EXAMPLES]/tools
INSTALLS += target
QT += network core gui widgets
CONFIG += qt ordered debug console
#CONFIG+=qml_debug
#CONFIG += qt ordered staticlib static release
#LFLAGS = -static-libgcc
#QMAKE_LFLAGS += -static-libgcc
#DEFINES += QT_STATIC_BUILD
QMAKE_CXXFLAGS = -Wno-unused-variable

OTHER_FILES += \
    README.txt \
    whassup_server.pl \
    Changelog.txt \
    icons.h \
    helpfiles\configWidget.html \
    helpfiles\filterWidget.html \
    helpfiles\Welcome.html \
    helpfiles\serviceWidget.html \
    helpfiles\serviceDetail.html \
    helpfiles\hostDetail.html \
    helpfiles/downtimeWidget.html \
    helpfiles/logWidget.html \
    helpfiles/whassupserver.html \
    images/web.png \
    images/warning.png \
    images/ok.png \
    images/notes.png \
    images/critical.png \
    images/action.png \
    images/log.png \
    images/info.png \
    images/notification_enabled.png \
    images/notification_disabled.png \
    images/checks_enabled.png \
    images/checks_disabled.png \
    INSTALL.txt \
    whassup.fcgi


