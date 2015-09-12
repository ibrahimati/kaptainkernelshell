#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>

/* Class website: http://www.cs.fsu.edu/~cop4610t/index.html*/
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
char **Parse_Args(char*, int);
char **Expand_Vars(char**, int, int*, int*);
char **Resolve_Paths(char**, int);

int Relative_Root(char*);
int Relative_Current(char*);
int Relative_Previous(char*);
int Relative_Home(char*);
int Relative_Path(char*);


/* Embedded strings*/
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

		cmd = malloc(ch_count * sizeof(char*));
		
		cmd = Parse(buffer);

		/*empty input */
		if(buffer[0] == '\0')
			continue;
		
		printf("\n");

		free (buffer);
		free (cmd);
	}

	/* Exiting shell but needs to upon finding "exit"*/
	printf("Exiting shell... \n\n");
	exit(0);
	/*close all background processes*/
	
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
	int num_args = 0; /*number of total arguments*/
	int l_size = strlen(line); /*line size */
	int var_count = 0; /*number of environmental variables*/
	args = malloc(l_size * sizeof(char*)); /*dynamically allocate space to args*/
	args[0] = NULL; /*initialize it at 0*/

	line = Parse_Whitespace(line, &num_args); /*call function to remove extra whitespace in line*/
	
	printf("%s %d", line, num_args);

	args = Parse_Args(line, num_args); 

	args = Expand_Vars(args, num_args, &var_count); /*call function to epand your environmental variables*/
		if(var_count < 0)
			break;/*entire line is invalid    is this right way to do it?*/

	args = Resolve_Paths(args, num_args);

	return args;
}

char *Parse_Whitespace(char *line, int *count)
{
	int l_size = strlen(line); /*line size */
	char *temp = malloc(l_size); /*holds new line with only one whitespace between args */
	char delimiters[6] = {'|', '<', '>', '&', '$'};
	int i;

	int itr;
	int j = 0;
	int f = 0;	/*placeholder for count */
	int no_space = 0;  /*when no space between args */
	int f_space = 0;

	for(i = 0; i < l_size;)
	{
		/* for front spaces*/
		while(line[i] == ' ')
			i++;

		while(line[i] != '\0')
		{
			if (line[i] == ' ')
			{	
				f_space = 1; 
				break; 
			}

		
			if(line[i] == '.')
			{	
				temp[j] = line[i];
				j++;
				i++;

				if(line[i] == '.')
				{
					temp[j] = line[i];
					j++;
					i++;
				}	
			}

			if(line[i] == '~')
			{
				temp[j] = line[i];
				j++;
				i++;
			}

			if(line[i] == ',')
			{	
				temp[j] = line[i];
				j++;
				i++;
			}

			if(line[i] == '/')
			{
				temp[j] = line[i];
				j++;
				i++;
			}

			for(itr = 0; itr < 6; itr++)
			{
				if(line[i] == delimiters[itr])
				{
					no_space = 1;

					if(no_space && isalpha(line[i-1]))
					{
						temp[j] = ' ';
						j++;
					}

					temp[j] = line[i];
					f++;
					j++;
					temp[j] = ' ';
					j++;
					i++;
					f++;
				}

				if(no_space)
					break;
					
			}

			if(no_space)
			{
				temp[j] = ' ';
				break;
			}
		
			temp[j] = line[i];
			i++;
			j++;

			if(line[i] == '\0')
				break;
		}



		if(f_space)
		{
			temp[j] = ' ';
			j++;
			i++;

			/*handles line with mult file declarations */
			if(line[i-1] == ' ' && (isalpha(line[i+1]) || line[i] == ' ')) 
				f++;

			if(line[i] == '.')
				f++;

			if(line[i] == '~')
				f++;
		}	
		else if(j != 0 && !no_space)
		{	
			temp[j] = ' ';
			j++;
			f++;
		}


		while(line[i] == ' ')
		{
			i++;
		}

		f_space = 0;
		no_space = 0;

		/* Handles excess whitespace after cmd*/
		if(line[i] == '\0' && line[i-1] == ' ')
		{	
			temp[j] = ' ';
			j++;
			f++;
			i = l_size + 1;
			break; 
		}

		/*printf("Hereee '%c'\n", line[i]);*/
	}

	temp[j] = '\0';

	*count = f;

	return temp;
}


char **Parse_Args(char *line, int count)
{
	char **args = malloc(count * sizeof(char*));
	int itr;
	int i = 0;
	int j = 0;
	int x = 0;

	for(itr = 0; itr < count; itr++)
		args[itr] = (char *)malloc(256);

	for(i = 0; line[i] != '\0';)
	{
		if(line[i] == ' ')
		{
			j++;
			x = 0;
			i++;
			continue;
		}

		args[j][x] = line[i];
		i++;
		x++;
		args[j][x] = '\0';
	}

	return args;
}

char **Expand_Vars(char **args, int num_args, int *var_count)
{

	/*variables*/
	int i;
	int j;
	int var_size = 0;

	/*allocate space for holding in variable chars*/
	char *vars = malloc(num_args * sizeof(char)); /*for the variable itself*/
	/*delimiters*/
	char dollaS = '$'; 


	for (i=0; i <num_args; i++)
	{
	  if (args[i][0] == dollaS) /*if the 1st char is a dollar sign*/
	  {
	    for (j=1; j <= strlen(args[i]); j++) /*start with next char in array*/ 
	    {
	      vars[var_size] = toupper(args[i][j]); /*copy it over and convert it to uppercase*/
	      *var_size++; /*increment array size counter*/
/*might not need*/if (args[i][j] == NULL) /*if reached end of the variable name*/	         
	    	{
			 *var_count++; /*increment variable counter*/
	    	 vars[var_size] = NULL; /*insert null character in c string*/
	         var_size = 0; /*reset counter back to 0*/
	         args[i] = getenv(vars); /*call getenv function and insert return in args*/
	         break; /*incase there's null*/
	        }/*end if*/
	    }/*end for j*/
	    if(args[i][0]== NULL) /*if getenv didn't get anything*/
	   	{
	   		printf(vars, " is not a valid argument\n"); /*error message*/
	   		var_count = -1; /*set flag so we know command is invalid on return*/
	   		break;		/*exit i for loop */
	   	}
	   	memset(vars,0,strlen(vars)); /*clear vars cstring*/
	  } /*end if dollaS*/
	}/*end for i*/
	
/*this stuff was in loop but we may need it for export or novel command instead
char *value = malloc(num_args * sizeof(char)); if we have to set it to something
bool set = false; set the bool at negative to start
int k = 0;   counter for the value array 
int success; for storing result of setenv
for loop
{
if (args[i][j]=='=') test for = sign
	set = true; trigger the bool
if ( set == true) test for set
	{
		value[k] = args[i][j]; store everything after into value
		
	}		
else  keep doing regular copy to vars
}
if (set == true)
{
	set = false; reset this back for later use
	success = setenv(vars,value, 1); call setenv function
	k= 0; reset counter to 0
	if (success == -1)test if command succeeded
	{
		printf ("Set command was not carried out properly. Check readme for further help."); print error message
		break; 
	}
}	
*/

	return args; /*return array but with variables in lookup format*/
}

char **Resolve_Paths(char **args, int size)
{
	int i;
	int j;
	int exists = 0;
	char *temp1 = getenv("PWD");
	char *temp2;			/* copy of PWD so don't change it*/

	j = strlen(temp1);
	temp2 = malloc(j*sizeof(char*));

	strcpy(temp2, temp1);

	for(i = 0; i < size; i++)
	{
		if(args[i][0] == '/')
			exists = Relative_Root(args[i]);
		else if(args[i][0] == '.' && args[i][1] == '/')
			exists = Relative_Current(args[i]);
		else if(args[i][0] == '.' && args[i][1] == '.' && args[i][2] == '/')
			exists = Relative_Previous(args[i]);
		else if(args[i][0] == '.' && args[i][1] == '.') /* for cd .. */
		{
			for(; j > 0; j--)
			{
				if(temp2[j] == '/')
				{
					temp2[j] = '\0';
					break;
				}
			}
			
			strcpy(args[i],temp2);
		}
		else if(args[i][0] == '~' && args[i][1] == '/')
		{
			exists = Relative_Home(args[i]);
		}
		/*else
			exists = Relative_Path(args[i]);*/

	}

	free(temp2);	
	/* try to open files if it exists then good, if its a command fork process*/
	if(!exists)
		printf("Hereeeeeee\n");

	return args;
}


/* Compares arg to the PWD.
	This function is met when a path starts with '/'*/
int Relative_Root(char *arg)
{
	char *temp = getenv("PWD");

	if(strcmp(temp, arg) == 0)
		return 1;

	return 0;
}

/* For a path that starts with './'
	This function takes PWD and concats to the front of string taking 
	out the '.'*/
int Relative_Current(char *arg)
{
	char *temp1 = getenv("PWD");
	int size1 = strlen(arg);
	int size2 = strlen(temp1);
	char *temp2 = malloc(size1*sizeof(char*));
	char *temp3 = malloc(size1*size2*sizeof(char*));
	int i;
	int j = 1;

	for(i = 0; i < size1; i++)
	{
		temp2[i] = arg[j];
		j++;
	}


	for(i = 0; i < size2; i++)
		temp3[i] = temp1[i];

	strcat(temp3, temp2);
	
	free(arg);
	
	arg = malloc(size1*size2*sizeof(char*));
	arg = temp3;

	free(temp2);

/* need to finish */
	return 0;
}


int Relative_Previous(char *arg)
{
	char *temp1 = getenv("PWD");
	int size1 = strlen(arg);
	int size2 = strlen(temp1);
	char *temp2 = malloc(size1*sizeof(char*));
	char *temp3 = malloc(size1*size2*sizeof(char*));
	char *temp4 = malloc(size2*sizeof(char*));
	int i;
	int j = 2;

	strcpy(temp4, temp1);

	for(i = 0; i < size1; i++)
	{
		temp2[i] = arg[j];
		j++;
	}

	for(i = size2; i > 0; i--)
	{
		if(temp4[i] == '/')
		{
			temp4[i] = '\0';
			break;
		}
	}

	for(i = 0; temp4[i] != '\0'; i++)
		temp3[i] = temp4[i];

	strcat(temp3, temp2);
	free(arg);
	arg = malloc(size1*size2*sizeof(char*));
	arg = temp3;

	free(temp2);
	free(temp4);

	return 0;
}


int Relative_Home(char *arg)
{
	char *temp1 = getenv("HOME");
	int size1 = strlen(arg);
	int size2 = strlen(temp1);
	char *temp2 = malloc(size1*sizeof(char*));
	char *temp3 = malloc(size1*size2*sizeof(char*));
	int i;
	int j = 1;

	for(i = 0; i < size1; i++)
	{
		temp2[i] = arg[j];
		j++;
	}
	temp2[i] = '\0';


	for(i = 0; i < size2; i++)
		temp3[i] = temp1[i];

	strcat(temp3, temp2);
	free(arg);
	arg = malloc(size1*size2*sizeof(char*));
	arg = temp3;

	free(temp2);

	return 0;
}

int Relative_Path(char *arg)
{
	return 0;
}