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

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Core Gui Widgets LinguistTools)
find_package(sword REQUIRED)
find_package(KF5 ${KF5_DEP_VERSION}
	REQUIRED COMPONENTS
	    XmlGui
		CoreAddons
)


configure_file(AppInfo.h.cmake AppInfo.h)

add_executable(TotalReqall
    ChooseReferenceWidget.cpp
    CustomContentAdder.cpp
    MainWindow.cpp
    MemorizeWidget.cpp
    SavedContentLoader.cpp
    SettingsDialog.cpp
    SimpleRefChooser.cpp
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
)

install(TARGETS TotalReqall ${INSTALL_TARGETS_DEFAULT_ARGS})

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} TotalReqall_en_US.ts)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
