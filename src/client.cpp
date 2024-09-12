#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "fabricaDeJsons.h"

int main() {
    // Crear un socket
    int cliente_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (cliente_socket == -1) {
        std::cerr << "No se pudo crear el socket.\n";
        return -1;
    }

    // Configurar la dirección del servidor
    sockaddr_in servidor_dir;
    servidor_dir.sin_family = AF_INET;
    servidor_dir.sin_port = htons(8080);
    servidor_dir.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Conectar al servidor
    if (connect(cliente_socket, (sockaddr*)&servidor_dir, sizeof(servidor_dir)) == -1) {
        std::cerr << "No se pudo conectar al servidor.\n";
        close(cliente_socket);
        return -1;
    }

    // Pedir nombre de usuario
    std::string username;
    std::cout << "Ingrese su nombre de usuario: ";
    std::cin >> username;

    // Crear el JSON de identificación
    std::string json_identify = FabricaDeJsons::crearJsonIdentificacion(username);

    // Enviar el JSON al servidor
    send(cliente_socket, json_identify.c_str(), json_identify.size(), 0);

    //std::cout << "Mensaje enviado al servidor: " << json_identify << "\n";

    // Cerrar el socket
    close(cliente_socket);

    return 0;
}
