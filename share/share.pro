TEMPLATE = aux

include(../bonaka.pri)

STATIC_BASE = $$PWD
STATIC_OUTPUT_BASE = $$APP_DATA_PATH
STATIC_INSTALL_BASE = $$APP_DATA_PATH

DATA_DIRS = \
	themes \

macx: DATA_DIRS += scripts


for(data_dir, DATA_DIRS) {
	files = $$files($$PWD/$$data_dir/*, true)
	# Info.plist.in are handled below
	for(file, files):!contains(file, ".*/Info\\.plist\\.in$"):!exists($$file/*): \
		STATIC_FILES += $$file
}

include(../data.pri)
