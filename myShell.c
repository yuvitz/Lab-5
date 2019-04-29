#include <linux/limits.h>
#include "LineParser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <unistd.h>




#define MAX_BUF_SIZE 2048

char *getPath(cmdLine *pCmdLine){
	char currDir[PATH_MAX];
	getcwd(currDir, PATH_MAX);
	int pathLength=strlen(currDir)+strlen(pCmdLine->arguments[0])+1;
	char *path= calloc(pathLength, sizeof(char));
	strcpy(path, currDir);
	strcpy(path+strlen(currDir), "/");
	strcpy(path+strlen(currDir)+1, pCmdLine->arguments[0]);
	return path;
}

void changeDir(char *path){
	if(chdir(path)==-1)
		fprintf(stderr, "cd: %s - no such path \n", path);
}

int execute(cmdLine *pCmdLine){
	int procId=0;
	char *path = getPath(pCmdLine);
	if(strncmp("cd", pCmdLine->arguments[0], 2*sizeof(char))==0)
		changeDir(pCmdLine->arguments[1]);
	else{
		procId=fork();
		if(procId==0)
			if(execvp(path, pCmdLine->arguments)==-1)
				_exit(1);	
	}
	free(path);
	return procId;
}

void quit(cmdLine *firstCmdLine){
	freeCmdLines(firstCmdLine);
	exit(0);
}

int main (int argc, char **argv){
	pid_t cpid=0;
	int first=0;
	char currDir[PATH_MAX];
	char buffer[MAX_BUF_SIZE];
	char *str= fgets(buffer, MAX_BUF_SIZE, stdin);
	cmdLine *firstLine, *line;
	firstLine=parseCmdLines(str);
	while(1){
		getcwd(currDir, PATH_MAX);
		printf("%s\n", currDir);
		if(first==0){
			line=firstLine;
			first=1;
		}
		else
			line= parseCmdLines(str);
		if(strcmp(line->arguments[0], "quit")==0)
			quit(firstLine);
		cpid= execute(line);
		if(line->blocking==1){
			waitpid(cpid, NULL, 0);
			printf("%s\n", "after sleep");
		}
			str= fgets(buffer, MAX_BUF_SIZE, stdin);
	}
}