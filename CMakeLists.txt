# Versión mínima requerida de CMake
cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto y versión
project(ChatApp VERSION 1.0)

# Establecer el estándar de C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Añadir el directorio include/ al path de búsqueda de cabeceras
include_directories(include)

# Añadir los archivos fuente
add_executable(ChatApp src/main.cpp)

# Enlazar bibliotecas (si fuera necesario)
# target_link_libraries(MiProyecto libname)
