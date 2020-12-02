#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
// Server side: 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define EVENT_SIZE (sizeof(struct inotify_event))
#define BUF_LEN (1024 * (EVENT_SIZE + 16))

const char *get_filename_extension(const char *filename)
{
    const char *dot = strrchr(filename, '.');
    if (!dot || dot == filename)
    {
        return "";
    }
    else
    {
        return dot + 1;
    }
}
/*
struct sockaddr_in {
    short            sin_family;   // e.g. AF_INET
    unsigned short   sin_port;     // e.g. htons(3490)
    struct in_addr   sin_addr;     // see struct in_addr, below
    char             sin_zero[8];  // zero this if you want to
};
*/
/*
struct inotify_event {
	_Int32t   wd;
	_Uint32t  mask;
	_Uint32t  cookie;
	_Uint32t  len;
	char name[0];
};
*/
void error(char *msg)
{
    perror(msg);
    exit(1);
}


int main(void)
{
    int notifier;
    int watcher;
    char *message;
    const char *folder = "../DNAComparator-Client/"; // change for own
    notifier = inotify_init();
    int clilen;
    char recvBuff[1024];
    struct sockaddr_in serv_addr, cli_addr;
    int sockfd, clisockfd, n;
    int portno = 6668;
    memset(recvBuff, '0',sizeof(recvBuff));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(6668);

    //create a socket
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);


    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR creating socket");

    //bind address to socket
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR: could not bind socket");

    //listen for connection request
    if (listen(sockfd, 5) < 0)
        error("ERROR: could not listen on socket");

    memset(&serv_addr, '0', sizeof(serv_addr));


    if (notifier < 0)
    {
        perror("Error notifier init");
        exit(2);
    }
    watcher = inotify_add_watch(notifier, folder, IN_CREATE);

    if (watcher == -1)
    {
        printf("No se puede observar: %s\n", folder);
    }
    else
    {
        printf("Observando: %s\n", folder);
    }

    daemon(1, 1);
    
    char recvbuffer[BUF_LEN];
    while (1)
    {
        int length = 0;
        int i = 0;
        char buffer[BUF_LEN];
        length = read(notifier, buffer, BUF_LEN);
        struct inotify_event *event = (struct inotify_event *)&buffer[i];
        clilen = sizeof(serv_addr);
        clisockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (clisockfd < 0)
            error("ERROR: could not accept connection");
      
            
            
        while (i < length)
        {
            
            if (event->len)
            {
                
                printf("%s\n", get_filename_extension(event->name));
                if (strcmp(get_filename_extension(event->name), "seq") == 0)
                {
                    if (event->mask & IN_CREATE)
                    {
                        printf("El archivo %s fue creado.\n", event->name);
                                    //send data
                        message = event->name;
                        n = send(clisockfd, message, strlen(message), 0);
                        if (n < 0)
                        {
                            fputs("ERROR sending data to client\n", stderr);
                            break;
                        }
                    }
                }
            }
            i += EVENT_SIZE + event->len;
        }
    }
    (void)inotify_rm_watch(notifier, watcher);
    (void)close(notifier);
    close(clisockfd);
    close(sockfd);
    return 0;
}