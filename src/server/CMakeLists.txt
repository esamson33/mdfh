project(mdfh_server)

set(SOURCES
        main.cpp 
        udp_multicast.hpp udp_multicast.cpp
        system_message.hpp system_message.cpp message.cpp message.h)

add_executable(server ${SOURCES})
IF(APPLE)
    target_link_libraries(server boost_system-mt boost_thread-mt boost_serialization-mt pthread)
ELSE()
    target_link_libraries(server boost_system boost_thread boost_serialization pthread)
ENDIF()
set_target_properties(server PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")