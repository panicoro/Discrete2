cmake_minimum_required(VERSION 3.10)

# about this project
# ..................
project(MatDiscreta2019 C)

set(CMAKE_C_STANDARD 99)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Wextra -std=c99 -O3 -DNDEBUG
-IWhalaan -g")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Whalaan)

add_executable(MatDiscreta2019
        Whalaan/main.c
        Whalaan/Carga.c
        Whalaan/Coloreo.c
        Whalaan/Construye.c
        Whalaan/Hash.c
        Whalaan/Info.c
        Whalaan/Lista.c
        Whalaan/Ordenacion.c
        Whalaan/U32ToString.c
        Whalaan/TestOrdenacion.c
        Whalaan/TestOrdenacion.h
        Whalaan/TestMain.c
        Whalaan/TestMain.h)
    
