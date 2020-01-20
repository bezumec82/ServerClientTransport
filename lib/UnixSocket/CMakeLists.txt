cmake_minimum_required( VERSION 3.14 )

set( PROJECT_NAME UnixSocketTransport )
###########
### Library
###########
message( "Configuring library : " ${PROJECT_NAME} )
file( GLOB CXX_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)
add_library(
    ${PROJECT_NAME}
STATIC
    ${CXX_FILES}
)

target_include_directories(
    ${PROJECT_NAME}
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
    ${PROJECT_NAME}
LINK_PRIVATE
    Tools
)

########
### Test
########
message( "Configuring example : " ${PROJECT_NAME}Test.out )
include_directories( ${Boost_INCLUDE_DIR} )
message( "Located boost libs : " ${Boost_LIBRARIES} )
file( GLOB CXX_FILES ${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp )
add_executable(
    ${PROJECT_NAME}Test.out
    ${CXX_FILES}
)
target_link_libraries(
    ${PROJECT_NAME}Test.out
LINK_PRIVATE
    ${PROJECT_NAME}
    Tools
    ${Boost_LIBRARIES}
)