cmake_minimum_required(VERSION 3.26)
project(FocusLab)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

add_executable(
        FocusLab
        cpp-app/main.cpp
        core/games/MemoryGame.h
)

target_link_libraries(FocusLab SFML::Graphics SFML::Window SFML::System)