#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

/*structs*/
struct Redirection
{
	char **tokens;
	char **input;
	char **output;
};

struct Node
{
	char *prev_dir;
	char *curr_dir;
	struct Node *children;
};

/* tree */
struct Directories
{
	struct Node *root;
	int size;
};

struct Env
{
	char *home;
	char *pwd;
	char *shell; /*??*/
	char *user;
	char *path; /*??*/
	char *oldpwd;
};
/*structs*/

/*functions*/
char *Read_Line()
{
	char *buffer = malloc(sizeof(char)*(256));
	int ch_count = 0;
	char ch;

	/* Read in command line */
	ch = getchar();
	while(ch != '\n')
	{
		buffer[ch_count] = ch;
		ch = getchar();
		ch_count++;
	}
	buffer[ch_count] = '\0';

	if(ch_count > 255)
	{
		/*stop = 1;  run an exit strategy >>>>*/
	}
	/* Full command line read in*/

	return buffer;
}


/* Embedded strings*/
char *env_var[] = {"$HOME", "$PWD", "$SHELL", "$USER", "$PATH", "$OLDPWD"};
char *built_in[] = {"exit", "ls", "cd", "echo"};


int main()
{
	char *buffer; 
	char delimiters[7] = {'\n', ' ', '<', '>', '|', '&'};
	int i;
	int stop = 0;

	while (1)
	{
		/* Shell prompt*/
		printf("$HOME:$PWD <> ");

		buffer = malloc(sizeof(char)*(256));
		buffer = Read_Line();

		printf("%s\n", buffer);

		/*empty input*/
		if(buffer[0] == '\0')
			continue;
		
		/* First char invalid*/
		for(i = 0; i < 7; i++)
		{
			if(buffer[0] == delimiters[i])
			{
				stop = 1;
				break;
			}	
		}

		/* Exiting shell but needs to upon finding "exit"*/
		if(stop == 1)
		{
			printf("Exiting shell... \n\n");
			break;
		}

		free (buffer);
		/*
		if(n_word == "exit")
			break;*/

		/* Exiting shell*/
	}

	return 0;
}
