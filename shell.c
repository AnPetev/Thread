#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define BUFFER_SIZE 1024
#define TOK_BUFFER_SIZE 100
#define TOK_DELIM "\t\n"
void lsh_loop(){
	char *line;
	char **args;
	int status;
	while(status){
		line = lsh_read_line();
		args = lsh_split_line(line);
		status = lsh_execute(args);
		free(line);
		free(args);
	}
}
char *lsh_read_line(){
	int bufsize = LSH_RL_BUFSIZE;
	int position = 0;
	char *buffer = malloc(sizeof(char) * bufsize);
	int c;

	if(!buffer){
		fprintf(stderr,"allocation error");
		exit(EXIT_FAILURE);
	}
while(1){
	c = getchar();
	if(c == EOF || c=='\n'){
		buffer[position] = '\0';
		return buffer;
	}
	else{
		buffer[position] = c;
	}
	position++;

	if(position >= bufsize){
		bufsize += LSH_RL_BUFFSIZE;
		buffer=realloc(buffer,bufsize);
		if(!buffer){
			fprintf(stderr,"allocation error");
			exit(EXIT_FAILURE);
		}
	}
}
}
char **lsh_split_line(char *line){
	int bufsize = LSH_TOK_BUFSIZE,position = 0;
	char **tokens = malloc(bufsize * sizeof(char*));
	char *token;

	if(!tokens){
		fprintf(stderr,"allocation error");
		exit(EXIT_FAILURE);
	}
	token = strtok(line,LSH_TOK_DELIM);
	while(token !=NULL){
		tokens[position] = token;
		position++;
	if(position >= bufsize){
		bufsize += LSH_TOK_BUFSIZE;
		tokens=realloc(tokens,bufsize * sizeof(char*));
		if(!tokens){
			fprintf(stderr,"allocation error");
			exit(EXIT_FAILURE);
		}
	}
	token=strtok(NULL,LSH_TOK_DELIM);
	}
	tokens[position] = NULL;
	return tokens;
}
int lsh_launch(char ** args){
	pid_t pid,wpid;
	int status;
	pid = fork();
	if(pid==0){
		if(execvp(args[0],args)==-1){
			perror("lsh");
		}
		exit(EXIT_FAILURE);
	}
	else if(pid<0){
		perror("lsh");
	}
	do{
		wpid=waitpid(pid,&status,WUNTRACED);
	}
	while(!WIFEXITED(status) && !WIFSIGNALED(status));
}
return 1;
}
int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);

char *builtin_str[] = {"cd","help","exit"};
int(*builtin_func[])(char **) = {&lsh_cd,&lsh_help,&lsh_exit};
int lsh_num_builtins(){
	return sizeof(builtin_str)/sizeof(char*);
}
int lsh_cd(char **args){
	if(args[1] == NULL){
		fprintf(stderr,"expected argument");
	}
	else{
		if(chdir(args[1]) != 0){
			perror("lsh");
		}
	}
	return 1;
}
int lsh_help(char **args){
	int i;
	for(i=0;i<lsh_num_builtins();i++){
		printf("%s",builtin_str[i]);
	}
	return 1;
}

int lsh_exit(char **args){
	return 0;
}
int lsh_execute(char **args){
	int i;
	if(args[0] == NULL){
		return 1;
	}
	for(i=0;i<lsh_num_builtins();i++){
		if(strcmp(args[0],builtin_str[i]==0))
		
{
	return (*builtin_func[i])(args);
}
}return lsh_launch(args);
}
int main(int argc,char* argv[]){
	inf_loop();
}
			}
