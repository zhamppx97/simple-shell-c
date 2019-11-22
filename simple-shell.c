#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 80
#define BUFFER_SIZE 50
#define buffer "\n\Shell Command History:\n"

char history[10][BUFFER_SIZE];
int count = 0;

void displayHistory()
{
	printf("Shell command history:\n");
    	int i;
    	int j = 0;
    	int histCount = count;
    	for (i = 0; i < 10; i++)
    	{
        	printf("%d. ", histCount);
        	while (history[i][j] != '\n' && history[i][j] != '\0')
        	{
            		printf("%c", history[i][j]);
            		j++;
        	}
        	printf("\n");
        	j = 0;
        	histCount--;
        	if (histCount ==  0)
            		break;
    	}
    	printf("\n");
} 

int formatCommand(char inputBuffer[], char *args[], int *flag)
{
	int length;
    int i;
    int start;
    int ct = 0;
    int hist;

 	length = read(STDIN_FILENO, inputBuffer, MAX_LINE);	
   
	start = -1;
    	if (length == 0)
        	exit(0);
    	if (length < 0)
    	{
        	printf("Command not read\n");
        	exit(-1);
    	}
    	for (i = 0; i < length; i++)
    	{
        	switch (inputBuffer[i])
        	{
            		case ' ':
            		case '\t' :
                		if(start != -1)
                		{
                    			args[ct] = &inputBuffer[start];    
                    			ct++;
                		}
                		inputBuffer[i] = '\0';
                		start = -1;
                		break;
            		case '\n': 
                		if (start != -1)
                		{
                    			args[ct] = &inputBuffer[start];
                    			ct++;
                		}
                		inputBuffer[i] = '\0';
                		args[ct] = NULL;
                		break;
            		default :           
                		if (start == -1)
                    			start = i;
                		if (inputBuffer[i] == '&')
                		{
                    			*flag  = 1;
                    			inputBuffer[i] = '\0';
                		}
        	}
	}
    
	args[ct] = NULL;

	if(strcmp(args[0], "history") == 0)
        {		
		if(count > 0)
		{		
                	displayHistory();
		}
		else
		{
			printf("\nNo Commands in the history\n");
		}
		return -1;
        }
	else if (args[0][0]-'!' == 0)
	{	
		int x = args[0][1]- '0'; 
		int z = args[0][2]- '0'; 
		
		if(x > count)
		{
			printf("\nNo Such Command in the history\n");
			strcpy(inputBuffer,"Wrong command");
		} 
		else if (z != -48)
		{
			printf("\nNo Such Command in the history. Enter <=!9 (buffer size is 10 along with current command)\n");
			strcpy(inputBuffer,"Wrong command");
		}
		else
		{
			if(x == -15)
			{	 
				strcpy(inputBuffer,history[0]);
			}
			else if(x == 0)
			{	 
				printf("Enter proper command");
				strcpy(inputBuffer,"Wrong command");
			}			
			else if(x >= 1)
			{
				strcpy(inputBuffer,history[count-x]);
			}
			
		}
	}
	for (i = 9; i > 0; i--)
       		strcpy(history[i], history[i-1]);
    		strcpy(history[0],inputBuffer);
    		count++;
		if(count > 10)
		{
			count=10;
		}
}

int main(void)
{
    char inputBuffer[MAX_LINE];
    int flag;
    char *args[MAX_LINE / 2 + 1];
    int should_run = 1;
    
    pid_t pid,tpid;
    int i;
   
    while (should_run)
    {            
        flag = 0;
        printf("osh 5951001121:~$ > ");
        fflush(stdout);
        if(-1 != formatCommand(inputBuffer, args, &flag))
	    {
		    pid = fork();
        
        	if (pid < 0)
        	{
            		printf("Fork failed.\n");
            		exit (1);
        	}
       		else if (pid == 0)
        	{
            		if (execvp(args[0], args) == -1)
           	 	{	
                		printf("Error executing command\n");
            		}
       		}
        	else
        	{
            		i++;
           	 	if (flag == 0)
           		{
                		i++;
                		wait(NULL);
           		}
        	}
   	    }
    }
}