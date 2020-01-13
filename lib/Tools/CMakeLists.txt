cmake_minimum_required (VERSION 2.6)
project (Tools)

# The version number.
set (Tools_VERSION_MAJOR 1)
set (Tools_VERSION_MINOR 0)

###########
### Library
###########
add_library (Tools INTERFACE)
target_include_directories(
    Tools 
INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
