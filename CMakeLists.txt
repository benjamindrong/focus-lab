cmake_minimum_required(VERSION 3.31)
project(FocusLab)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

add_executable(
        FocusLab
        cpp-app/main.cpp
)

target_link_libraries(
        FocusLab
        PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
)

# ---------------- TESTING ----------------

enable_testing()

find_package(Catch2 3 REQUIRED)

add_executable(
        FocusLabTests
        tests/ReactionGameTests.cpp
        tests/MemoryGameTests.cpp
        tests/GameSessionTests.cpp
)

target_include_directories(
        FocusLabTests PRIVATE .
)

target_link_libraries(
        FocusLabTests
        PRIVATE Catch2::Catch2WithMain
)

include(CTest)
include(Catch)
catch_discover_tests(FocusLabTests)

target_compile_definitions(FocusLabTests PRIVATE TESTING)