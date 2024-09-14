#ifndef FABRICADEJSONS_H
#define FABRICADEJSONS_H

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

class FabricaDeJsons {
public:
    static json create_identify(const string& username);

    static json create_identify_response(const string& operation, const string& result, const string& username);

    static json create_new_user(const string& username);
};

#endif // FABRICADEJSONS_H
