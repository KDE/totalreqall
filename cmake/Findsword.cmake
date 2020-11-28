#
# CMake module to search for the sword library
#

find_package(PkgConfig)
pkg_check_modules(PC_sword QUIET sword)

find_path(sword_INCLUDE_DIR
	NAMES swmgr.h swmodule.h versekey.h markupfiltmgr.h
	PATHS ${PC_sword_INCLUDE_DIRS}
	PATH_SUFFIXES sword
)

find_library(sword_LIBRARY
	NAMES sword
	HINTS ${PC_sword_LIBRARY_DIRS}
)

set(sword_VERSION ${PC_sword_VERSION})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(sword
	FOUND_VAR sword_FOUND
	REQUIRED_VARS
	    sword_INCLUDE_DIR
		sword_LIBRARY
	VERSION_VAR sword_VERSION
)

if(sword_FOUND)
  set(sword_LIBRARIES ${sword_LIBRARY})
  set(sword_INCLUDE_DIRS ${sword_INCLUDE_DIR})
  set(sword_DEFINITIONS ${PC_sword_CFLAGS_OTHER})
endif()

if(sword_FOUND AND NOT TARGET sword::sword)
  add_library(sword::sword UNKNOWN IMPORTED)
  set_target_properties(sword::sword PROPERTIES
	IMPORTED_LOCATION "${sword_LIBRARY}"
	INTERFACE_COMPILE_OPTIONS "${PC_sword_CFLAGS_OTHER}"
	INTERFACE_INCLUDE_DIRECTORIES "${sword_INCLUDE_DIR}"
  )
endif()

mark_as_advanced(sword_LIBRARY sword_INCLUDE_DIR)

