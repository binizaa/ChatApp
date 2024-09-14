cmake_minimum_required(VERSION 3.10)
project(ChatApp)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

# Localiza la biblioteca JSON
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)

include_directories(${JSONCPP_INCLUDE_DIRS})
link_directories(${JSONCPP_LIBRARY_DIRS})
add_definitions(${JSONCPP_CFLAGS_OTHER})

# Archivos fuente
add_executable(server src/server.cpp)
add_executable(client src/client.cpp src/fabricaDeJsons.cpp)

# Agregar nlohmann/json desde GitHub
include(FetchContent)
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.10.4
)
FetchContent_MakeAvailable(json)

# Enlazar la librería JSON
#target_link_libraries(chat_cliente PRIVATE nlohmann_json::nlohmann_json)

# Encuentra la biblioteca cJSON
find_path(CJSON_INCLUDE_DIR cJSON.h)
find_library(CJSON_LIBRARY cjson)


# Enlaza la biblioteca cJSON al ejecutable del servidor
target_link_libraries(server ${CJSON_LIBRARY})
