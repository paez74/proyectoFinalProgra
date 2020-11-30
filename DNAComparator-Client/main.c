/*
	Create a TCP socket
*/

#include<stdio.h>
#include <stdint.h>
#include<winsock2.h>
// referencia al archivo donde esta el genoma humano.
#define TEXTFILE "C:\\Users\\paez7\\Documents\\Tec\\ProgramacionAvanzada\\proyectoFinalProgra\\DNAComparator-Client\\Secuencia.txt" 

#pragma comment(lib,"ws2_32.lib") //Winsock Library
int lines=0;
char* text[255];

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

int readFile(char* fileName) 
{
	char* line;
    FILE* file;
	size_t len = 0;
    file = fopen(fileName, "r");
    if (file == NULL)
        return -1;
     while((getline(&line, &len, file)) != 1){
		text[lines] =malloc(sizeof(line));
		strcpy(text[lines],line);
		lines++;
     }
    return 0;
}

void compareDNA()
{
	    #pragma omp parallel
    {
       
        #pragma omp parallel for 
            for(int i = 0; i<lines; i++)
			{
				int foundAt  = -1;
				//int foundAt = sub_in_seq(text[i],sizeof(text[i])) // ideal que quede asi 
				if(foundAt != -1)
				{
					// logica para guardar los resultados o para desplegarlos directo

				} 
			}
              
    }
}


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
	
	
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons( 6666 );

	//Connect to remote server
	if (connect(s , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		puts("connect error");
		return 1;
	}
	
	puts("Connected");
	
	//Send some data
	message = "Hola como estas :)";
	if( send(s , message , strlen(message) , 0) < 0)
	{
		puts("Send failed");
		return 1;
	}
	puts("Data Send\n");
	
	
	
	while((recv_size = recv(s , server_reply , 2000 , 0)) != SOCKET_ERROR)
	{
		puts("Reply received\n");
		if(recv_size > 0)
		{
			server_reply[recv_size] = '\0';
			puts(server_reply);
			readFile(server_reply); 
			compareDNA();
		}

	}

	

	return 0;
}