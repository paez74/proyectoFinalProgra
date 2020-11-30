#include<io.h>
#include <stdio.h>
//#include <omp.h>
#include <winsock2.h> // en linux es sys-sock.h

#pragma comment(lib,"ws2_32.lib")

int sub_in_seq(char *sub, int sub_s, char *seq, int seq_s) {
    int i = 0, j;
    while (i < seq_s) {
        if (seq[i] == sub[i]) { // Potential subsequence
            j = i;
            while (j < sub_s && j < seq_s) {
                if (sub[j] != seq[j]) break;
                ++j;
            }

            if (j == sub_s) return i;
        }
        ++i;
    }

    return -1;
}

//funcion main
int main(int argc, char *argv[]) 
{
    WSADATA wsa;
    SOCKET s, new_socket;
    struct sockaddr_in server, client;
    int c;
    char *message;

    if(WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed");
        return 1;
    }

    if((s = socket(AF_INET,SOCK_STREAM,0)) == INVALID_SOCKET)
    {
        printf("Error creating socket");
    }

    printf("Socket created");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(6666); // numero de puerto


    if (bind (s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
    {
        printf("bind error");
    }

    listen(s,3);

    c = sizeof(struct sockaddr_in);
    while((new_socket = accept(s, (struct sockaddr *)&client, &c)) != INVALID_SOCKET)
    {
        puts("Connection Accepted");
        message = "Hello socket, como estas\n";
        send(new_socket, message, strlen(message), 0);
        _sleep(300);
        message = "Hola soy yo de nuevo\n";
        send(new_socket, message, strlen(message), 0);
        _sleep(250);
        message = "Y aqui sigo esperandote\n";
        send(new_socket,message,strlen(message),0);
    }


 if(new_socket == INVALID_SOCKET)
 {
     printf("Trono el socket");
 }

}