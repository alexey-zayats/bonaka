
macx {
	QMAKE_LFLAGS += -Wl,-rpath,@loader_path/../Frameworks/,-rpath,@executable_path/../Frameworks/
	QMAKE_RPATHDIR =
} else:linux-* {
	QMAKE_LFLAGS += -Wl,-z,origin \'-Wl,-rpath,../$${APP_LIBRARY_BASENAME}/\'
	QMAKE_RPATHDIR =
}
