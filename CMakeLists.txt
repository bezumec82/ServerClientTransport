cmake_minimum_required( VERSION 2.6 )
project( ServerClientTransport )

# Version number
set( ServerClientTransport_VERSION_MAJOR 1 )
set( ServerClientTransport_VERSION_MINOR 0 )

set( CMAKE_SYSTEM_NAME      Linux )
set( CMAKE_SYSTEM_PROCESSOR arm )


# Boost
message("Provided BOOSTROOT : " ${BOOSTROOT} )
find_package ( Boost 1.71.0 )
include_directories ( ${Boost_INCLUDE_DIR} )
message( "Found boost : " ${Boost_INCLUDE_DIR} )

########
### Test
########
add_executable( ${CMAKE_PROJECT_NAME}Test.out ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp )

#############
### Libraries
#############
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/lib )