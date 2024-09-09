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
#include <algorithm>  // Incluir para std::remove

#define PORT 1234
#define MAX_LEN 512

std::vector<int> clients;        // Lista de sockets de clientes
std::mutex clients_mutex;        // Mutex para proteger el acceso a la lista de clientes

void handle_client(int client_socket);
void broadcast_message(const std::string &message, int sender_socket);

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Crear el descriptor de socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forzar la unión del socket al puerto 1234
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

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

    std::cout << "Servidor escuchando en el puerto " << PORT << std::endl;

    while (true) {
        // Aceptar conexiones entrantes
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        std::cout << "Nuevo cliente conectado" << std::endl;

        // Añadir el nuevo cliente a la lista de clientes
        {
            std::lock_guard<std::mutex> guard(clients_mutex);
            clients.push_back(new_socket);
        }

        // Crear un hilo para manejar la conexión con el nuevo cliente
        std::thread t(handle_client, new_socket);
        t.detach();  // Desacoplar el hilo para que funcione independientemente
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

        std::string message = "Cliente " + std::to_string(client_socket) + ": " + buffer;
        std::cout << message;

        // Enviar el mensaje a todos los clientes excepto al remitente
        broadcast_message(message, client_socket);
    }
}
