project(mdfh_common)

set(SOURCES message.hpp message.cpp market_data/order.cpp market_data/order.hpp
        system_message.hpp system_message.cpp message.hpp)

# all sources that need to be tested in unit tests go into a static library
add_library(mdfh_common STATIC ${SOURCES})
target_include_directories(mdfh_common INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/..)
