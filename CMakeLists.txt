cmake_minimum_required(VERSION 3.5)

project(RainGraphic LANGUAGES CXX)

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

find_package(Qt5 COMPONENTS Widgets LinguistTools Sql REQUIRED)

set(TS_FILES RainGraphic_ru_RU.ts)

if(ANDROID)
  add_library(RainGraphic SHARED
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    tile.cpp
    tile.h
    surface.cpp
    surface.h
    surfaceview.cpp
    surfaceview.h
    sqlite.cpp
    sqlite.h
    ${TS_FILES}
  )
else()
  add_executable(RainGraphic
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    tile.cpp
    tile.h
    surface.cpp
    surface.h
    surfaceview.cpp
    surfaceview.h
    sqlite.cpp
    sqlite.h
    ${TS_FILES}
  )
endif()

target_link_libraries(RainGraphic PRIVATE Qt5::Widgets Qt5::Sql)

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})