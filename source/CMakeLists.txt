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
message(STATUS "Found OpenGL")
target_link_libraries(${CMAKE_PROJECT_NAME}_lib PRIVATE ${OPENGL_LIBRARIES})


# GLUT
find_package(GLUT REQUIRED)
message(STATUS "Found Glut")
target_link_libraries(${CMAKE_PROJECT_NAME}_lib PRIVATE ${GLUT_LIBRARIES})

# json
find_package(nlohmann_json 3.11.2 REQUIRED)
message(STATUS "Found nlohmann version ${nlohmann_json_VERSION} in ${nlohmann_json_DIR}")
# header-only

#fmt
find_package(fmt 8.1 REQUIRED)
message(STATUS "Found fmt version ${fmt_VERSION} in ${fmt_DIR}")
target_link_libraries(${CMAKE_PROJECT_NAME}_lib PUBLIC fmt)

#spdlog
find_package(spdlog 1.9 REQUIRED)
message(STATUS "Found spdlog version ${spdlog_VERSION} in ${spdlog_DIR}")
target_link_libraries(${CMAKE_PROJECT_NAME}_lib PUBLIC spdlog)
target_compile_definitions(${CMAKE_PROJECT_NAME}_lib PUBLIC SPDLOG_FMT_EXTERNAL)

# libpng
find_package(PNG 1.6 REQUIRED)
message(STATUS "Found libPng version ${PNG_VERSION_STRING} in ${PNG_INCLUDE_DIR}")
target_link_libraries(${CMAKE_PROJECT_NAME}_lib PRIVATE ${PNG_LIBRARIES})

# tbb for parallel ranges
find_package(TBB QUIET)
if (${TBB_FOUND})
    message(STATUS "Found TBB version ${TBB_VERSION} in ${TBB_DIR}")
    #dump_cmake_variables()
    if (CMAKE_SYSTEM_NAME MATCHES "Linux")
        target_link_libraries(${CMAKE_PROJECT_NAME}_lib PRIVATE tbb)
    elseif(MINGW)
        target_link_libraries(${CMAKE_PROJECT_NAME}_lib PRIVATE tbb12)
    endif ()
else ()
    message(NOTICE "Optional: better parallel performance can be obtain by installing intel tbb")
endif ()

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
