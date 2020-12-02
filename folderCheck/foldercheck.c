#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
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
int main(void)
{
    int notifier;
    int watcher;
    const char *folder = "/home/parga/LabDump/"; // change for own
    notifier = inotify_init();
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr;
    memset(recvBuff, '0',sizeof(recvBuff));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(6666);

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\n Error : Connect Failed \n");
        return 1;
    }

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

    //daemon(1, 1);

    while (1)
    {
        int length = 0;
        int i = 0;
        char buffer[BUF_LEN];
        length = read(notifier, buffer, BUF_LEN);
        struct inotify_event *event = (struct inotify_event *)&buffer[i];
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
                    }
                }
            }
            i += EVENT_SIZE + event->len;
        }
    }
    (void)inotify_rm_watch(notifier, watcher);
    (void)close(notifier);
    return 0;
}