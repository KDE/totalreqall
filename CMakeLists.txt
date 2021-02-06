cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(totalreqall VERSION 0.5.0)

set(QT_MIN_VERSION "5.10.0")
set(KF5_REQUIRED_VERSION "5.56.0")

find_package(ECM ${KF5_REQUIRED_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "6")
set(CPACK_PACKAGE_VERSION_PATCH "0")

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(FeatureSummary)
include(CMakeDependentOption)

# not required
# this is first to set Qt5_FOUND to ON even if the TTS is not found
find_package(Qt5 ${QT_MIN_VERSION} CONFIG COMPONENTS TextToSpeech)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SWORD REQUIRED IMPORTED_TARGET sword)
find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Core Gui Widgets LinguistTools)
find_package(sword REQUIRED)
find_package(KF5 ${KF5_DEP_VERSION}
	REQUIRED COMPONENTS
		XmlGui
		CoreAddons
		I18n
)

cmake_dependent_option(TTS_ENABLED "Compile with text-to-speech support" ON Qt5TextToSpeech_FOUND OFF)

if(NOT TTS_ENABLED)
	add_compile_definitions(NO_TTS)
endif()

configure_file(AppInfo.h.cmake AppInfo.h)

include_directories(src)

add_executable(totalreqall
	src/ChooseReferenceWidget.cpp
	src/CustomContentAdder.cpp
	src/Helper.cpp
	src/MainWindow.cpp
	src/MemorizeWidget.cpp
	src/SavedContentLoader.cpp
	src/SettingsDialog.cpp
	src/SimpleRefChooser.cpp
	src/UserSettings.cpp
	src/WelcomePage.cpp
	src/main.cpp
	general.qrc
	icons.qrc
)

target_link_libraries(totalreqall
	Qt5::Widgets
	sword::sword
	KF5::CoreAddons
	KF5::XmlGui
	KF5::I18n
	PkgConfig::SWORD
)

if(TTS_ENABLED)
	target_link_libraries(totalreqall Qt5::TextToSpeech)
endif()

install(TARGETS totalreqall ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES org.kde.totalreqall.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
install(PROGRAMS org.kde.totalreqall.desktop DESTINATION  ${XDG_APPS_INSTALL_DIR})

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} TotalReqall_en_US.ts)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
