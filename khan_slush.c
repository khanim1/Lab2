#include <stdio.h>
#include <unistd.h>

#include <stdlib.h>

//for strtok()
#include <string.h>


#define bufferSize 256

int pipe_func ( ) { //need to recurse, 
                //start at left end of command string,
                //work to the right recursively.
                return 0; //maybe change return later
}

int execing ( char* arg []){

        //
        //
        //
        //
        //
        return 0;//maybe change later
}


int cd (char* dir){
        chdir(dir);
        return 0;//maybe change return later
}








int main( int argc, char* argv[]){

        int max_args = 15;
        int max_argv_size = max_args + 2; //one for argv[0], one for null terminator
        char* cmd;
        char* my_argv[max_argv_size];

        while(1)
        {
                printf("Enter input: ");

                char buffer[ bufferSize];

                char* input_string = fgets(buffer, bufferSize, stdin);
                if(input_string==0){//for EOF with cntrl+D
                        printf("EOF\n");
                        printf("Goodbye\n");
                        exit(0);
                }
                printf("%s\n", input_string);
                if(input_string[strlen(input_string)-1] == '\n'){
                        input_string[strlen(input_string)-1] = 0;
                }
                
                cmd = strtok(input_string, " ");

                my_argv[0]=cmd;

                int i=1;
                char* res = strtok(NULL, " ");

                while(res!=NULL){
                        my_argv[i]=res;
                        i++;
                        res = strtok(NULL, " ");
                }

                if(res==NULL) my_argv[i]='\0';

                int execd = execvp( cmd, my_argv );
                if(execd==-1){
                        perror("Not found");
                        exit(-2);
                }
        }

        return 0;
}
