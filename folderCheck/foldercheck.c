#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <string.h>
#define EVENT_SIZE (sizeof(struct inotify_event))
#define BUF_LEN  (1024 * (EVENT_SIZE + 16))

const char *get_filename_extension(const char *filename){
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename){
        return "";
    }else{
        return dot + 1;
    }
}

/*
struct inotify_event {
	_Int32t   wd;
	_Uint32t  mask;
	_Uint32t  cookie;
	_Uint32t  len;
	char name[0];
};
*/
int main(void){
    int notifier;
    int watcher;
    const char* folder = "/home/parga/Desktop/LabDump/";
    notifier = inotify_init();
    if(notifier < 0){
        perror("Error notifier init");
        exit(2);
    }
    daemon(1,1);
    watcher = inotify_add_watch(notifier, folder, IN_CREATE);
    
    if(watcher == -1){
        printf("No se puede observar: %s\n", folder);
    }else{
        printf("Observando: %s\n", folder);
    }

    while(1){
        int length = 0;
        int i = 0;
        char buffer[BUF_LEN];
        length=read(notifier,buffer,BUF_LEN);
        struct inotify_event *event = (struct inotify_event *) &buffer[i];
        while(i<length){
            if(event->len){
                printf("%s\n",get_filename_extension(event->name));
                if(strcmp(get_filename_extension(event->name),"seq") == 0){
                    if(event->mask & IN_CREATE){
                        printf("El archivo %s fue creado.\n", event->name);
                    }
                }
            }
        i += EVENT_SIZE + event->len;
        }
    }
    (void) inotify_rm_watch(notifier,watcher);
    (void) close(notifier);
    return 0;

}