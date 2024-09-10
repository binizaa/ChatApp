#ifndef PROTOCOLO_HPP
#define PROTOCOLO_HPP

#include <string>
#include <nlohmann/json.hpp>  // Biblioteca para manejar JSON

using json = nlohmann::json;

// Funciones para manejar el protocolo de mensajes
std::string handle_identify(const std::string &message, int client_socket, std::map<int, std::string> &client_usernames);
std::string create_response(const std::string &operation, const std::string &result, const std::string &extra);
std::string create_new_user_notification(const std::string &username);

#endif  // PROTOCOLO_HPP
