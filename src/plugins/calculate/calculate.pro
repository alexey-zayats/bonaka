DEFINES += CALCULATE_LIBRARY

QT += \
	network \
	widgets \

include(../../plugin.pri)

SOURCES += \
	calculate_mode.cpp \
	calculate_icons.cpp \
    calculatewindow.cpp \
    calculateplugin.cpp \
    boilerwidget.cpp \
    heatexchanger.cpp \
    heatvolume.cpp \
    treemodelcompleter.cpp \
    scum.cpp \
    boiler.cpp \
    boilercalculate.cpp \
    boilermodels.cpp

HEADERS += \
        calculate_mode.h \
        calculate_global.h \
        calculate_icons.h \
        calculate_constants.h \
    calculatewindow.h \
    calculateplugin.h \
    boilerwidget.h \
    heatexchanger.h \
    heatvolume.h \
    treemodelcompleter.h \
    scum.h \
    boiler.h \
    boilercalculate.h \
    boilermodels.h

RESOURCES += \
        calculate.qrc \

FORMS += \
    boilerform.ui
