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
/*structs*/

/*function prototypes*/
char *Read_Line(int*);
char **Parse(char*);
char *Parse_Whitespace(char*, int*);
char **Parse_Args(char*);
char **Expand_Vars(char**);
char **Resolve_Paths(char**);


/* Embedded strings*/
char *env_var[] = {"$HOME", "$PWD", "$SHELL", "$USER", 
					"$PATH", "$OLDPWD", "$UNDEFINED"};
char *built_in[] = {"exit", "ls", "cd", "echo"};


int main()
{
	char *buffer; 
	char **cmd;
	int ch_count = 0;

	while (1)
	{
		/* Shell prompt*/
		printf("%s@%s:%s <> ", getenv("HOME"), getenv("MACHINE"), getenv("PWD"));


		buffer = malloc(sizeof(char)*(256));
		
		buffer = Read_Line(&ch_count);

		cmd = malloc(20 * sizeof(char*));
		
		cmd = Parse(buffer);

		/*empty input*/
		if(buffer[0] == '\0')
			continue;

		printf("\n");

		free (buffer);
		free (cmd);
	}

	/* Exiting shell but needs to upon finding "exit"*/
	printf("Exiting shell... \n\n");

	return 0;
}



char *Read_Line(int *ch_count)
{
	char *buffer = malloc(sizeof(char)*(256));
	char ch;
	int i = 0; /* temp holder for ch_count */

	/* Read in command line */
	ch = getchar();
	while(ch != '\n')
	{
		buffer[i] = ch;
		ch = getchar();
		i++;
	}
	buffer[i] = '\0';

	*ch_count = i;
	/* Full command line read in*/

	return buffer;
}


char **Parse(char *line)
{
	char **args;
	int num_args = 0;

	args = malloc(20 * sizeof(char*));
	args[0] = NULL;

	line = Parse_Whitespace(line, &num_args);
	printf("%s", line);
	/*args = Parse_Args(line);
	args = Expand_Vars(args);
	args = Resolve_Paths(args);
	*/
	return args; /*args*/
}

char *Parse_Whitespace(char *line, int *count)
{
	int l_size = strlen(line); /*line size */
	char *temp = malloc(l_size); /*holds new line with only one whitespace between args */
	char delimiters[6] = {'|', '<', '>', '&', '$', '~'};
	int i; 
	int itr;
	int j = 0 ;
	int f = 0;	/*placeholder for count */
	int no_space = 0;  /*when no space between args */

	for(i = 0; i < l_size;)
	{
		while(line[i] != '\0')
		{
			if (line[i] == ' ')
				break; 

			temp[j] = line[i];
			i++;
			j++;


			/* for no spaces in cmd line */
			for(itr = 0; itr < 6; itr++)
			{
				if(line[i] == delimiters[itr])
				{
					f++;
					temp[j] = ' ';
					j++;
					temp[j] = line[i];
					f++;
					j++;
					temp[j] = ' ';
					j++;
					i++;
					f++;
					no_space = 1;
				}

				if(no_space)
					break;
			}
			
			if(no_space)
			{
				/*temp[j] = ' ';*/
				break;
			}
		}

		if(j != 0 && !no_space)
		{	
			temp[j] = ' ';/*line[i];*/
			j++;
			f++;
		}


		while(line[i] == ' ')
		{
			i++;
		}
	}

	*count = f;

	/*printf("%s %d", temp, f);*/

	return temp;
}


char **Parse_Args(char *line)
{
	return NULL;
}

char **Expand_Vars(char **args)
{
	return NULL;
}

char **Resolve_Paths(char **args)
{
	return NULL;
}