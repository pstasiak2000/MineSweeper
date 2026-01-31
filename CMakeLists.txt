cmake_minimum_required(VERSION 3.10)
project(MineSweeper C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

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
)

target_include_directories(MineSweeper PRIVATE
    src
    include
    ${GTK4_INCLUDE_DIRS}
)

target_compile_options(MineSweeper PRIVATE ${GTK4_CFLAGS_OTHER})
target_link_libraries(MineSweeper PRIVATE timer gui game_engine ${GTK4_LIBRARIES})
