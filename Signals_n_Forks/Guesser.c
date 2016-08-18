#include "Hwk2H.h"

//
//		Global variable definition:
//

//  PURPOSE:  To hold the answer being constructed by this process.
int	answer;

//  PURPOSE:  To hold the current binary position being queried.
int	binaryPlace;

//  PURPOSE:  To hold '1' while this program should continue to run, or '0'
//	otherwise.
int	shouldContinue = 1;

//  PURPOSE:  To hold the number of guesses that have been executed for the
//	current value of 'binaryPlace'.
int	count = 0;

pid_t pPID;

//
//		Global function definition:
//

//  PURPOSE:  To inform the answering process of our first guess for the digit
//	at binary place 'binaryPlace'.  No parameters.  No return value.
void firstGuess()
{
	printf("Guesser: \"Is there a %d?\"\n", binaryPlace);
	sleep(1);
	//  Tell the answering (parent) process our guess is '1'
	kill(pPID, ONE_SIG);
}


//  PURPOSE:  To inform the answering process of our second guess (if
//	necessary) for the digit at binary place 'binaryPlace'.  No parameters.
//	No return value.
void secondGuess()
{
	count++;
	printf("Guesser: \"Whoops, I guess there is no %d.\"\n", binaryPlace);
	sleep(1);
	//  Tell the answering (parent) process our guess is '0'
	kill(pPID, ZERO_SIG);
}


//  PURPOSE:  To handle when the answering process informs us that the latest
//	guess was correct.  Ignores 'sig'.  No return value.
void correctSigHandler (int	sig)
{
	if (count == 0)
	{
		answer = answer | binaryPlace;
		printf("Guesser: \"So far I have %d\"\n", answer);
	}

	count = 0;
	binaryPlace = binaryPlace >> 1;

	if (binaryPlace == 0)
	{
		printf("Guesser: \"Yay!  I got the answer!  It is %d\"\n", answer);
	}
	else
	{
		firstGuess();
	}
}


//  PURPOSE:  To handle when the answering process informs us that the latest
//	guess was incorrect.  Ignores 'sig'.  No return value.
void incorrectSigHandler (int sig)
{
	secondGuess();
}


//  PURPOSE:  To handle when the answering process tells us that time time is
//	up.  Ignores 'sig'.  No return value.
void sigIntHandler(int sig)
{
	printf("Guesser: \"Aw man!  And I was close, too!\"\n");
	shouldContinue = 0;
}


//  PURPOSE:  To run the program that does the guessing.  Ignores command line
//	arguments.  Returns 'EXIT_SUCCESS' to OS.
int		main()
{
	answer = 0;
	binaryPlace = 1 << 7;

	pPID = getppid();

	// Install signal handlers
	//SIGINT
	struct sigaction intAct;
	memset(&intAct, '\0', sizeof(struct sigaction));
	intAct.sa_handler = sigIntHandler;
	sigaction(SIGINT, &intAct, NULL);

	//Incorrect Sig
	struct sigaction badAct;
	memset(&badAct, '\0', sizeof(struct sigaction));
	badAct.sa_handler = incorrectSigHandler;
	sigaction(INCORRECT_SIG, &badAct, NULL);

	//Correct Sig
	struct sigaction goodAct;
	memset(&goodAct, '\0', sizeof(struct sigaction));
	goodAct.sa_handler = correctSigHandler;
	sigaction(CORRECT_SIG, &goodAct, NULL);

	firstGuess();

	while (shouldContinue && (binaryPlace > 0))
	{
		sleep(1);
	}

	return(EXIT_SUCCESS);
}
