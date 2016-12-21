include(../../library.pri)

DEFINES += WIDGETS_LIBRARY
unix:LIBS += $$QMAKE_LIBS_DYNLOAD

QT += gui widgets

SOURCES += \
	filechooser.cpp \
	widgets.cpp \
	plugindetailsview.cpp \
	pluginerroroverview.cpp \
	pluginerrorview.cpp \
	pluginview.cpp \

HEADERS += \
	filechooser.h \
	widgets_global.h \
	plugindetailsview.h \
	pluginerroroverview.h \
	pluginerrorview.h \
	pluginview.h \

FORMS += \
	plugindetailsview.ui \
	pluginerroroverview.ui \
	pluginerrorview.ui \

RESOURCES += \
	pluginview.qrc \

