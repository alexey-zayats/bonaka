include($$replace(_PRO_FILE_PWD_, ([^/]+$), \\1/\\1_dependencies.pri))
TARGET = $$APP_LIB_NAME

include(../bonaka.pri)

win32 {
	DLLDESTDIR = $$APP_BIN_PATH
}

DESTDIR = $$APP_LIBRARY_PATH
osx{
	QMAKE_LFLAGS_SONAME = -Wl,-install_name,@rpath/../Frameworks/
}


include(rpath.pri)

TARGET = $$qtLibraryName($$TARGET)

TEMPLATE = lib
CONFIG += shared dll

contains(QT_CONFIG, reduce_exports):CONFIG += hide_symbols

!macx {
	win32 {
		dlltarget.path = $$APP_BIN_PATH
		INSTALLS += dlltarget
	} else {
		target.path = $$APP_LIBRARY_PATH
		INSTALLS += target
	}
}
