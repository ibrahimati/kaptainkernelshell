#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>



int main()
{
	char buffer[256];
	char delimiters[7] = {'\n', ' ', '<', '>', '|', '&'};
	int i;
	int stop = 0;
	char n_word[5] = "open";
	int ch_count = 0;
	char ch;

	while (1)
	{
		// ch_count reset
		ch_count = 0;

		// Shell prompt
		printf("$HOME:$PWD <> ");

		// Read in command line
		ch = getchar();
		while(ch != '\n')
		{
			buffer[ch_count] = ch;
			ch = getchar();
			ch_count++;
		}
		buffer[ch_count] = '\0';

		if(ch_count == 0)
			continue;

		if(ch_count > 255)
		{
			stop = 1;
		}
		// Full command line read in

		printf("%s\n", buffer);
		
		// First char invalid
		for(i = 0; i < 7; i++)
		{
			if(buffer[0] == delimiters[i])
			{
				stop = 1;
				break;
			}	
		}

		// Exiting shell
		if(stop == 1)
		{
			printf("Exiting shell... \n\n");
			break;
		}

		if(n_word == "exit")
			break;
		// Exiting shell
	}

	return 0;
}
