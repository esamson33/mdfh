project(mdfh_client)

add_executable(client main.cpp udp_receiver.hpp udp_receiver.cpp)
IF(APPLE)
    target_link_libraries(client boost_system-mt boost_thread-mt pthread)
ELSE()
    target_link_libraries(client boost_system boost_thread pthread)
ENDIF()
set_target_properties(client PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
