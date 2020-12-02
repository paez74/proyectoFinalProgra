#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <winsock2.h>

// referencia al archivo donde esta el genoma humano.
#define TEXTFILE "C:\\Users\\paez7\\Documents\\Tec\\ProgramacionAvanzada\\proyectoFinalProgra\\DNAComparator-Client\\Secuencia.txt"

#pragma comment(lib, "ws2_32.lib") //Winsock Library
int lines = 0;
char sequences[100][BUFSIZ];

int seq_size()
{
	FILE *seq_f = fopen(TEXTFILE, "r");
	int size = 0;

	char c;
	while ((c = fgetc(seq_f)) != EOF)
		++size;

	fclose(seq_f);
	return size;
}

int sub_in_seq(char *sub, int sub_s)
{
	FILE *seq_f = fopen(TEXTFILE, "r");

	if (!seq_f)
		return -1;

	int i = 0, seq_s = 0, j;
	char c;
	char *seq = calloc(sub_s + 1, sizeof(char));
	while ((c = fgetc(seq_f)) != EOF)
	{
		seq[seq_s++] = c;

		if (seq_s == sub_s)
		{
			if (seq[0] == sub[0])
			{ // Potential subsequence
				j = 0;
				while (j < seq_s)
				{
					if (sub[j] != seq[j])
						break;
					++j;
				}

				if (j == seq_s)
					return i - sub_s + 2;
			}

			for (int k = 0; k < seq_s; ++k)
				seq[k] = seq[k + 1];
			seq[seq_s--] = '\0';
		}

		++i;
	}

	fclose(seq_f);
	return -1;
}

int readFile(char *fileName)
{
	printf("File name = %s", fileName);
	char *line;
	FILE *file;
	size_t len = 0;
	file = fopen(fileName, "r");
	if (file == NULL)
	{
		printf("No se pudo abrir\n");
		return -1;
	}
	int i = 0;
	printf("\nVa a entrar al while");
	while (i < 100 && fgets(sequences[i], sizeof(sequences[0]), file))
	{
		sequences[i][strlen(sequences[i]) - 1] = '\0';
		lines++;
		i = i + 1;
	}
	printf("Salio del while\n");
	return 0;
}

void compareDNA()
{
	int total_chars = seq_size(), matched_chars = 0, matched_lines = 0;
	printf("Lines to search = %d\n", lines);
#pragma omp parallel for shared(matched_lines) schedule(static)
	for (int i = 0; i < lines; i++)
	{
		int j = 0, size = 0;
		while (sequences[i][j++] != '\0')
			++size;

		int foundAt = sub_in_seq(sequences[i], size); // ideal que quede asi
		if (foundAt != -1)
		{
			printf("%s a partir del caracter %d\n", sequences[i], foundAt);
			matched_chars += size;
			matched_lines += 1;
		}
		else
		{
			printf("%s no se encontro\n", sequences[i]);
		}
	}

	printf("El archivo cubre el %.2f porciento del genoma de referencia\n", (matched_chars * 100.0) / total_chars);
	printf("%d secuencias mapeadas\n", matched_lines);
	printf("%d secuencias no mapeadas\n", lines - matched_lines);
}

int main(int argc, char *argv[])
{

	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in server;
	char *message, server_reply[2000];
	int recv_size;

	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}

	printf("Initialised.\n");

	//Create a socket
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}

	printf("Socket created.\n");

	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(6668);

	//Connect to remote server
	if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		puts("connect error");
		return 1;
	}

	puts("Connected");

	while ((recv_size = recv(s, server_reply, 2000, 0)) != SOCKET_ERROR)
	{
		puts("Reply received\n");
		if (recv_size > 0)
		{
			server_reply[recv_size] = '\0';
			puts(server_reply);
			readFile(server_reply);
			compareDNA();
		}
	}

	return 0;
}