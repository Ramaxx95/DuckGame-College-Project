cmake_minimum_required(VERSION 3.16)

project(lobby VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_library(lobby
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_library(lobby SHARED
        ${PROJECT_SOURCES}
    )
endif()

target_link_libraries(lobby PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Agrego el path del client para tener acceso al protocolo
target_include_directories(lobby PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../client)
target_include_directories(lobby PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../common)

# Agrego ruta para las imagenes del lobby
add_definitions(-DIMG_PATH="${PROJECT_SOURCE_DIR}/img") 

# Define la ruta a la carpeta /sprites que está fuera del directorio actual
set(SPRITES_PATH "${PROJECT_SOURCE_DIR}/../client/sprites")

# Agrega esta ruta como una definición
add_definitions(-DSPRITES_PATH="${SPRITES_PATH}")

set_target_properties(lobby PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
)

include(GNUInstallDirs)
install(TARGETS lobby
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(lobby)
endif()
