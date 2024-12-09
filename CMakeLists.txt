cmake_minimum_required(VERSION 3.29)
project(CrowHelp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "D:/Games/samp/gtasa")

add_subdirectory(external/samp-api)

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/external/samp-api/include)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp" "src/*.h")

add_library(CrowHelp SHARED ${SOURCES}
        src/tools/helpers/ChatInput.h)

target_link_libraries(CrowHelp PRIVATE sampapi)
target_include_directories(CrowHelp PRIVATE external/samp-api/include)

add_custom_command(
        TARGET CrowHelp
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E rename "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/CrowHelp.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/crowhelp.asi"
)
