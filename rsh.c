#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define N 12

extern char **environ;

char *allowed[N] = {"cp","touch","mkdir","ls","pwd","cat","grep","chmod","diff","cd","exit","help"};

int isAllowed(const char*cmd) {
	// return 1 if cmd is one of the allowed commands
	// return 0 otherwise
	char* cpyCMD = malloc(200);
	strcpy(cpyCMD, cmd);
	cpyCMD = strtok(cpyCMD, " ");
	for (int i=0; i<=11; i++) {
		if (strcmp(cpyCMD,allowed[i]) == 0) {
			return 1;
		}
	}
	free(cpyCMD);
	return 0;
}

int main() {

    // TODO
    // Add variables as needed

    char line[256];
    char tknLine[256];
    char *currentTkn = malloc(200);
    pid_t pid;
    int status;
    posix_spawnattr_t attr;
    char **argv = malloc(20*sizeof(char*));
    int tknCounter = 0;

    while (1) {
	tknCounter = 0;
        for (int i=0; i<20; i++) {
            argv[i] = calloc(1, 200);
	}

	fprintf(stderr,"rsh>");

	if (fgets(line,256,stdin)==NULL) continue;

	if (strcmp(line,"\n")==0) continue;

	line[strlen(line)-1]='\0';
	strcpy(tknLine,line);
	if (!isAllowed(line)) {
		printf("Not Allowed!\n");
		continue;
	}

	currentTkn = strtok(tknLine, " ");
	while (currentTkn != NULL) {
		strcpy(argv[tknCounter], currentTkn);
		tknCounter++;
		currentTkn = strtok(NULL, " ");
	}
	argv[tknCounter] = NULL;
	// TODO
	// Add code to spawn processes for the first 9 commands
	// And add code to execute cd, exit, help commands
	// Use the example provided in myspawn.c
	
	if (strcmp(argv[0], "cd") == 0) {
		if (argv[2] != NULL) {
			printf("-rsh: cd: too many arguments\n");
			continue;
		}
		chdir(argv[1]);
	} else if (strcmp(argv[0], "exit") == 0) {
		return 0;
	} else if (strcmp(argv[0], "help") == 0) {
		printf("The allowed commands are:\n1: cp\n2: touch\n3: mkdir\n4: ls\n5: pwd\n6: cat\n7: grep\n");
		printf("8: chmod\n9: diff\n10: cd\n11: exit\n12: help\n");
	} else {
		posix_spawnattr_init(&attr);
		if (posix_spawnp(&pid, argv[0], NULL, &attr, argv, environ) != 0) {
			perror("spawn failed");
			exit(EXIT_FAILURE);
		}
		if (waitpid(pid, &status, 0) == -1) {
			perror("waitpid failed");
			exit(EXIT_FAILURE);
		}
		posix_spawnattr_destroy(&attr);
	}
	for (int i=0; i<20; i++) {
            free(argv[i]);
	}
    }
    return 0;
}
