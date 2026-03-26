set(CMAKE_C_STANDARD 11)
cmake_minimum_required(VERSION 3.10)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)


### --- VERSIONING FROM GIT ---
find_package(Git QUIET)
 
if(GIT_FOUND)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} describe --tags --abbrev=0
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_TAG
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
else()
    set(GIT_TAG "v0.0.0")
endif()

### -- Set the project version and date strings
string(REGEX REPLACE "^v" "" PROJECT_VERSION "${GIT_TAG}")
string(TIMESTAMP PROJECT_DATE "%Y-%m-%d")

configure_file(
    ${CMAKE_SOURCE_DIR}/include/version.h.in
    ${CMAKE_BINARY_DIR}/generated/version.h
    @ONLY
)

project(MineSweeper
        VERSION ${PROJECT_VERSION}
        LANGUAGES C)

message(STATUS "Building MineSweeper version ${PROJECT_VERSION}")

# --- Set the developer tools here --- 
option(COMPILE_WITH_DEV_TOOLS
    "Compile with developer tools enabled"
    FALSE)

configure_file(
    ${CMAKE_SOURCE_DIR}/include/devtools.h.in
    ${CMAKE_BINARY_DIR}/generated/devtools.h
    @ONLY
)

# Use pre-generated resources.c in src/
set(RESOURCES_C "${CMAKE_CURRENT_SOURCE_DIR}/src/resources.c")


# --- Libraries ---
add_library(timer STATIC src/timer/timer.c)
add_library(gui STATIC src/gui/gui.c)
add_library(game_engine STATIC src/game/game_engine.c)
add_library(ldbd STATIC src/ldbd/ldbd.c)

target_include_directories(timer PUBLIC
    src/timer
    ${GTK4_INCLUDE_DIRS}
)

target_include_directories(gui PUBLIC
    src/gui
    ${GTK4_INCLUDE_DIRS}
)

target_include_directories(game_engine PUBLIC
    src/game
    ${GTK4_INCLUDE_DIRS}  
)


target_include_directories(ldbd PUBLIC
    src/ldbd
    ${GTK4_INCLUDE_DIRS}
)

target_compile_options(timer PUBLIC ${GTK4_CFLAGS_OTHER})
target_link_libraries(timer PUBLIC ${GTK4_LIBRARIES})

target_compile_options(gui PUBLIC ${GTK4_CFLAGS_OTHER})
target_link_libraries(gui PUBLIC ${GTK4_LIBRARIES})

target_compile_options(game_engine PUBLIC ${GTK4_CFLAGS_OTHER})
target_link_libraries(game_engine PUBLIC ${GTK4_LIBRARIES})


target_compile_options(ldbd PUBLIC ${GTK4_CFLAGS_OTHER})
target_link_libraries(ldbd PUBLIC ${GTK4_LIBRARIES})

# --- Executable ---
add_executable(MineSweeper 
    src/main.c
    src/base.c
    src/actions.c
    src/toolbar.c
    ${RESOURCES_C}
)

# 🔑 Generate resources.c BEFORE building the target
add_custom_command(
	TARGET MineSweeper PRE_BUILD
    COMMAND glib-compile-resources
            --sourcedir=${CMAKE_CURRENT_SOURCE_DIR}/resources
            --generate-source
            --target=${RESOURCES_C}
            ${CMAKE_CURRENT_SOURCE_DIR}/resources/resources.gresource.xml
    COMMENT "Generating resources.c"
)

target_include_directories(MineSweeper PRIVATE
    src
    include
    ${CMAKE_BINARY_DIR}/generated
    ${GTK4_INCLUDE_DIRS}
)

target_compile_options(MineSweeper PRIVATE ${GTK4_CFLAGS_OTHER})
target_link_libraries(MineSweeper PRIVATE timer gui game_engine ldbd ${GTK4_LIBRARIES})

### --- Install the executable ---
install(TARGETS MineSweeper
    RUNTIME DESTINATION bin
)

### --- Install the .desktop file ---
install(FILES
    ${CMAKE_SOURCE_DIR}/data/org.example.MineSweeper.desktop
    DESTINATION share/applications
)

### --- Install AppStream metadata ---
configure_file(
    ${CMAKE_SOURCE_DIR}/data/org.example.MineSweeper.appdata.xml.in
    ${CMAKE_BINARY_DIR}/org.example.MineSweeper.appdata.xml
    @ONLY
)

install(FILES
    ${CMAKE_BINARY_DIR}/org.example.MineSweeper.appdata.xml
    DESTINATION share/metainfo
)


### --- Install scalable icon ---
install(FILES
    ${CMAKE_SOURCE_DIR}/resources/icons/org.example.MineSweeper.svg
    DESTINATION share/icons/hicolor/scalable/apps
)
