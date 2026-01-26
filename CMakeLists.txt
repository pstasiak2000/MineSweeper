cmake_minimum_required(VERSION 3.10)
project(MineSweeper C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

# --- Timer library ---
add_library(timer STATIC src/timer/timer.c)
target_include_directories(timer PUBLIC
    src/timer
    ${GTK4_INCLUDE_DIRS}
)
target_compile_options(timer PUBLIC ${GTK4_CFLAGS_OTHER})
target_link_libraries(timer PUBLIC ${GTK4_LIBRARIES})

# --- Executable ---
add_executable(MineSweeper 
    src/main.c
    src/actions.c
    src/game_engine.c
    src/gui.c
    src/toolbar.c
)

target_include_directories(MineSweeper PRIVATE
    src
    include
    ${GTK4_INCLUDE_DIRS}
)
target_compile_options(MineSweeper PRIVATE ${GTK4_CFLAGS_OTHER})
target_link_libraries(MineSweeper PRIVATE timer ${GTK4_LIBRARIES})
