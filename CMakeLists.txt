cmake_minimum_required(VERSION 3.5)

project(RainGraphic LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets LinguistTools Sql Concurrent REQUIRED)

set(TS_FILES RainGraphic_ru_RU.ts)

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

target_link_libraries(RainGraphic PRIVATE Qt5::Widgets Qt5::Sql Qt5::Concurrent)

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
