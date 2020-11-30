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
char sequences[100][BUFSIZ];

void shift_left(char *arr, int s) {
    for (int i = s; i >= 0; i--)
        arr[i] = arr[i - 1];
}

int sub_in_seq(char *sub, int sub_s) {
    FILE *seq_f = fopen(TEXTFILE, "r");

    int i = 0, j = 0, seq_s = 0;
    char c;
	char *seq = malloc(sizeof(char) * (sub_s + 1));
	seq[sub_s] = '\0';
    while ((c = fgetc(seq_f)) != EOF) {
        seq[seq_s++] = c;

        if (seq_s == sub_s) {
            if (seq[0] == sub[0]) { // Potential subsequence
                j = 0;
                while (j < seq_s) {
                    if (sub[j] != seq[j]) break;
                    ++j;
                }

                if (j == seq_s) return i;
            }

            shift_left(seq, seq_s);
            --seq_s;
        }
        ++i;
    }

    fclose(seq_f);

    return -1;
}

int readFile(char* fileName) 
{
	printf("File name = %s", fileName);
	char* line;
    FILE* file;
	size_t len = 0;
    file = fopen(fileName, "r");
    if (file == NULL){
		printf("No se pudo abrir\n");
        return -1;
	}
	int i = 0;
	printf("\nVa a entrar al while");
     while (i < 100 && fgets(sequences[i], sizeof(sequences[0]), file))
    {
        sequences[i][strlen(sequences[i])-1] = '\0';
		lines++;
        i = i + 1;
    }
	printf("Salio del while\n");
    return 0;
}

void compareDNA()
{
	
		int foundLines = 0;
        printf("Lines to search = %d\n", lines);
        #pragma omp parallel for shared(foundLines) schedule(static)
            for(int i = 0; i<lines; i++)
			{
				
				int foundAt = sub_in_seq(sequences[i],sizeof(sequences[i])); // ideal que quede asi 
				if(foundAt != -1)
				{
					// logica para guardar los resultados o para desplegarlos directo
					printf("%s a partir del caracter %d\n", sequences[i],foundAt);
					foundLines++;
				}else 
				{
					printf("%s no se encontro\n", sequences[i]);
				} 
			}
			printf("El archivo cubre el %d porciento del genoma de referencia\n", foundLines/lines * 100);
			printf("%d secuencias mapeadas\n",foundLines);
			printf("%d secuencias no mapeadas\n", foundLines - lines * -1);
              
}


int main(int argc , char *argv[])
{
	readFile("Ejemplo.seq");
	compareDNA();
	// WSADATA wsa;
	// SOCKET s;
	// struct sockaddr_in server;
	// char *message , server_reply[2000];
	// int recv_size;

	// printf("\nInitialising Winsock...");
	// if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
	// {
	// 	printf("Failed. Error Code : %d",WSAGetLastError());
	// 	return 1;
	// }
	
	// printf("Initialised.\n");
	
	// //Create a socket
	// if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
	// {
	// 	printf("Could not create socket : %d" , WSAGetLastError());
	// }

	// printf("Socket created.\n");
	
	
	// server.sin_addr.s_addr = inet_addr("127.0.0.1");
	// server.sin_family = AF_INET;
	// server.sin_port = htons( 6666 );

	// //Connect to remote server
	// if (connect(s , (struct sockaddr *)&server , sizeof(server)) < 0)
	// {
	// 	puts("connect error");
	// 	return 1;
	// }
	
	// puts("Connected");
	
	// //Send some data
	// message = "Hola como estas :)";
	// if( send(s , message , strlen(message) , 0) < 0)
	// {
	// 	puts("Send failed");
	// 	return 1;
	// }
	// puts("Data Send\n");
	
	
	
	// while((recv_size = recv(s , server_reply , 2000 , 0)) != SOCKET_ERROR)
	// {
	// 	puts("Reply received\n");
	// 	if(recv_size > 0)
	// 	{
	// 		server_reply[recv_size] = '\0';
	// 		puts(server_reply);
	// 		readFile(server_reply); 
	// 		compareDNA();
	// 	}

	// }

	

	return 0;
}