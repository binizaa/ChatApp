#include "fabricaDeJsons.h"
using namespace std;

// Usamos el espacio de nombres para nlohmann::json
using json = nlohmann::json;

string FabricaDeJsons::crearJsonIdentificacion(const string& username) {
    // Crear un objeto JSON con el formato que se requiere
    json j;
    j["type"] = "IDENTIFY";
    j["username"] = username;

    // Convertir el JSON a string y retornarlo
    return j.dump();
}

string FabricaDeJsons::crearJsonRespuestaSuccess(const string& username) {
    json j;
    j["type"] = "RESPONSE";
    j["operation"] = "IDENTIFY";
    j["result"] = "SUCCESS";
    j["extra"] = username;
    return j.dump();
}

string FabricaDeJsons::crearJsonRespuestaUsuarioYaExiste(const string& username) {
    json j;
    j["type"] = "RESPONSE";
    j["operation"] = "IDENTIFY";
    j["result"] = "USER_ALREADY_EXISTS";
    j["extra"] = username;
    return j.dump();
}

