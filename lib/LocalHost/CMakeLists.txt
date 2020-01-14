cmake_minimum_required( VERSION 2.6 )
set( PROJECT_NAME LocalHost )
###########
### Library
###########
message( "Preparing library : " ${PROJECT_NAME} )
file(GLOB CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(
    ${PROJECT_NAME}
STATIC
    ${CPP_FILES}
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
message( "Preparing executable : " ${PROJECT_NAME}Test.out )
add_executable(
    ${PROJECT_NAME}Test.out 
    ${CMAKE_CURRENT_SOURCE_DIR}/test/main.cpp 
)
target_link_libraries (
    ${PROJECT_NAME}Test.out
LINK_PRIVATE
    ${PROJECT_NAME}
    Tools
)
