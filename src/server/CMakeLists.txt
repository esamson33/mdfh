project(mdfh_server)

find_package( Boost 1.69 REQUIRED system thread serialization)
include_directories( ${Boost_INCLUDE_DIRS} )

#add_library(mdfh_server STATIC udp_multicast.hpp)
#target_include_directories(mdfh_server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

#set(GCC_COMPILE_FLAGS "-Wunused")
#add_definitions(${GCC_COMPILE_FLAGS})

set(SOURCES
        main.cpp 
        udp_multicast.hpp udp_multicast.cpp
        system_message.hpp system_message.cpp message.cpp message.h)

add_executable(server ${SOURCES})
#target_link_libraries(server PRIVATE mdfh_server)
#target_link_libraries(server PRIVATE ${Boost_LIBRARIES})
target_link_libraries(server boost_system boost_thread boost_serialization pthread)
set_target_properties(server PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin") 

