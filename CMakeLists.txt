cmake_minimum_required( VERSION 2.6 )
project( Transport )

set( CMAKE_DISABLE_IN_SOURCE_BUILD ON )
set( CMAKE_DISABLE_SOURCE_CHANGES  ON )

if( "${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}" )
  message( SEND_ERROR "In-source builds are not allowed." )
endif ()

# set( CMAKE_VERBOSE_MAKEFILE ON )
set( CMAKE_COLOR_MAKEFILE ON )

# Boost
message( "Provided BOOSTROOT : " ${BOOSTROOT} )
find_package( Boost 1.71.0 )
include_directories( ${Boost_INCLUDE_DIR} )
message( "Found boost : " ${Boost_INCLUDE_DIR} )

########
### Test
########
add_executable(
    ${CMAKE_PROJECT_NAME}Test.out
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp 
)

#############
### Libraries
#############
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/lib )