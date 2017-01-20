include(../../library.pri)

DEFINES += APP_ENTITY_LIB
QT += sql

HEADERS += \
    object.h \
    errors.h \
    entity.h \
    util.h \
    keeper.h \
    attr.h \
    tree.h \
    defines.h \
    link.h \
    connection.h \
    file.h \
    role.h \
    user.h

SOURCES += \
    object.cpp \
    util.cpp \
    keeper.cpp \
    attr.cpp \
    tree.cpp \
    link.cpp \
    connection.cpp \
    file.cpp \
    role.cpp \
    user.cpp
