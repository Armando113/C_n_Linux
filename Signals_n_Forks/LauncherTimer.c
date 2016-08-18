#include <stdio.h>

#include "Hwk2H.h"

//
//		Global variable definition:
//

//  PURPOSE:  To hold '1' when the time is up, or '0' before then.
int	isTimeUp = 0;


//  PURPOSE:  To hold '1' after the child has stopped, or '0' before then.
int	didChildStop = 0;


//
//		Global function definition:
//

//  PURPOSE:  To handle when the child process has stopped by unzombie-ing it.
//  	  Ignores 'sig'.  No return value.
void sigChildHandler(int sig)
{
	//  wait for the child so it does not stay a zombie process
	
	pid_t pid;
	int signal;

	while ((pid = waitpid(-1, &signal, WNOHANG)) > 0)
	{
		if (WIFEXITED(signal) != 0)
		{
			printf("Done!\n");
		}
	}

	if (isTimeUp)
	{
		printf("Timer: \"Sorry, time is up!\"\n");
	}
	else
	{
		printf("Timer: \"Congratulations!\"\n");
	}

	didChildStop = 1;
}


//  PURPOSE:  To run the program.  Ignores parameters.  Sends 'EXIT_SUCCESS'
//	to OS.
int main()
{
	//  I.  Application validity check:
	struct sigaction action;

	memset(&action, '\0', sizeof(struct sigaction));
	action.sa_handler = sigChildHandler;
	sigaction(SIGCHLD, &action, NULL);

	//  II.  Do program:
	pid_t childId;
	char *answerBuff = "Answerer";

	//  Do something here
	//Fork it home baby! Launch the Answerer exe
	childId = fork();
	if (childId == 0)
	{
		execl(ANSWERER_PROG, ANSWERER_PROG, NULL);
		fprintf(stderr, "Could not run %s :'(\n", ANSWERER_PROG);
		exit(EXIT_FAILURE);
	}


	int secs;

	for (secs = TIME_LIMIT_NUM_SECONDS; (secs > 0) && !didChildStop; secs--)
	{
		printf("Timer: \"%d seconds\"\n", secs);
		sleep(1);
	}

	isTimeUp = 1;

	if (!didChildStop)
	{
		printf("Timer: \"%d seconds\"\n", secs);
		//  Stop the child process by sending it SIGINT
		kill(childId, SIGINT);
	}

	while (!didChildStop)
	{
		sleep(1);
	}

	return(EXIT_SUCCESS);
}