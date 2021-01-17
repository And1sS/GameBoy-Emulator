cmake_minimum_required(VERSION 3.10)

project(GameBoy-Emulator)

set(CMAKE_CXX_STANDARD 20)
set(SFML_STATIC_LIBRARIES true)
set(SFML_DIR E:/SFML-2.5.1/lib/cmake/SFML)

file(GLOB HEADERS CONFIGURE_DEPENDS *.h)
file(GLOB SOURCES CONFIGURE_DEPENDS *.cpp)
file(GLOB CARTRIDGES CONFIGURE_DEPENDS Cartridges/*)
file(GLOB SOUND CONFIGURE_DEPENDS Sound/*)

source_group(Headers FILES ${HEADERS})
source_group(Sources FILES ${SOURCES})
source_group(Cartridges FILES ${CARTRIDGES})
source_group(Sound FILES ${SOUND})

find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)
add_executable(GameBoy-Emulator ${HEADERS} ${SOURCES} ${CARTRIDGES} ${SOUND})
target_link_libraries(GameBoy-Emulator sfml-graphics sfml-system sfml-window sfml-audio)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT GameBoy-Emulator)