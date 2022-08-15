#
# base library
#
file(GLOB_RECURSE SRCS
        rc/*.cpp
        )
file(GLOB_RECURSE HDRS
        rc/*.h
        )
add_library(${CMAKE_PROJECT_NAME}_lib
        STATIC
        ${SRCS} ${HDRS})
target_include_directories(${CMAKE_PROJECT_NAME}_lib PUBLIC rc)

# ----==== third party ====----
# OpenGL
find_package(OpenGL REQUIRED)
message(STATUS "Found OpenGL in ${OPENGL_INCLUDE_DIR}")
target_link_libraries(${CMAKE_PROJECT_NAME}_lib PRIVATE ${OPENGL_LIBRARIES})
message(STATUS "OpenGL Libraries: ${OPENGL_LIBRARIES}")

# GLUT
find_package(GLUT REQUIRED)
message(STATUS "Found Glut version ${GLUT_VERSION_STRING} in ${GLUT_INCLUDE_DIR}")
target_link_libraries(${CMAKE_PROJECT_NAME}_lib PRIVATE ${GLUT_LIBRARIES})
message(STATUS "freeglut Libraries: ${GLUT_LIBRARY}")

# json
find_package(nlohmann_json 3.11.1 REQUIRED)
message(STATUS "Found nlohmann")
# header-only

# libpng
find_package(PNG REQUIRED)
message(STATUS "Found libPng version ${PNG_VERSION_STRING} in ${PNG_INCLUDE_DIR}")
target_link_libraries(${CMAKE_PROJECT_NAME}_lib PRIVATE ${PNG_LIBRARIES})

# magic_enum
target_include_directories(${CMAKE_PROJECT_NAME}_lib PUBLIC ${MAGIC_ENUM_INCLUDE_DIR})
message(STATUS "Found magic_enum in ${MAGIC_ENUM_INCLUDE_DIR}")
# header-only
#
#  Main Executable
#
add_executable(${CMAKE_PROJECT_NAME} main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME}
        ${CMAKE_PROJECT_NAME}_lib
        )