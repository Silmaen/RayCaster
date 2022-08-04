cmake_minimum_required(VERSION 3.23)

#
# ---=== Initializations ===---
#
project(RayCaster VERSION 0.0.1 LANGUAGES CXX)
set(PRJPREFIX RAYCAST)
set(PRJPREFIX_LOWER raycast)
set(${PRJPREFIX}_ROOT_DIR "${PROJECT_SOURCE_DIR}")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#
# ---=== Base Configs ===---
#
include(cmake/BaseConfig.cmake)
#
# ---=== Dependencies ===---
#
add_subdirectory(external)
#
# ---=== the main sources ===---
#
add_subdirectory(source)
#
# ---=== TESTING PROCEDURES ===---
#
enable_testing()
add_subdirectory(test)
