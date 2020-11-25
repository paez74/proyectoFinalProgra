
#include<stdio.h>
#include<winsock2.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library
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

//main
int main(int argc , char *argv[])
{
	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in server;
	char *message , server_reply[2000];
	int recv_size;

	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
	{
		printf("Failed. Error Code : %d",WSAGetLastError());
		return 1;
	}
	
	printf("Initialised.\n");
	
	//Create a socket
	if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d" , WSAGetLastError());
	}

	printf("Socket created.\n");
	
	
	server.sin_addr.s_addr = inet_addr("74.125.235.20");
	server.sin_family = AF_INET;
	server.sin_port = htons( 80 );

	//Connect to remote server
	if (connect(s , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		puts("connect error");
		return 1;
	}
	
	puts("Connected");
	
	//Send some data
	message = "GET / HTTP/1.1\r\n\r\n";
	if( send(s , message , strlen(message) , 0) < 0)
	{
		puts("Send failed");
		return 1;
	}
	puts("Data Send\n");
	
	//Receive a reply from the server
	if((recv_size = recv(s , server_reply , 2000 , 0)) == SOCKET_ERROR)
	{
		puts("recv failed");
	}
	
	puts("Reply received\n");

	//Add a NULL terminating character to make it a proper string before printing
	server_reply[recv_size] = '\0';
	puts(server_reply);

	#pragma omp parallel
    {
        int i;
        // aqui va el multithreding para evaluar todas las diferentes lineas del archivo a leer
        #pragma omp parallel for 
            for(i = 0; i<12; i++)
             printf("Esta en un for paralelo");
              
    }



	return 0;
}