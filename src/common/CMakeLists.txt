project(mdfh_common)

# all sources that need to be tested in unit tests go into a static library
add_library(mdfh_common STATIC order_book.cpp order_book.hpp)
target_include_directories(mdfh_common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

