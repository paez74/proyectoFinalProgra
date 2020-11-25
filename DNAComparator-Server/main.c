#include<io.h>
#include <stdio.h>
#include <omp.h>
#include <winsock2.h> // en linux es sys-sock.h

#pragma comment(lib,"ws2_32.lib")

// funcion para leer archivo y partir las lineas

// funcion para evaluar archivo contra el archivo con el valor de ADN

//funcion para desplegar la respuesta
/* formato de la respuesta 
GCCTCCTGCTGCTGCTGCTCTCC a partir del caracter 21
GGACCTCCCAGGCCAGTGCCGGG a partir del caracter x
AAGACCTTCTCCTCCTGCAAATA a partir del caracter x
TTCTTCTGGAAGACCTTCTCCTC a partir del caracter x
CCAGGCGGCAGGAAGGCGCACCCCCCCAGCAATCCGTGCGCCGG no se encontro

El archivo cubre el 15% del genoma de referencia
X secuencias mapeadas
X secuencias no mapeadas
*/
//funcion main
int main(int argc, char *argv[]) 
{
    WSADATA wsa;
    SOCKET s, new_socket;
    struct sockaddr_in server, client;
    int c;
    char *message;

    if(WSAStartup(MAKEWORD(2,2),&wsa != 0))
    {
        printf("Failed");
        return 1;
    }

    if((s = socket(AF_INET,SOCK_STREAM,0)) == INVALID_SOCKET)
    {
        printf("Error creatign socket");
    }

    printf("Socket created");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(6666); // numero de puerto


    if (bind (s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
    {
        printf("bind error");
    }

    listern(s,3);

    c = sizeof(struct sockaddr_in);
    while((new_socket = accept(s, (struct sockaddr *)&client, &c)) != INVALID_SOCKET)
    {
        puts("Connection Accepted");
        message = "Hello socket\n";
        send(new_socket, message, strlen(message), 0);
    }


 if(new_socket == INVALID_SOCKET)
 {
     printf("Trono el socket");
 }


    #pragma omp parallel
    {
        int i;
        // aqui va el multithreding para evaluar todas las diferentes lineas del archivo a leer
        #pragma omp parallel for 
            for(i = 0; i<12; i++)
             printf("Esta en un for paralelo");
              
    }

}