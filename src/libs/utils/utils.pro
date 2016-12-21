include(../../library.pri)

DEFINES += APP_UTILS_LIB

QT += gui widgets

SOURCES += \
	hostosinfo.cpp \
	fileutils.cpp \
	savefile.cpp \
	systemid.cpp \
	utils.cpp \
	emailvalidator.cpp \
	styledbar.cpp \
	appmainwindow.cpp \
	mainwindow.cpp \
	stringutils.cpp \
	stylehelper.cpp \
	theme/theme.cpp \
	icon.cpp \
	proxyaction.cpp \
	fadingindicator.cpp \
	lineedit.cpp \
	completinglineedit.cpp \
	execmenu.cpp \
	historycompleter.cpp \
	headerviewstretcher.cpp \
	checkablemessagebox.cpp \
	qtcolorbutton.cpp \
	categorysortfiltermodel.cpp \
	itemviews.cpp \
	treemodel.cpp \
	overridecursor.cpp \
	filesearch.cpp \
	runextensions.cpp \
	outputformatter.cpp \
	synchronousprocess.cpp \
	ansiescapecodehandler.cpp \
	navigationtreeview.cpp \
	tooltip/tips.cpp \
	tooltip/tooltip.cpp \

HEADERS += \
	hostosinfo.h \
	osspecificaspects.h \
	savefile.h \
	fileutils.h \
	systemid.h \
	utils_global.h \
	emailvalidator.h \
	systemid_p.h \
	styledbar.h \
	appmainwindow.h \
	mainwindow.h \
	stringutils.h \
	algorithm.h \
	stylehelper.h \
	theme/theme.h \
	theme/theme_p.h \
	icon.h \
	proxyaction.h \
	fadingindicator.h \
	lineedit.h \
	completinglineedit.h \
	execmenu.h \
	historycompleter.h \
	utilsicons.h \
	headerviewstretcher.h \
	checkablemessagebox.h \
	qtcolorbutton.h \
	categorysortfiltermodel.h \
	itemviews.h \
	treemodel.h \
	overridecursor.h \
	filesearch.h \
	mapreduce.h \
	runextensions.h \
	functiontraits.h \
	outputformat.h \
	outputformatter.h \
	synchronousprocess.h \
	ansiescapecodehandler.h \
	navigationtreeview.h \
	tooltip/tips.h \
	tooltip/tooltip.h \
	tooltip/effects.h \
	tooltip/reuse.h \

!macx {
	win32 {
	   SOURCES += systemid_win.cpp
	} else {
	   SOURCES += systemid_unix.cpp
	}
} else {
	SOURCES += systemid_mac.cpp
}

osx {
	HEADERS += \
		fileutils_mac.h
	OBJECTIVE_SOURCES += \
		fileutils_mac.mm
	LIBS += -framework Foundation
}

win32 {
	LIBS += -luser32 -lshell32
	LIBS += -liphlpapi -lws2_32
}
