#
#  UI Library
#  ==========
include(../cmake/qt.cmake)
#
# UI Lib
#
file(GLOB_RECURSE SRCS_UI
        we/*
        )
file(GLOB_RECURSE SRCS_RESOURCE
        resources/*.*rc
        )

add_executable(${CMAKE_PROJECT_NAME}_world_editor main.cpp ${SRCS_UI} ${SRCS_RESOURCE})
if(${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    target_compile_options(${CMAKE_PROJECT_NAME}_world_editor PRIVATE -Wno-reserved-identifier)
endif()
target_include_directories(${CMAKE_PROJECT_NAME}_world_editor PRIVATE we)
target_link_libraries(${CMAKE_PROJECT_NAME}_world_editor
        ${CMAKE_PROJECT_NAME}_lib
        )

qt_attach_to_target(${CMAKE_PROJECT_NAME}_world_editor)
#
#  Main Executable
#
