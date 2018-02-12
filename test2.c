#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

//from my git

void func(int numcmd, char* cmds[], char** args[]){
	
	//while(1){

	printf("Numcmd: %d\n", numcmd);	

	int l=numcmd;
	int j = 0;
	int i=0;
	int ret_val;
	int frk = -1;
	int newpipe[2];
	int oldpipe[2];
	//while(1){
	for (j;j<=l;j++) { //l is num of commands
		if (j!=l) {
			printf("In this loop\n");
			//int fd[2];
			if (j!=0) { //remember previous pipe
				//oldpipe[1] = newpipe[1];
				//oldpipe = newpipe;
			}
			//newpipe = fd;
			//newpipe[0] = fd[0];
			//newpipe[1] = fd[1];
			pipe(newpipe);
			frk = fork();
			if(frk==-1){
				perror("Fork");
				exit(-2);
			}
			i++;//counter for forks
		}
		//printf("newpipe[1]: %d\n",*newpipe);
		printf("Here %d\n", frk);
		if (j==0) {
			if (frk ==0) {
				//printf("fd[1]: %d\n",fd[1]);
				dup2(newpipe[1],STDOUT_FILENO);
				close(newpipe[0]);
				ret_val = execvp(cmds[j],args[j]);
				if (ret_val == -1){
					perror ("Error exec'ing");
					exit(-2);
				}
			}
			printf("Closing FD: %d\n", newpipe[1]);
			close(newpipe[1]);
		}
		
		if (j==l) {  //if last command?
			frk = fork();
			if(frk==-1){
				perror("Fork j=l");
				exit(-2);
			}
			i++;
			if (frk==0) {
				dup2(oldpipe[0],STDIN_FILENO);
				ret_val = execvp(cmds[j],args[j]);
				if (ret_val == -1){
					perror ("Error exec'ing 2");
					exit(-2);
				}
				//close(oldpipe[1]);
			}
			printf("Closing FD: %d\n", oldpipe[0]);
			close(oldpipe[0]);
		}

		if( (j != l) && (j != 0) )
		{ //Now in the middle commands
			if (frk==0) {
				dup2(newpipe[1],STDOUT_FILENO);
				dup2(oldpipe[0],STDIN_FILENO);
				close(newpipe[0]);
				ret_val = execvp(cmds[j],args[j]);
				if (ret_val == -1){
					perror ("Error exec'ing 3");
					exit(-2);
				}
				//close(oldpipe[1]);
			}
			printf("Closing FD: %d\n", newpipe[1]);
			close(newpipe[1]);
			printf("Closing FD: %d\n", oldpipe[0]);
			close(oldpipe[0]);
		}
		
		oldpipe[0] = newpipe[0];
		waitpid(frk,NULL,0);

	}


	//}
	
	/*
	int q;
	for (q=0;q<i;q++) {
		waitpid(-1,NULL,0);
	}
	*/

}






int main(int argc, char* argv[]){
	
	int max_args = 15;
	int max_argv_size = max_args + 2;
	char* cmds_args[max_args];
	char* my_argv[max_argv_size];
	char* cmds[max_args];
	char** args[max_args];
	int bufferSize=256;
	char buffer[bufferSize];
	
	int l;
	
	
	while(1){
	
	printf("Enter input: ");
	
	//printf("Home directory: %s\n", getenv("HOME"));
	
	char* dir = getcwd(buffer, bufferSize)+strlen(getenv("HOME"));
	
	printf("slush|%s>", dir);
	
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
	l=i;
	//int l = i; //remember i for later
	for (i; i>=0; --i) {
		cmds[j] = strtok(cmds_args[i]," \n"); //array of commands only
		//*CMDS IN EXECUTION ORDER, LAST IS FIRST
		//char* my_argv[max_argv_size];
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
		
		args[j] = my_argv; //array of my_argv's correlates with cmd[]
		j++;
	}
	
	
	func(l, cmds, args);
	
	}
	
	return 0;
}
