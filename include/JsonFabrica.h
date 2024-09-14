#ifndef JSONFABRICA_H
#define JSONFABRICA_H

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

class JsonFabrica {
public:
    // Crear mensaje de IDENTIFY
    static json create_identify(const string& username) {
        return json{
            {"type", "IDENTIFY"},
            {"username", username}
        };
    }

    // Crear mensaje de respuesta RESPONSE para IDENTIFY
    static json create_identify_response(const string& operation, const string& result, const string& username) {
        return json{
            {"type", "RESPONSE"},
            {"operation", operation},
            {"result", result},
            {"extra", username}
        };
    }

    // Crear mensaje de NEW_USER
    static json create_new_user(const string& username) {
        return json{
            {"type", "NEW_USER"},
            {"username", username}
        };
    }
};

#endif // JSONFABRICA_H
