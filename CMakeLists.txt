cmake_minimum_required(VERSION 3.10.0)
project(OsuTagDivider VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)

enable_testing()

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE TEST_SOURCES ${CMAKE_SOURCE_DIR}/tests/*.cpp)
file(COPY ${CMAKE_SOURCE_DIR}/tests/test_data DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR}/AppDir/usr/share/OsuTagDivider)
file(COPY ${CMAKE_SOURCE_DIR}/app_data/OsuTagDivider.desktop DESTINATION ${CMAKE_BINARY_DIR}/AppDir/usr/share/applications)

include_directories(${GTK3_INCLUDE_DIRS} ${GTKMM_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS} ${GTKMM_LIBRARY_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER})

include_directories(${CMAKE_SOURCE_DIR}/include)
file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_library(sources ${SOURCES})

if (WIN32)
    set(APP_ICON ${CMAKE_SOURCE_DIR}/app_data/appico.rc)
    add_executable(OsuTagDivider WIN32 ${APP_ICON} ${CMAKE_SOURCE_DIR}/main.cpp)
else()
    add_executable(OsuTagDivider ${CMAKE_SOURCE_DIR}/main.cpp)
endif()

target_link_libraries(${PROJECT_NAME} sources ${GTK3_LIBRARIES} ${GTKMM_LIBRARIES})

add_executable(UnitTests ${TEST_SOURCES})
target_link_libraries(UnitTests sources gtest_main ${GTK3_LIBRARIES} ${GTKMM_LIBRARIES})

add_test(NAME UnitTests COMMAND UnitTests)
