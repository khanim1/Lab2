#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

//from my git
//
void func(int numcmd, char* cmds[], char** args[]){
int l=numcmd;
int j = 0;
int i=0;
int ret_val;
int frk;
int* newpipe;
int oldpipe[2];
for (j;j<=l;j++) { //l is num of commands
if (j!=l) {
//int fd[2];
if (j!=0) { //remember previous pipe
oldpipe[0] = newpipe[0];
oldpipe[1] = newpipe[1];
//oldpipe = newpipe;
}
newpipe = (int *) malloc(2*sizeof(int));
//newpipe = fd;
//newpipe[0] = fd[0];
//newpipe[1] = fd[1];
pipe(newpipe);
frk = fork();
if(frk==-1){
perror("Fork");
exit(-2);
}
//i++;//counter for forks
}
//printf("newpipe[1]: %d\n",*newpipe);
if (j==0) {
if (frk ==0) {
//printf("fd[1]: %d\n",fd[1]);
//dup2(newpipe[1],STDOUT_FILENO);
ret_val = execvp(cmds[j],args[j]);
if (ret_val == -1) perror ("Error exec'ing");
close(newpipe[0]);
i++;
exit(0);
}
close(newpipe[1]);
}

if (j==l) {  //if last command?
frk = fork();
if(frk==-1){
perror("Fork j=l");
exit(-2);
}
if (frk==0) {
dup2(oldpipe[0],STDIN_FILENO);
ret_val = execvp(cmds[j],args[j]);
if (ret_val == -1) perror ("Error exec'ing");
close(oldpipe[1]);
i++;
exit(0);
}
close(oldpipe[0]);
}
else {
if (frk==0) {
dup2(newpipe[1],STDOUT_FILENO);
dup2(oldpipe[0],STDIN_FILENO);
ret_val = execvp(cmds[j],args[j]);
if (ret_val == -1) perror ("Error exec'ing");
close(oldpipe[1]);
close(newpipe[0]);
i++;
exit(0);
}
close(newpipe[1]);
close(oldpipe[0]);
}
}


int q=0;
int status;
pid_t pid;
while (q<i) {
pid = wait(&status);
q++;
}


}


int main(int argc, char* argv[]){
int max_args = 15;
int max_argv_size = max_args + 2;
char* cmds_args[max_args];
//char* my_argv[max_argv_size];
char* cmds[max_args];
char** args[max_args];
int bufferSize=256;
char buffer[bufferSize];

printf("Enter input: ");

char* input_string = fgets(buffer, bufferSize, stdin);
if(input_string==0){//for EOF with cntrl+D
printf("\nEOF Entered\n");
printf("Goodbye\n");
exit(0);
}

if(input_string[strlen(input_string)-1] == '\n'){
input_string[strlen(input_string)-1] = 0;
}


int i = 0;
int j=0;
cmds_args[i] = strtok(buffer,"("); //stores commands and args initially
char* ret = strtok(NULL,"(");
while (ret!=NULL) {
i++;
if (i == max_args) {
printf("Too many commands \n");
//exit(-1);
return 0;
}
cmds_args[i] = ret;
ret = strtok(NULL,"(");
}

int l = i; //remember i for later
for (i; i>=0; --i) {
cmds[j] = strtok(cmds_args[i]," \n"); //array of commands only
//*CMDS IN EXECUTION ORDER, LAST IS FIRST
//char* my_argv[max_argv_size];
char** my_argv = malloc(sizeof(char) * max_argv_size);
printf("Setting argv[0] to cmds[%d]: %s\n",j,cmds[j]);
my_argv[0] = cmds[j]; //my_argv[0] must contain 
//function name, used in execvp via args[] (see below)
ret = strtok(NULL, " \n");
int m = 0;
while (ret != NULL) {
m++;
if (m==max_args) {
printf("Too many arguments \n");
//exit(-1);
return 0;
}

my_argv[m] = ret;
ret = strtok(NULL," \n");
		}
my_argv[m+1] = '\0';
//if (my_argv[2] == NULL) {
//}

//printf("Setting args[%d] to my_argv with vals of:\n",j);
//printf("my_argv[0] = %s\n",my_argv[0]);
//printf("my_argv[1] = %s\n",my_argv[1]);
args[j] = my_argv; //array of my_argv's correlates with cmd[]
j++;
}
//printf("cmds[0]: %s\n",cmds[0]);
//printf("cmds[1]: %s\n",cmds[1]);
//printf("args[0] argv[0]: %s\n",args[0][0]);
//printf("args[0] argv[1]: %s\n",args[0][1]);
//printf("args[0] argv[2]: %s\n",args[0][2]);
//printf("args[1] argv[0]: %s\n",args[1][0]);
//printf("args[1] argv[1]: %s\n",args[1][1]);
//printf("args[1] argv[2]: %s\n",args[1][2]);

func(l, cmds, args);

return 0;
}
