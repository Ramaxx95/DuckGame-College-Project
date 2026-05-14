cmake_minimum_required(VERSION 3.13)

if (POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

if (NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)
endif()

# strongly encouraged to enable this globally to avoid conflicts between
# -Wpedantic being enabled and -std=c++20 and -std=gnu++20 for example
# when compiling with PCH enabled
set(CMAKE_CXX_EXTENSIONS OFF)

project(taller_tp
    VERSION 1.0
    HOMEPAGE_URL "https://github.com/URL-HERE"
    LANGUAGES CXX
    )

# Find and link SDL2
find_package(SDL2 REQUIRED)
# find_package(SDL2_mixer REQUIRED)
# find_package(SDL2_image REQUIRED)
# find_package(SDL2_ttf REQUIRED)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core)
find_package(yaml-cpp REQUIRED)

option(TALLER_TESTS "Enable / disable tests." ON)
option(TALLER_CLIENT "Enable / disable client program." ON)
option(TALLER_SERVER "Enable / disable server program." ON)
option(TALLER_EDITOR "Enable / disable editor program." ON)
option(TALLER_MAKE_WARNINGS_AS_ERRORS "Enable / disable warnings as errors." ON)

message(CMAKE_CXX_COMPILER_ID="${CMAKE_CXX_COMPILER_ID}")

# Define target: lib common between client, server and editor programs
# HEY TODO XXX: change the name to something that you like

add_library(taller_common STATIC)

# add the source files that make the lib
# HEY!! TODO XXX: add more subfolders here
add_subdirectory(common/)
add_subdirectory(lobby/)
add_subdirectory(init/)

# set compiler warnings to the targets
include(cmake/CompilerWarnings.cmake)
set_project_warnings(taller_common ${TALLER_MAKE_WARNINGS_AS_ERRORS} FALSE)

# Say where the headers will be found required by library target
# Make this PUBLIC so any dependant linking lib will inherit this
# location (so our dependants will not have to explicitly request this)
target_include_directories(taller_common PUBLIC .)

# Install libSDL2pp (for client or editor)
# HEY!! TODO XXX: add more dependendies (either for client, server or editor)
if(TALLER_CLIENT OR TALLER_EDITOR)
    # Support both SDL_Image and SDL_ttf font
    set(SDL2PP_WITH_IMAGE ON) # imagenes
    set(SDL2PP_WITH_TTF ON) # fonts
    set(SDL2PP_WITH_MIXER ON) # sonido

    include(FetchContent)

    FetchContent_Declare(
        libSDL2pp
        URL https://github.com/libSDL2pp/libSDL2pp/archive/77a97972b3d86b7f3da7589e71da3eb5d993cb47.zip
    )
    FetchContent_MakeAvailable(libSDL2pp)

endif()

# Program section
# ---------------
if(TALLER_CLIENT)
    # HEY!! TODO XXX: Change the name of the executable to the one you like!
    add_executable(taller_client)

    # Make them depend on taller_common lib
    add_dependencies(taller_client taller_common lobby init SDL2pp::SDL2pp)

    # Add source files and enable warnings
    # HEY!! TODO XXX: add more subfolders here
    add_subdirectory(client)

    # Agrego ruta para los audios
    add_definitions(-DSOUNDS_PATH="${PROJECT_SOURCE_DIR}/client/sounds")

    # Agrego ruta para los sprites
    add_definitions(-DSPRITE_PATH="${PROJECT_SOURCE_DIR}/client/sprites")

    # Agrego ruta para los sprites
    add_definitions(-DARMORS_PATH="${PROJECT_SOURCE_DIR}/client/armors")

    # Agrego ruta para los sprites
    add_definitions(-DHELMETS_PATH="${PROJECT_SOURCE_DIR}/client/helmets")

    # Agrego ruta para el fondo
    add_definitions(-DBKGR_PATH="${PROJECT_SOURCE_DIR}/client/background")

    # Agrego ruta para las armas
    add_definitions(-DWEAPON_PATH="${PROJECT_SOURCE_DIR}/client/weapons")

    # Agrego ruta para la animacion de media partida
    add_definitions(-DMIDDLE_PATH="${PROJECT_SOURCE_DIR}/client/middle_animation") 

    # Agrego ruta para las fuentes
    add_definitions(-DFONTS_PATH="${PROJECT_SOURCE_DIR}/client/fonts")

    # Agrego ruta para el tipo de letra
    add_definitions(-DFONT_PATH="${PROJECT_SOURCE_DIR}/client/font") 

    # Agrego ruta para el tipo de letra
    add_definitions(-DCONFIG_PATH="${PROJECT_SOURCE_DIR}/config") 

    set_project_warnings(taller_client ${TALLER_MAKE_WARNINGS_AS_ERRORS} FALSE)

    # add libSDL2pp's headers
    target_include_directories(taller_client PRIVATE ${libSDL2pp_SOURCE_DIR} Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Core)

    # Link the taller_common lib target
    target_link_libraries(taller_client taller_common lobby init SDL2pp::SDL2pp Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Core)
endif()

if(TALLER_SERVER)
    # HEY!! TODO XXX: Change the name of the executable to the one you like!
    add_executable(taller_server)

    # Make them depend on taller_common lib
    add_dependencies(taller_server taller_common)

    # Agrego ruta para el tipo de letra
    add_definitions(-DCONFIG_PATH="${PROJECT_SOURCE_DIR}/config") 

    # Add source files and enable warnings
    # HEY!! TODO XXX: add more subfolders here
    add_subdirectory(server)

    set_project_warnings(taller_server ${TALLER_MAKE_WARNINGS_AS_ERRORS} FALSE)

    # Link the taller_common lib target
    target_link_libraries(taller_server yaml-cpp taller_common)
endif()

if(TALLER_EDITOR)
    # HEY!! TODO XXX: Change the name of the executable to the one you like!
    add_executable(taller_editor)

    # Make them depend on taller_common lib
    add_dependencies(taller_editor taller_common SDL2pp::SDL2pp)

    # Add source files and enable warnings
    # HEY!! TODO XXX: add more subfolders here
    add_subdirectory(editor)

    set_project_warnings(taller_editor ${TALLER_MAKE_WARNINGS_AS_ERRORS} FALSE)

    # add libSDL2pp's headers
    target_include_directories(taller_editor PRIVATE ${libSDL2pp_SOURCE_DIR})

    # Link the taller_common lib target
    target_link_libraries(taller_editor taller_common SDL2pp::SDL2pp)
endif()


# Testing section
# ---------------

if(TALLER_TESTS)
    include_directories(${GTEST_INCLUDE_DIRS} ${GMOCK_INCLUDE_DIRS})

    # Let's use google test
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    )

    FetchContent_Declare(
        googlemock
        URL https://github.com/google/googlemock/archive/master.zip
    )

    # For Windows: Prevent overriding the parent project's compiler/linker settings
    # (borrowed from https://google.github.io/googletest/quickstart-cmake.html)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest googlemock)
    include(GoogleTest)

    # target for testing
    # HEY!! TODO XXX: Change the name of the executable to the one you like!
    add_executable(taller_tests)

    target_sources(taller_tests
        PRIVATE
        ${PROJECT_SOURCE_DIR}/server/lobbyProtocol.cpp
        ${PROJECT_SOURCE_DIR}/client/lobbyProtocol.cpp
    )

    # HEY!! TODO XXX: I'm assuming that the code under testing is in taller_common
    # Change this if you have to.
    add_dependencies(taller_tests taller_common)

    # Say where to find the xoz headers
    # HEY!! TODO XXX: add more subfolders here
    target_include_directories(taller_tests PUBLIC . ${PROJECT_SOURCE_DIR}/server ${PROJECT_SOURCE_DIR}/client)

    # Add the source files to the taller_tests target
    # HEY!! TODO XXX: add more subfolders here
    add_subdirectory(tests/)

    # Set the compiler warnings to the tests target (but be a little lesser strict)
    set_project_warnings(taller_tests ${TALLER_MAKE_WARNINGS_AS_ERRORS} TRUE)

    # Say where googletest headers can be found
    target_include_directories(taller_tests
        PUBLIC
        ${gtest_SOURCE_DIR}/include
        ${gmock_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/server
        ${PROJECT_SOURCE_DIR}/client
        )

    # HEY!! TODO XXX: I'm assuming that the code under testing is in taller_common
    # Change this if you have to.
    target_link_libraries(taller_tests
        taller_common
        GTest::gtest_main
        GTest::gtest_main
        GTest::gmock
        GTest::gmock_main
    )
endif()
