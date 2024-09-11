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

#define MAX_LEN 512

struct Client {
    int socket;
    std::string name;
};

vector<Client> clients;
mutex clients_mutex;

void handle_client(int client_socket);
void broadcast_message(const string &message, int sender_socket);
string trim(const string &str);

int main(int argc, char *argv[]) {
    if (argc < 3) {
        cerr << "Uso: " << argv[0] << " <IP> <Puerto>" << endl;
        return 1;
    }

    string ip_address = argv[1];  

    if (ip_address == "localhost") {
        ip_address = "127.0.0.1";
    }

    int port = stoi(argv[2]);  

    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Crear el socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_port = htons(port);  

    if (inet_pton(AF_INET, ip_address.c_str(), &address.sin_addr) <= 0) {
        perror("Dirección invalida");
        exit(EXIT_FAILURE);
    }

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    cout << "Servidor escuchando en " << ip_address << ": " << port << endl;

    while (true) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        cout << "Nuevo cliente conectado" << endl;

        thread t(handle_client, new_socket);
        t.detach();
    }

    close(server_fd);
    return 0;
}

void handle_client(int client_socket) {
    char buffer[MAX_LEN];
    string name;

    // Pedir nombre de usuario
    send(client_socket, "Ingrese su nombre: ", strlen("Ingrese su nombre: "), 0);
    int bytes_read = recv(client_socket, buffer, MAX_LEN, 0);
    if (bytes_read <= 0) {
        close(client_socket);
        return;
    }

    name = trim(string(buffer, bytes_read));

    // Crear el cliente y agregarlo a la lista
    {
        lock_guard<mutex> guard(clients_mutex);
        clients.push_back({client_socket, name});
    }

    string welcome_msg = "Bienvenido al chat, " + name + "!\n";
    send(client_socket, welcome_msg.c_str(), welcome_msg.length(), 0);

    // Loop principal de mensajes
    while (true) {
        memset(buffer, 0, MAX_LEN);
        bytes_read = recv(client_socket, buffer, MAX_LEN, 0);
        if (bytes_read <= 0) {
            cout << "Cliente desconectado: " << name << endl;
            close(client_socket);

            // Remover el cliente de la lista
            {
                lock_guard<mutex> guard(clients_mutex);
                clients.erase(remove_if(clients.begin(), clients.end(),
                                        [client_socket](const Client &c) { return c.socket == client_socket; }),
                              clients.end());
            }
            break;
        }

        string message(buffer, bytes_read);
        message = trim(message);

        if (message == "/exit") {
            cout << "Cliente " << name << " ha enviado /exit. Desconectando...\n";
            close(client_socket);

            {
                lock_guard<mutex> guard(clients_mutex);
                clients.erase(remove_if(clients.begin(), clients.end(),
                                        [client_socket](const Client &c) { return c.socket == client_socket; }),
                              clients.end());
            }
            break;
        }

        message = name + ": " + message + "\n";
        cout << message;

        broadcast_message(message, client_socket);
    }
}

void broadcast_message(const string &message, int sender_socket) {
    lock_guard<mutex> guard(clients_mutex);
    for (const auto &client : clients) {
        if (client.socket != sender_socket) {
            send(client.socket, message.c_str(), message.length(), 0);
        }
    }
}

// Función para eliminar espacios en blanco y saltos de línea
string trim(const string &str) {
    size_t first = str.find_first_not_of("\r\n");
    size_t last = str.find_last_not_of("\r\n");
    return str.substr(first, (last - first + 1));
}