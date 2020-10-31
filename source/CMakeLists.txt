cmake_minimum_required(VERSION 3.10)

project(GameBoy-Emulator)

set(SFML_STATIC_LIBRARIES true)

set(SFML_DIR E:/SFML-2.5.1/lib/cmake/SFML)

find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)
add_executable(GameBoy-Emulator main.cpp APU.cpp CPU.cpp Memory.cpp PPU.cpp Timer.cpp Sound/ToneGenerator.cpp)
target_link_libraries(GameBoy-Emulator sfml-graphics sfml-system sfml-window sfml-audio)