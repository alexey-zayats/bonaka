DEFINES += CORE_LIBRARY
DEFINES += APP_SHOW_BUILD_DATE

QT += \
	help \
	network \
	printsupport \
	sql \
	gui \
	widgets \

# embedding build time information prevents repeatedly binary exact versions from same source code
isEmpty(APP_SHOW_BUILD_DATE): APP_SHOW_BUILD_DATE = $$(APP_SHOW_BUILD_DATE)
!isEmpty(APP_SHOW_BUILD_DATE): DEFINES += APP_SHOW_BUILD_DATE

include(../../plugin.pri)

msvc: QMAKE_CXXFLAGS += -wd4251 -wd4290 -wd4250

SOURCES += \
	actionbar.cpp \
	tabwidget.cpp \
	mainwindow.cpp \
	coreplugin.cpp \
	manhattanstyle.cpp \
	styleanimator.cpp \
	coreicons.cpp \
	themechooser.cpp \
	id.cpp \
	icore.cpp \
	icontext.cpp \
	imode.cpp \
	idocument.cpp \
	modemanager.cpp \
	actionmanager/actionmanager.cpp \
	actionmanager/actioncontainer.cpp \
	actionmanager/command.cpp \
	actionmanager/commandbutton.cpp \
	actionmanager/commandmappings.cpp \
	actionmanager/commandsfile.cpp \
	statusbarmanager.cpp \
	statusbarwidget.cpp \
	minisplitter.cpp \
	dialogs/ioptionspage.cpp \
	dialogs/settingsdialog.cpp \
	dialogs/shortcutsettings.cpp \
	rightpane.cpp \
	navigationwidget.cpp \
	navigationsubwidget.cpp \
	inavigationwidgetfactory.cpp \
	systemsettings.cpp \
	versiondialog.cpp \
	windowsupport.cpp \
	settingsdatabase.cpp \
	generalsettings.cpp \
	infobar.cpp \
	outputpanemanager.cpp \
	outputpane.cpp \
	ioutputpane.cpp \
	findplaceholder.cpp \
	find/findtoolbar.cpp \
	find/ifindfilter.cpp \
	find/ifindsupport.cpp \
	find/currentdocumentfind.cpp \
	find/findtoolwindow.cpp \
	find/searchresultwindow.cpp \
	find/findplugin.cpp \
	find/searchresultwidget.cpp \
	find/searchresulttreeitemdelegate.cpp \
	find/searchresulttreeitems.cpp \
	find/searchresulttreemodel.cpp \
	find/searchresulttreeview.cpp \
	plugindialog.cpp \
	find/itemviewfind.cpp \
	find/basetextfind.cpp \
	editormanager/ieditor.cpp \
	editormanager/ieditorfactory.cpp \
	editormanager/editorview.cpp \
	editormanager/editorwindow.cpp \
	editormanager/editormanager.cpp \
	editormanager/editorarea.cpp \
	editormanager/openeditorswindow.cpp \
	editormanager/openeditorsview.cpp \
	editormanager/documentmodel.cpp \
	opendocumentstreeview.cpp \
	documentmanager.cpp \
	editortoolbar.cpp \
	fileiconprovider.cpp \
	progressmanager/progressmanager.cpp \
	progressmanager/progressview.cpp \
	progressmanager/progressbar.cpp \
	progressmanager/futureprogress.cpp \
	messagemanager.cpp \
	messageoutputwindow.cpp \
	outputwindow.cpp \
	iwelcomepage.cpp \
	locator/ilocatorfilter.cpp \
	sidebar.cpp \
	sidebarwidget.cpp \

HEADERS += \
	actionbar.h \
	tabwidget.h \
	mainwindow.h \
	coreplugin.h \
	core_global.h \
	manhattanstyle.h \
	styleanimator.h \
	coreicons.h \
	themechooser.h \
	coreconstants.h \
	id.h \
	icore.h \
	icontext.h \
	imode.h \
	idocument.h \
	modemanager.h \
	actionmanager/actionmanager.h \
	actionmanager/actionmanager_p.h \
	actionmanager/actioncontainer.h \
	actionmanager/actioncontainer_p.h \
	actionmanager/command.h \
	actionmanager/command_p.h \
	actionmanager/commandbutton.h \
	actionmanager/commandmappings.h \
	actionmanager/commandsfile.h \
	statusbarmanager.h \
	statusbarwidget.h \
	minisplitter.h \
	dialogs/ioptionspage.h \
	dialogs/settingsdialog.h \
	dialogs/shortcutsettings.h \
	rightpane.h \
	navigationwidget.h \
	navigationsubwidget.h \
	inavigationwidgetfactory.h \
	systemsettings.h \
	versiondialog.h \
	windowsupport.h \
	settingsdatabase.h \
	generalsettings.h \
	infobar.h \
	outputpanemanager.h \
	outputpane.h \
	ioutputpane.h \
	findplaceholder.h \
	find/findtoolbar.h \
	find/ifindfilter.h \
	find/ifindsupport.h \
	find/textfindconstants.h \
	find/currentdocumentfind.h \
	find/findtoolwindow.h \
	find/searchresultwindow.h \
	find/findplugin.h \
	find/searchresultwidget.h \
	find/searchresulttreeitemdelegate.h \
	find/searchresulttreeitemroles.h \
	find/searchresulttreeitems.h \
	find/searchresulttreemodel.h \
	find/searchresulttreeview.h \
	find/searchresultcolor.h \
	find/itemviewfind.h \
	find/basetextfind.h \
	plugindialog.h \
	editormanager/ieditor.h \
	editormanager/ieditorfactory.h \
	editormanager/editorview.h \
	editormanager/editorwindow.h \
	editormanager/editormanager.h \
	editormanager/editormanager_p.h \
	editormanager/editorarea.h \
	editormanager/openeditorswindow.h \
	editormanager/openeditorsview.h \
	editormanager/documentmodel.h \
	opendocumentstreeview.h \
	documentmanager.h \
	editortoolbar.h \
	fileiconprovider.h \
	progressmanager/progressmanager_p.h \
	progressmanager/progressview.h \
	progressmanager/progressbar.h \
	progressmanager/futureprogress.h \
	progressmanager/progressmanager.h \
	messagemanager.h \
	messageoutputwindow.h \
	outputwindow.h \
	iwelcomepage.h \
	locator/ilocatorfilter.h \
	sidebar.h \
	sidebarwidget.h \

FORMS = \
	systemsettings.ui \
	generalsettings.ui \
	find/findwidget.ui \
	find/finddialog.ui \

RESOURCES += \
core.qrc

win32 {
	SOURCES += progressmanager/progressmanager_win.cpp
	QT += gui-private # Uses QPlatformNativeInterface.
	LIBS += -lole32 -luser32
}
else:macx {
	OBJECTIVE_SOURCES += \
		progressmanager/progressmanager_mac.mm
	LIBS += -framework AppKit
}
else:unix {
	SOURCES += progressmanager/progressmanager_x11.cpp
}
