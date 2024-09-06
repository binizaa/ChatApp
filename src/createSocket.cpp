#include "createSocket.h"

int createSocket() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Fallo la creación del socket");
        exit(EXIT_FAILURE);
    }
    return sockfd;
}
