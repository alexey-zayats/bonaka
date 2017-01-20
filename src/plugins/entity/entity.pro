DEFINES += ENTITY_LIBRARY
QT += \
	network \
	widgets \

include(../../plugin.pri)

SOURCES += \
	entity.cpp \
	entity_mode.cpp \
	entity_icons.cpp

HEADERS += \
	entity.h \
	entity_mode.h \
	entity_global.h \
	entity_icons.h \
	entity_constants.h

RESOURCES += \
	entity.qrc \

