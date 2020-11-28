cmake_minimum_required(VERSION 3.5)

project(TotalReqall LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "5")
set(CPACK_PACKAGE_VERSION_PATCH "0")

find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED)
find_package(sword REQUIRED 1.8.1)

configure_file(cmake/AppInfo.h AppInfo.h)

set(SRC_FILES
	ChooseReferenceWidget.cpp
	CustomContentAdder.cpp
	MainWindow.cpp
	MemorizeEdit.cpp
	MemorizeWidget.cpp
	SavedContentLoader.cpp
	SettingsDialog.cpp
	SimpleRefChooser.cpp
	WelcomePage.cpp
	main.cpp
)
set(TS_FILES TotalReqall_en_US.ts)
set(QRC_FILES
	general.qrc
	icons.qrc
)

add_executable(TotalReqall
	${SRC_FILES}
	${QRC_FILES}
	${TS_FILES}
)

# Unfortunately, this is required due to some wacky configuration in the Sword headers.
include_directories("/usr/include/sword")

target_link_libraries(TotalReqall PRIVATE Qt5::Widgets sword)

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
