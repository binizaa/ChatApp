#include "fabricaDeJsons.h"

json FabricaDeJsons::create_identify(const string& username) {
    return json{
        {"type", "IDENTIFY"},
        {"username", username}
    };
}

json FabricaDeJsons::create_identify_response(const string& operation, const string& result, const string& username) {
    return json{
        {"type", "RESPONSE"},
        {"operation", operation},
        {"result", result},
        {"extra", username}
    };
}

json FabricaDeJsons::create_new_user(const string& username) {
    return json{
        {"type", "NEW_USER"},
        {"username", username}
    };
}
