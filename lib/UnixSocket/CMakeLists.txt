###########
### Library
###########
add_library ( UnixSocketTransport STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/UnixSocket.cpp
)

target_include_directories (
    UnixSocketTransport
PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries (
    UnixSocketTransport
LINK_PRIVATE
    Tools
)

########
### Test
########
add_executable(UnixSocketTransportTest.out 
    ${CMAKE_CURRENT_SOURCE_DIR}/test/main.cpp 
)
target_link_libraries (
    UnixSocketTransportTest.out
LINK_PRIVATE
    UnixSocketTransport
    Tools
)
