#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <nlohmann/json.hpp>

int main() {
    int servidor_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (servidor_socket == -1) {
        std::cerr << "No se pudo crear el socket del servidor.\n";
        return -1;
    }

    sockaddr_in servidor_dir;
    servidor_dir.sin_family = AF_INET;
    servidor_dir.sin_port = htons(8080);
    servidor_dir.sin_addr.s_addr = INADDR_ANY;

    if (bind(servidor_socket, (sockaddr*)&servidor_dir, sizeof(servidor_dir)) == -1) {
        std::cerr << "Error en bind.\n";
        close(servidor_socket);
        return -1;
    }

    if (listen(servidor_socket, 5) == -1) {
        std::cerr << "Error en listen.\n";
        close(servidor_socket);
        return -1;
    }

    std::cout << "Esperando conexiones...\n";

    sockaddr_in cliente_dir;
    socklen_t cliente_len = sizeof(cliente_dir);
    int cliente_socket = accept(servidor_socket, (sockaddr*)&cliente_dir, &cliente_len);
    if (cliente_socket == -1) {
        std::cerr << "Error al aceptar conexión.\n";
        close(servidor_socket);
        return -1;
    }

    // Buffer para recibir datos
    char buffer[1024] = {0};
    int bytes_recibidos = recv(cliente_socket, buffer, 1024, 0);
    if (bytes_recibidos == -1) {
        std::cerr << "Error al recibir datos.\n";
        close(cliente_socket);
        close(servidor_socket);
        return -1;
    }

    // Convertir los datos a JSON
    std::string datos(buffer, bytes_recibidos);
    auto json_recibido = nlohmann::json::parse(datos);

    std::cout << "JSON recibido: " << json_recibido.dump(4) << "\n";

    close(cliente_socket);
    close(servidor_socket);

    return 0;
}   
