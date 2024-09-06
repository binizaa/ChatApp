#include <iostream>
#include <sys/types.h>   
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <unistd.h> 
#include <thread>
using namespace std;

int cntID = 0;

int createSocket() {
    int sck = socket(AF_INET, SOCK_STREAM, 0);
    if (sck < 0) {
        perror("Fallo la creación del socket");
        exit(1);
    }
    return sck;
}

int main(int argc, char *argv[]){
    if(argc < 2){
        cout<<"Argumentos incorrectos"<<endl;
        return 0;
    }

    int portNum = atoi(argv[1]); 

    int serverSocket = createSocket();

    //Dirección IPv4
    struct sockaddr_in adress;

    adress.sin_family = AF_INET;
    adress.sin_port = htons(portNum); //Big-endian
    adress.sin_addr.s_addr = INADDR_ANY;

    if(bind(serverSocket, (struct sockaddr *)&adress, sizeof(adress)) < 0){
        cout<<"Error en Bind"<<endl;
        return 0;
    }

    if(listen(serverSocket, 8) < 0){
        cout<<"Error en listen"<<endl;
        return 0;
    }

    cout<<"Bienvenidos"<<endl;

    struct sockaddr_in clientAdress;
    socklen_t lenght = sizeof(sockaddr_in);
    int clientSocket;

    while(true){
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAdress,&lenght); 
        if(clientSocket < 0){
            cout<<"Error de acept"<<endl;
            return 0;
        }
        cntID++;
        //thread t(clientSocket, cntID);
        //clients.push_back()
    }

    close(serverSocket);
    return 0;

}