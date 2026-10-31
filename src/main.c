#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ARG_LENGTH 1024

char **files;

void init_files(int num){
    files = malloc(num * sizeof(char *));
}

typedef struct{
    int pid_status;
    pid_t waiter;
    pid_t child_id;
    char* Name;
}child;

int main(int argc, char* argv[]){
    if (argc > 12) {
        fprintf(stderr,"Too Many Arguments\n");
        return 1;
    }
    else if (argc < 3) {
        fprintf(stderr,"Not Enough Arguments Passed\n");
        return 1;
    }
    char* comm = argv[1];
    int arg_count = argc - 2;
    //printf("%d",arg_count);
    child kid[arg_count];
    init_files(arg_count);
    int k = 0;
    for (int i = 2;i < argc;i++) {
        if (k < arg_count) {
            files[k] = argv[i];
            kid[k].Name = files[k];
            //printf("Name: %s\n,%d",kid[k].Name,k);
            k++;
        }
        else {
            break;
        }
    } //Will be abstracted later
    //printf("%s\n",kid[1].Name);
    //Hand out instructions to children and gather their pids when done and wait for all of them
    for (int i = 0;i < arg_count;i++) {
        kid[i].child_id = fork();
        if (kid[i].child_id == -1) {
            fprintf(stderr,"Can't Fork Process: %s\n",strerror(errno));
        }
        if (kid[i].child_id == 0) {
            if (execlp(comm,comm,files[i],NULL) == -1) {
                fprintf(stderr,"Cannot run Process: %s\n",strerror(errno));
                exit(1);
            }
        }
    }
    for (int i = 0;i < arg_count; i++) {
        kid[i].waiter = waitpid(kid[i].child_id,&(kid[i].pid_status),0);
        if (WEXITSTATUS(kid[i].pid_status)) {
            fprintf(stderr,"Error in child exit process. Child:%s\n",kid[i].Name);
        }//Checks if exited abnormally
        else if (WIFSIGNALED(kid[i].pid_status)) {
            fprintf(stderr,"Child %s Stopped by Signal:%d\n",kid[i].Name,WTERMSIG(kid[i].pid_status));
        }//Checks if exited by signal
    }
    free(files);
    return 0;
}