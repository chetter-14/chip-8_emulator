include_directories("${Chip-8_emulator_SOURCE_DIR}/include")
add_executable(Main main.cpp chip8.cpp)

set(CMAKE_C_COMPILER /c/cygwin64/bin/gcc)
set(CMAKE_CXX_COMPILER /c/cygwin64/bin/g++)

target_link_libraries(Main PRIVATE "${Chip-8_emulator_SOURCE_DIR}/lib/freeglutd.lib")