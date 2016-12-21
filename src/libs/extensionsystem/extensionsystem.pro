
include(../../library.pri)

DEFINES += EXTENSIONSYSTEM_LIBRARY
unix:LIBS += $$QMAKE_LIBS_DYNLOAD

HEADERS += \
	invoker.h \
	iplugin.h \
	iplugin_p.h \
	extensionsystem_global.h \
	pluginmanager.h \
	pluginmanager_p.h \
	pluginspec.h \
	pluginspec_p.h \
	plugincollection.h \


SOURCES += \
	invoker.cpp \
	iplugin.cpp \
	pluginmanager.cpp \
	pluginspec.cpp \
	plugincollection.cpp \
	extentionsystem.cpp \


