APP_NAME   = "App"

include(../../../bonaka.pri)

TEMPLATE = app
VERSION = $$APP_VERSION
TARGET = $${APP_TARGET}

macx:DESTDIR = $$APP_BUILD_TREE
else:DESTDIR = $$APP_BIN_PATH

QT += network widgets

include(../../rpath.pri)

LIBS *= -l$$qtLibraryName(extensionsystem) \
	-l$$qtLibraryName(utils) \
	-l$$qtLibraryName(widgets)

APP_GIT_REVISION = $$system(git --git-dir $$APP_SOURCE_TREE/.git --work-tree $$APP_BUILD_TREE describe --always --tags)

win32 {
	RC_FILE = bonaka.rc
	target.path = $$APP_BIN_PATH
	INSTALLS += target
} else:macx {
	LIBS += -framework CoreFoundation
	ASSETCATALOG.files = $$PWD/bonaka.xcassets
	macx-xcode {
		QMAKE_BUNDLE_DATA += ASSETCATALOG
	} else {
		ASSETCATALOG.output = $$APP_DATA_PATH/bonaka.icns
		ASSETCATALOG.commands = xcrun actool \
			--app-icon bonaka \
			--output-partial-info-plist $$shell_quote($(TMPDIR)/bonaka.Info.plist) \
			--platform macosx \
			--minimum-deployment-target 10.7 \
			--compile $$shell_quote($$APP_DATA_PATH) \
			$$shell_quote($$PWD/bonaka.xcassets) > /dev/null
		ASSETCATALOG.input = ASSETCATALOG.files
		ASSETCATALOG.CONFIG += no_link target_predeps
		QMAKE_EXTRA_COMPILERS += ASSETCATALOG
	}
	QMAKE_INFO_PLIST = Info.plist
} else {
	target.path  = $$APP_BIN_PATH
	INSTALLS    += target
}

QMAKE_SUBSTITUTES += $$PWD/bonaka_version.h.in

SOURCES += \
	bonaka.cpp

HEADERS += \
	bonaka.h
