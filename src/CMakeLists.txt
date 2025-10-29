# This file was automatically generated for projects
# without default 'CMakeLists.txt' file.

FILE(GLOB_RECURSE app_sources
    ${CMAKE_SOURCE_DIR}/src/*.c
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

FILE(GLOB_RECURSE elecrow_sources
    ${CMAKE_SOURCE_DIR}/elecrow/*.c
)

set(project_includes
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/elecrow
)

idf_component_register(
    SRCS ${app_sources} ${elecrow_sources}
    INCLUDE_DIRS ${project_includes}
)
