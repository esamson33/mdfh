project(mdfh_common)

set(SOURCES message.hpp message.cpp)

# all sources that need to be tested in unit tests go into a static library
add_library(mdfh_common STATIC ${SOURCES})
target_include_directories(mdfh_common INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/..)
message( STATUS TAE ${INCLUDE_DIRECTORIES} )