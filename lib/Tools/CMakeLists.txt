cmake_minimum_required( VERSION 2.6 )
set( PROJECT_NAME Tools )

# The version number.
set( ${CMAKE_PROJECT_NAME}_VERSION_MAJOR 1 )
set( ${CMAKE_PROJECT_NAME}_VERSION_MINOR 0 )

###########
### Library
###########
message( "Preparing library : " ${PROJECT_NAME} )
add_library( ${PROJECT_NAME} INTERFACE )
target_include_directories(
    ${PROJECT_NAME}
INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
