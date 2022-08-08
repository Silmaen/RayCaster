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

# GLUT
find_package(GLUT REQUIRED)

# json
find_package(nlohmann_json 3.2.0 REQUIRED)

message(STATUS "Found Glut in ${GLUT_INCLUDE_DIR}")
if (WIN32)
    target_link_libraries(${CMAKE_PROJECT_NAME}_lib PRIVATE opengl32 glu32 freeglut)
else ()
    target_link_libraries(${CMAKE_PROJECT_NAME}_lib PRIVATE GL GLU glut)
endif ()
#target_link_libraries(${CMAKE_PROJECT_NAME}_lib PRIVATE nlohmann_json)
#
#  Main Executable
#
add_executable(${CMAKE_PROJECT_NAME} main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME}
        ${CMAKE_PROJECT_NAME}_lib
        )