project(mdfh_client)

set(SOURCES main.cpp
        udp_receiver.hpp udp_receiver.cpp)
add_executable(client ${SOURCES})
IF(APPLE)
    target_link_libraries(client mdfh_common boost_system-mt boost_thread-mt pthread)
ELSE()
    target_link_libraries(client mdfh_common boost_system boost_thread pthread)
ENDIF()
set_target_properties(client PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
