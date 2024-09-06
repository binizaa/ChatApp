cmake_minimum_required(VERSION 3.10)

project(ChatApp)

# Establecer el estándar de C++ (o C)
set(CMAKE_CXX_STANDARD 11)

# Incluir los directorios de cabecera
include_directories(include)

# Añadir el ejecutable del servidor y los archivos fuente
add_executable(server src/server.cpp src/createSocket.cpp)
