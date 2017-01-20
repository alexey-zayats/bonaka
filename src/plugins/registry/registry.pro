DEFINES += REGISTRY_LIBRARY
QT += \
	network \
	widgets \

include(../../plugin.pri)

SOURCES += \
	registry.cpp \
	registry_mode.cpp \
	registry_icons.cpp

HEADERS += \
	registry.h \
	registry_mode.h \
	registry_global.h \
	registry_icons.h \
	registry_constants.h

RESOURCES += \
	registry.qrc \

