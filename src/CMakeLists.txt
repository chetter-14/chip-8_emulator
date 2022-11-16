include_directories("${Chip-8_emulator_SOURCE_DIR}/include")
include_directories("C:/cygwin64/usr/include/GL")
add_executable(Main main.cpp)

set(CMAKE_C_COMPILER /c/cygwin64/bin/gcc)
set(CMAKE_CXX_COMPILER /c/cygwin64/bin/g++)