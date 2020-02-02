cmake_minimum_required( VERSION 3.14 )

if(NOT WIN32)
  string(ASCII 27 ESC)
  set( NORM "${ESC}[39m" )
  set( RED  "${ESC}[31m" ) 
  set( GRN  "${ESC}[32m" )
  set( CYN  "${ESC}[36m" )
  set( MAG  "${ESC}[35m" )
  set( YEL  "${ESC}[33m" )
  set( BLU  "${ESC}[34m" )
endif()

set( CMAKE_DISABLE_IN_SOURCE_BUILD ON )
set( CMAKE_DISABLE_SOURCE_CHANGES  ON )

if( "${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}" )
  message( SEND_ERROR "In-source builds are not allowed." )
endif ()

project( Transport )

# set( CMAKE_VERBOSE_MAKEFILE ON )
# set( CMAKE_COLOR_MAKEFILE ON )

# Boost
message( "${YEL}Provided BOOSTROOT : ${BOOSTROOT}${NORM}" )
find_package( Boost 1.71.0 )
include_directories( ${Boost_INCLUDE_DIRS} )
message( "${GRN}Found boost : ${Boost_INCLUDE_DIRS}${NORM}" )

########
### Test
########
add_executable(
  ${PROJECT_NAME}Test.out
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

#############
### Libraries
#############
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/lib )