DEFINES += ESTIMATION_LIBRARY
QT += \
	network \
	widgets \

include(../../plugin.pri)

SOURCES += \
	estimation.cpp \
	estimationmode.cpp \
	estimationicons.cpp

HEADERS += \
	estimation.h \
	estimationmode.h \
	estimation_global.h \
	estimationicons.h \
	estimationconstants.h

RESOURCES += \
	estimation.qrc \

