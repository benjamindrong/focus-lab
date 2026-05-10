cmake_minimum_required(VERSION 3.26)
project(FocusLab)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

add_executable(FocusLab main.cpp
        core/GameSession.cpp
        core/GameSession.h
        core/GameState.cpp
        core/GameState.h
        core/Input.cpp
        core/Input.h
        core/metrics/Metrics.h
        core/games/IGame.h
        core/games/ReactionGame.h)

target_link_libraries(FocusLab SFML::Graphics SFML::Window SFML::System)