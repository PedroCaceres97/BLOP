cmake_minimum_required(VERSION 3.15)
project(BLOP VERSION 1.0.0 LANGUAGES C)

# Opciones
option(BUILD_SHARED_LIBS "Build BLOP as a shared library" OFF)
option(BLOP_BUILD_EXAMPLES "Build example programs" OFF)

# Directorios principales
set(BLOP_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(BLOP_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Agregar los headers como un include público
include_directories(${BLOP_INCLUDE_DIR})

# Buscar todos los archivos fuente en src
file(GLOB_RECURSE BLOP_SOURCES "${BLOP_SRC_DIR}/*.c")

# Crear la librería
add_library(blop "")
target_include_directories(blop PUBLIC ${BLOP_INCLUDE_DIR})

add_subdirectory(src)

# Definir el estándar de C
set_target_properties(blop PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED YES
    C_EXTENSIONS NO
)

# Instalar librería y headers
install(TARGETS blop
    EXPORT blop-targets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY ${BLOP_INCLUDE_DIR}/blop DESTINATION include)