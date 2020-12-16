cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(totalreqall VERSION 0.5.0)

set(QT_MIN_VERSION "5.10.0")
set(KF5_REQUIRED_VERSION "5.56.0")

find_package(ECM ${KF5_REQUIRED_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "5")
set(CPACK_PACKAGE_VERSION_PATCH "0")

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(FeatureSummary)

option(TTS_ENABLED "Compile with text-to-speech support" ON)

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Core Gui Widgets LinguistTools)
find_package(sword REQUIRED)
find_package(KF5 ${KF5_DEP_VERSION}
	REQUIRED COMPONENTS
	    XmlGui
		CoreAddons
		I18n
)

find_package(Qt5 ${QT_MIN_VERSION} CONFIG COMPONENTS TextToSpeech)

if(TTS_ENABLED AND NOT Qt5TextToSpeech_FOUND)
	message(FATAL_ERROR "You asked for TTS, but it couldn't be found.")
elseif(NOT TTS_ENABLED)
	add_definitions(-DNO_TTS)
endif()

configure_file(AppInfo.h.cmake AppInfo.h)

add_executable(TotalReqall
    ChooseReferenceWidget.cpp
    CustomContentAdder.cpp
    MainWindow.cpp
	MemorizeWidget.cpp
    SavedContentLoader.cpp
    SettingsDialog.cpp
    SimpleRefChooser.cpp
    UserSettings.cpp
    WelcomePage.cpp
    main.cpp
    general.qrc
    icons.qrc
)

target_link_libraries(TotalReqall
	Qt5::Widgets
    sword::sword
    KF5::CoreAddons
    KF5::XmlGui
    KF5::I18n
)

if(TTS_ENABLED)
	target_link_libraries(TotalReqall Qt5::TextToSpeech)
endif()

install(TARGETS TotalReqall ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES org.kde.totalreqall.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
install(PROGRAMS org.kde.totalreqall.desktop DESTINATION  ${XDG_APPS_INSTALL_DIR})

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} TotalReqall_en_US.ts)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
