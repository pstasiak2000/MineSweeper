cmake_minimum_required(VERSION 3.10)
project(MineSweeper C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

# Use pre-generated resources.c in src/
set(RESOURCES_C "${CMAKE_CURRENT_SOURCE_DIR}/src/resources.c")

# add_custom_command(
#     OUTPUT ${RESOURCES_C}
#     COMMAND glib-compile-resources
#             --sourcedir=${CMAKE_CURRENT_SOURCE_DIR}/resources
#             --target=${RESOURCES_C}
#             --generate-source
#             ${CMAKE_CURRENT_SOURCE_DIR}/resources/resources.gresource.xml
#     DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/resources/resources.gresource.xml
#     COMMENT "Compiling GResources..."
# )

# --- Timer library ---
add_library(timer STATIC src/timer/timer.c)
add_library(gui STATIC src/gui/gui.c)
add_library(game_engine STATIC src/game/game_engine.c)

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


target_compile_options(timer PUBLIC ${GTK4_CFLAGS_OTHER})
target_link_libraries(timer PUBLIC ${GTK4_LIBRARIES})

target_compile_options(gui PUBLIC ${GTK4_CFLAGS_OTHER})
target_link_libraries(gui PUBLIC ${GTK4_LIBRARIES})

target_compile_options(game_engine PUBLIC ${GTK4_CFLAGS_OTHER})
target_link_libraries(game_engine PUBLIC ${GTK4_LIBRARIES})

# --- Executable ---
add_executable(MineSweeper 
    src/main.c
    src/base.c
    src/actions.c
    src/toolbar.c
    ${RESOURCES_C}
)

target_include_directories(MineSweeper PRIVATE
    src
    include
    ${GTK4_INCLUDE_DIRS}
)

target_compile_options(MineSweeper PRIVATE ${GTK4_CFLAGS_OTHER})
target_link_libraries(MineSweeper PRIVATE timer gui game_engine ${GTK4_LIBRARIES})


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
install(FILES
    ${CMAKE_SOURCE_DIR}/data/org.example.MineSweeper.appdata.xml
    DESTINATION share/metainfo
)


### --- Install scalable icon ---
install(FILES
    ${CMAKE_SOURCE_DIR}/resources/icons/org.example.MineSweeper.svg
    DESTINATION share/icons/hicolor/scalable/apps
)
