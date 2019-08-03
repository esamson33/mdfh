cmake_minimum_required(VERSION 3.10.2)
project(mdfh)

set(CMAKE_BUILD_TYPE Debug)
set(GCC_COMPILE_FLAGS "-Wunused")
set(MDFH_COMPILE_FLAGS "-DMDFH_DEV")
add_definitions(${GCC_COMPILE_FLAGS} ${MDFH_COMPILE_FLAGS})

find_package( Boost 1.66 REQUIRED COMPONENTS system thread serialization)
include_directories( ${Boost_INCLUDE_DIRS} )
IF(APPLE)
    LINK_DIRECTORIES(/opt/local/lib)
ENDIF()

#add_subdirectory(src/common)
add_subdirectory(src/server)
add_subdirectory(src/client)
