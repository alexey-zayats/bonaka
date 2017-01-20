!isEmpty(BONAKA_PRI_INCLUDED):error("bonaka.pri already included")
BONAKA_PRI_INCLUDED = 1

APP_VERSION = 0.0.1
APP_COMPAT_VERSION = 0.0.0

# enable c++11
CONFIG += c++11

defineReplace(qtLibraryName) {
   unset(LIBRARY_NAME)
   LIBRARY_NAME = $$1
   CONFIG(debug, debug|release) {
	  !debug_and_release|build_pass {
		  mac:RET = $$member(LIBRARY_NAME, 0)_debug
			 else:win32:RET = $$member(LIBRARY_NAME, 0)d
	  }
   }
   isEmpty(RET):RET = $$LIBRARY_NAME
   return($$RET)
}

defineTest(minQtVersion) {
	maj = $$1
	min = $$2
	patch = $$3
	isEqual(QT_MAJOR_VERSION, $$maj) {
		isEqual(QT_MINOR_VERSION, $$min) {
			isEqual(QT_PATCH_VERSION, $$patch) {
				return(true)
			}
			greaterThan(QT_PATCH_VERSION, $$patch) {
				return(true)
			}
		}
		greaterThan(QT_MINOR_VERSION, $$min) {
			return(true)
		}
	}
	greaterThan(QT_MAJOR_VERSION, $$maj) {
		return(true)
	}
	return(false)
}

# For use in custom compilers which just copy files
defineReplace(stripSrcDir) {
	return($$relative_path($$absolute_path($$1, $$OUT_PWD), $$_PRO_FILE_PWD_))
}

isEmpty(APP_LIBRARY_BASENAME) {
	APP_LIBRARY_BASENAME = lib
}

APP_SOURCE_TREE = $$PWD
isEmpty(APP_BUILD_TREE) {
	sub_dir = $$_PRO_FILE_PWD_
	sub_dir ~= s,^$$re_escape($$PWD),,
	APP_BUILD_TREE = $$clean_path($$OUT_PWD)
	APP_BUILD_TREE ~= s,$$re_escape($$sub_dir)$,,
}

APP_TARGET   = "Bonaka"

osx {
	APP_BUNDLE = $$APP_BUILD_TREE/$${APP_TARGET}.app

#	# set output path if not set manually
	APP_OUTPUT_PATH = $$APP_BUNDLE/Contents

	APP_LIBRARY_PATH = $$APP_OUTPUT_PATH/Frameworks
	APP_PLUGIN_PATH  = $$APP_OUTPUT_PATH/PlugIns
	APP_LIBEXEC_PATH = $$APP_OUTPUT_PATH/Resources
	APP_DATA_PATH    = $$APP_OUTPUT_PATH/Resources
	APP_BIN_PATH     = $$APP_OUTPUT_PATH/MacOS
} else {
	# target output path if not set manually
	APP_OUTPUT_PATH = $$APP_BUILD_TREE

	APP_LIBRARY_PATH = $$APP_OUTPUT_PATH/$$APP_LIBRARY_BASENAME
	APP_PLUGIN_PATH  = $$APP_OUTPUT_PATH/plugins
	APP_DATA_PATH    = $$APP_OUTPUT_PATH/share
	APP_BIN_PATH     = $$APP_OUTPUT_PATH/bin
	win32: \
		APP_LIBEXEC_PATH = $$APP_OUTPUT_PATH/bin
	else: \
		APP_LIBEXEC_PATH = $$APP_OUTPUT_PATH/lib
}

LINK_LIBRARY_PATH = $$APP_LIBRARY_PATH
LINK_PLUGIN_PATH  = $$APP_PLUGIN_PATH

INCLUDEPATH += \
	$$APP_BUILD_TREE/src/apps \ # for <app/app_version.h>
	$$APP_SOURCE_TREE/src/libs \
	$$APP_SOURCE_TREE/tools

APP_PLUGIN_DIRS_FROM_ENVIRONMENT = $$(APP_PLUGIN_DIRS)
APP_PLUGIN_DIRS += $$split(APP_PLUGIN_DIRS_FROM_ENVIRONMENT, $$QMAKE_DIRLIST_SEP)
APP_PLUGIN_DIRS += $$APP_SOURCE_TREE/src/plugins
for(dir, APP_PLUGIN_DIRS) {
	INCLUDEPATH += $$dir
}

CONFIG += \
	depend_includepath \
	no_include_pwd

LIBS *= -L$$LINK_LIBRARY_PATH  # application libraries
exists($$APP_LIBRARY_PATH): LIBS *= -L$$APP_LIBRARY_PATH  # library path from output path

DEFINES += APP_LIBRARY_BASENAME=\\\"$$APP_LIBRARY_BASENAME\\\"

!macx:DEFINES += QT_USE_FAST_OPERATOR_PLUS QT_USE_FAST_CONCATENATION

unix {
	CONFIG(debug, debug|release):OBJECTS_DIR = $${OUT_PWD}/.obj/debug-shared
	CONFIG(release, debug|release):OBJECTS_DIR = $${OUT_PWD}/.obj/release-shared

	CONFIG(debug, debug|release):MOC_DIR = $${OUT_PWD}/.moc/debug-shared
	CONFIG(release, debug|release):MOC_DIR = $${OUT_PWD}/.moc/release-shared

	RCC_DIR = $${OUT_PWD}/.rcc
	UI_DIR = $${OUT_PWD}/.uic
}

msvc {
	#Don't warn about sprintf, fopen etc being 'unsafe'
	DEFINES += _CRT_SECURE_NO_WARNINGS
	QMAKE_CXXFLAGS_WARN_ON *= -w44996
	# Speed up startup time when debugging with cdb
	QMAKE_LFLAGS_DEBUG += /INCREMENTAL:NO
}

qt {
	QT += network \
		widgets \
		gui \
		core
}

!isEmpty(APP_PLUGIN_DEPENDS) {
	LIBS *= -L$$APP_PLUGIN_PATH  # plugin path from output directory
	LIBS *= -L$$LINK_PLUGIN_PATH  # when output path is different from app build directory
}

# recursively resolve plugin deps
done_plugins =
for(ever) {
	isEmpty(APP_PLUGIN_DEPENDS): \
		break()
	done_plugins += $$APP_PLUGIN_DEPENDS
	for(dep, APP_PLUGIN_DEPENDS) {
		dependencies_file =
		for(dir, APP_PLUGIN_DIRS) {
			exists($$dir/$$dep/$${dep}_dependencies.pri) {
				dependencies_file = $$dir/$$dep/$${dep}_dependencies.pri
				break()
			}
		}
		isEmpty(dependencies_file): \
			error("Plugin dependency $$dep not found")
		include($$dependencies_file)
		LIBS += -l$$qtLibraryName($$APP_PLUGIN_NAME)
	}
	APP_PLUGIN_DEPENDS = $$unique(APP_PLUGIN_DEPENDS)
	APP_PLUGIN_DEPENDS -= $$unique(done_plugins)
}

# recursively resolve library deps
done_libs =
for(ever) {
	isEmpty(APP_LIB_DEPENDS): \
		break()
	done_libs += $$APP_LIB_DEPENDS
	for(dep, APP_LIB_DEPENDS) {
		include($$PWD/src/libs/$$dep/$${dep}_dependencies.pri)
		LIBS += -l$$qtLibraryName($$APP_LIB_NAME)
	}
	APP_LIB_DEPENDS = $$unique(APP_LIB_DEPENDS)
	APP_LIB_DEPENDS -= $$unique(done_libs)
}
