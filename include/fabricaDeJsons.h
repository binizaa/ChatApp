#ifndef FABRICA_DE_JSONS_H
#define FABRICA_DE_JSONS_H

#include <string>
#include <nlohmann/json.hpp>

class FabricaDeJsons {
public:
    // Función para crear el JSON de tipo IDENTIFY con el nombre de usuario
    static std::string crearJsonIdentificacion(const std::string& username);

    // Función para crear el JSON de respuesta cuando la identificación es exitosa
    static std::string crearJsonRespuestaSuccess(const std::string& username);

    // Función para crear el JSON de respuesta cuando el usuario ya existe
    static std::string crearJsonRespuestaUsuarioYaExiste(const std::string& username);
};

#endif // FABRICA_DE_JSONS_H
