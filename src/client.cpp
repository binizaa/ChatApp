#include <arpa/inet.h>
#include <iostream>
#include <string>
#include <thread>
#include <csignal>
#include <cstring>
#include <unistd.h>
#include <nlohmann/json.hpp>  // Incluir la librería de JSON

#define MAX_LEN 512

using json = nlohmann::json;

void handle_receive(int client_socket);
void trim(std::string &str);
void ignore_sigint(int sig);

int client_socket;

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Uso: " << argv[0] << " <IP> <Puerto>\n";
        return 1;
    }

    std::string ip_address = argv[1];
    int port = std::stoi(argv[2]);

    if (ip_address == "localhost") {
        ip_address = "127.0.0.1";
    }

    struct sockaddr_in server_address{};

    signal(SIGINT, SIG_IGN);  // Ignorar señales de interrupción

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);

    if (inet_pton(AF_INET, ip_address.c_str(), &server_address.sin_addr) <= 0) {
        perror("Dirección inválida");
        exit(EXIT_FAILURE);
    }

    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Conexión fallida");
        exit(EXIT_FAILURE);
    }

    std::cout << "Conectado al servidor en " << ip_address << ":" << port << "\n";

    std::thread receive_thread(handle_receive, client_socket);

    std::string username;
    std::cout << "Ingrese su nombre de usuario: ";
    std::getline(std::cin, username);

    json identify_message = {
        {"type", "IDENTIFY"},
        {"username", username}
    };

    std::string json_str = identify_message.dump();
    send(client_socket, json_str.c_str(), json_str.length(), 0);

    std::string buffer;
    while (true) {
        std::getline(std::cin, buffer);
        if (buffer == "/exit") {
            send(client_socket, buffer.c_str(), buffer.length(), 0);
            break;
        }

        send(client_socket, buffer.c_str(), buffer.length(), 0);
    }

    receive_thread.join(); // Esperar a que el hilo termine
    close(client_socket);
    return 0;
}

void handle_receive(int client_socket) {
    char buffer[MAX_LEN];
    int bytes_read;

    while ((bytes_read = recv(client_socket, buffer, MAX_LEN, 0)) > 0) {
        buffer[bytes_read] = '\0'; 

        try {
            json received_json = json::parse(buffer);
            if (received_json["type"] == "RESPONSE" && received_json["operation"] == "IDENTIFY" && received_json["result"] == "SUCCESS") {
                std::cout << "Identificación exitosa: " << received_json["extra"] << std::endl;
            } else {
                std::cout << buffer << std::endl;
            }
        } catch (json::parse_error &e) {
            std::cout << buffer << std::endl; 
        }
    }

    if (bytes_read == 0) {
        std::cout << "Servidor desconectado.\n";
    } else {
        perror("Error en la recepción");
    }
}

void trim(std::string &str) {
    str.erase(str.find_last_not_of("\n\r") + 1);
}
