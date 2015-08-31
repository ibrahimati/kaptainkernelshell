#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>


int main()
{
	char buffer[256];
	char delimiters[7] = {'\n',' ', '<', '>', '|', '&'};
	int i;
	int stop = 0;

	while (1)
	{

		printf("$HOME:$PWD <> ");

		fgets(buffer, 255, stdin);

		printf("%s \n", buffer);
		
		for(i = 0; i < 7; i++)
		{
			if(buffer[i] == delimiters[i])
			{
				stop = 1;
				break;
			}	
		}

		if(stop == 1)
		{
			break;
		}
	}

	return 0;
}
