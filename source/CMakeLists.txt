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

# OpenGL
find_package(OpenGL REQUIRED)

# GLUT
find_package(GLUT REQUIRED)
message(STATUS "Found Glut in ${GLUT_INCLUDE_DIR}")
target_link_libraries(${CMAKE_PROJECT_NAME}_lib opengl32 glu32 freeglut)

#
#  Main Executable
#
add_executable(${CMAKE_PROJECT_NAME} main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME}
        ${CMAKE_PROJECT_NAME}_lib
        )