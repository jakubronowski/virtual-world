cmake_minimum_required(VERSION 3.20)

project(VirtualWorld
    VERSION 1.0.0
    DESCRIPTION "Turn-based 2D simulation"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile_commands.json for clangd / IDEs
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Core simulation static library
set(CORE_SOURCES
    Position.h
    Exceptions.h
    Random.h
    Organism.cpp
    Organism.h
    Animal.cpp
    Animal.h
    Plant.cpp
    Plant.h
    OrganismFactory.cpp
    OrganismFactory.h
    WorldRenderer.cpp
    WorldRenderer.h
    World.cpp
    World.h
    Animals/Wolf.cpp
    Animals/Wolf.h
    Animals/Sheep.cpp
    Animals/Sheep.h
    Animals/Fox.cpp
    Animals/Fox.h
    Animals/Hare.cpp
    Animals/Hare.h
    Animals/Mosquito.cpp
    Animals/Mosquito.h
    Plants/Grass.cpp
    Plants/Grass.h
    Plants/Guarana.cpp
    Plants/Guarana.h
    Plants/Thorn.cpp
    Plants/Thorn.h
)

add_library(VirtualWorldCore STATIC ${CORE_SOURCES})

target_include_directories(VirtualWorldCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Animals
    ${CMAKE_CURRENT_SOURCE_DIR}/Plants
)

# Compiler warning flags
if(MSVC)
    target_compile_options(VirtualWorldCore PRIVATE /W4 /permissive-)
else()
    target_compile_options(VirtualWorldCore PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Main executable
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE VirtualWorldCore)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# ==============================================================================
# Testing with GoogleTest via FetchContent
# ==============================================================================
option(BUILD_TESTING "Enable test target build" ON)

if(BUILD_TESTING)
    enable_testing()

    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )
    # Prevent overriding parent project compiler/linker settings on Windows
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(googletest)

    add_executable(VirtualWorldTests tests/Tests.cpp)
    target_link_libraries(VirtualWorldTests PRIVATE VirtualWorldCore GTest::gtest_main)

    include(GoogleTest)
    gtest_discover_tests(VirtualWorldTests)
endif()
