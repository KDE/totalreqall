cmake_minimum_required(VERSION 3.5)

project(TotalReqall LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED)

set(TS_FILES TotalReqall_en_US.ts)
set(QRC_FILES
	bible.qrc
	general.qrc
	icons.qrc
)

include_directories(
	thirdparty/maddy/include
	/usr/include/sword # TODO: make this work on all platforms
)

if(ANDROID)
  add_library(TotalReqall SHARED
    Bible.cpp
    ChooseReferenceWidget.cpp
    MainWindow.cpp
    MarkdownDialog.cpp
    MemorizeEdit.cpp
    MemorizeWidget.cpp
    SettingsDialog.cpp
    SimpleRefChooser.cpp
    main.cpp
	${QRC_FILES}
	${TS_FILES}
  )
else()
  add_executable(TotalReqall
    Bible.cpp
    ChooseReferenceWidget.cpp
    MainWindow.cpp
    MarkdownDialog.cpp
    MemorizeEdit.cpp
    MemorizeWidget.cpp
    SettingsDialog.cpp
    SimpleRefChooser.cpp
    main.cpp
	${QRC_FILES}
    ${TS_FILES}
  )
endif()

target_link_libraries(TotalReqall PRIVATE Qt5::Widgets sword)

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
