#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#define MAX_LEN 512

void *handle_receive(void *socket_desc);
void trim(char *str);
void ignore_sigint(int sig);

int client_socket; 

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Uso: %s <IP> <Puerto>\n", argv[0]);
        return 1;
    }

    char *ip_address = argv[1];
    int port = atoi(argv[2]);

    if (strcmp(ip_address, "localhost") == 0) {
        ip_address = "127.0.0.1";
    }

    struct sockaddr_in server_address;

    ignorar SIGINT (Ctrl + C)
    signal(SIGINT, SIG_IGN);  

    // Crear el socket
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);

    if (inet_pton(AF_INET, ip_address, &server_address.sin_addr) <= 0) {
        perror("Dirección invalida");
        exit(EXIT_FAILURE);
    }

    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Conexión fallida");
        exit(EXIT_FAILURE);
    }

    printf("Conectado al servidor en %s:%d\n", ip_address, port);

    char buffer[MAX_LEN];
    pthread_t receive_thread;

    if (pthread_create(&receive_thread, NULL, handle_receive, (void *)&client_socket) != 0) {
        perror("Error al crear el hilo de recepción");
        exit(EXIT_FAILURE);
    }

    fgets(buffer, MAX_LEN, stdin);
    trim(buffer);
    send(client_socket, buffer, strlen(buffer), 0);

    while (1) {
        fgets(buffer, MAX_LEN, stdin);
        trim(buffer);

        if (strcmp(buffer, "/exit") == 0) {
            send(client_socket, buffer, strlen(buffer), 0);
            break;
        }

        send(client_socket, buffer, strlen(buffer), 0);
    }

    close(client_socket);
    return 0;
}

void *handle_receive(void *socket_desc) {
    int client_socket = *(int *)socket_desc;
    char buffer[MAX_LEN];
    int bytes_read;

    while ((bytes_read = recv(client_socket, buffer, MAX_LEN, 0)) > 0) {
        buffer[bytes_read] = '\0'; 
        printf("%s\n", buffer);
    }

    if (bytes_read == 0) {
        printf("Servidor desconectado.\n");
    } else {
        perror("Error en la recepción");
    }

    pthread_exit(NULL);
}

void trim(char *str) {
    size_t len = strlen(str);
    if (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r')) {
        str[len - 1] = '\0';
    }
}
