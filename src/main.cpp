#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <mutex>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <vector>
#include <algorithm>  
using namespace std;

#define PORT 1234
#define MAX_LEN 512

std::vector<int> clients;        
std::mutex clients_mutex;       

void handle_client(int client_socket);
void broadcast_message(const std::string &message, int sender_socket);

int main(int argc, char *argv[]) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(stoi(argv[2]));

    // Vincular el socket a la dirección y puerto de red
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Comenzar a escuchar en el socket
    if (listen(server_fd, 10) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    std::cout << "Servidor escuchando en el puerto " << argv[2] << std::endl;

    while (true) {
        // Aceptar conexiones entrantes
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        std::cout << "Nuevo cliente conectado" << std::endl;

        {
            std::lock_guard<std::mutex> guard(clients_mutex);
            clients.push_back(new_socket);
        }

        std::thread t(handle_client, new_socket);
        t.detach();  
    }

    close(server_fd);
    return 0;
}

void broadcast_message(const std::string &message, int sender_socket) {
    std::lock_guard<std::mutex> guard(clients_mutex);
    for (int client_socket : clients) {
        if (client_socket != sender_socket) {
            send(client_socket, message.c_str(), message.length(), 0);
        }
    }
}

#include <algorithm>
#include <cctype>

// Función para eliminar espacios en blanco y saltos de línea
std::string trim(const std::string &str) {
    size_t first = str.find_first_not_of("\r\n");
    size_t last = str.find_last_not_of("\r\n");
    return str.substr(first, (last - first + 1));
}

void handle_client(int client_socket) {
    char buffer[MAX_LEN];
    std::string welcome_msg = "Bienvenido al chat\n";
    send(client_socket, welcome_msg.c_str(), welcome_msg.length(), 0);

    while (true) {
        memset(buffer, 0, MAX_LEN);
        int bytes_read = recv(client_socket, buffer, MAX_LEN, 0);
        if (bytes_read <= 0) {
            std::cout << "Cliente desconectado" << std::endl;
            close(client_socket);

            // Remover el cliente de la lista
            {
                std::lock_guard<std::mutex> guard(clients_mutex);
                clients.erase(std::remove(clients.begin(), clients.end(), client_socket), clients.end());
            }
            break;
        }

        std::string message(buffer, bytes_read);
        message = trim(message);  

        if (message == "/exit") {
            std::cout << "Cliente " << client_socket << " ha enviado /exit. Desconectando...\n";
            close(client_socket);

            // Remover el cliente de la lista
            {
                std::lock_guard<std::mutex> guard(clients_mutex);
                clients.erase(std::remove(clients.begin(), clients.end(), client_socket), clients.end());
            }
            break;  // Salir del bucle para cerrar la conexión
        }

        message = "Cliente " + std::to_string(client_socket) + ": " + message + "\n";
        std::cout << message;

        broadcast_message(message, client_socket);
    }
}

