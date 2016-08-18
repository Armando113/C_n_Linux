#include "Hwk2H.h"

//
//		Global variable definition:
//

//  PURPOSE:  To hold the process id of the process that guesses the number
//	held by this ansewering process.
pid_t guesserId;


//  PURPOSE:  To hold the answer to be guessed.
int	answer;


//  PURPOSE:  To hold the binary place currently being guessed.
int	binaryPlace;


//  PURPOSE:  To hold '1' while this program should continue to run, or '0'
//	otherwise.
int	shouldContinue = 1;


//
//		Global function definition:
//

//  PURPOSE:  To handle when the guesser correctly guess that digit 'digit' is
//	at binary position 'binaryPlace'.  No return value.
void correctGuess(int digit)
{
	if (digit)
	{
		printf("Answerer: \"Yes, there is a %d.\"\n", binaryPlace);
	}
	else
	{
		printf("Answerer: \"Yes, there is no %d.\"\n", binaryPlace);
	}

	binaryPlace = binaryPlace >> 1;
	//  Tell the guessing process that it guessed correctly
	kill(guesserId, CORRECT_SIG);
}


//  PURPOSE:  To handle when the guesser incorrectly guess that digit 'digit' is
//	at binary position 'binaryPlace'.  No return value.
void incorrectGuess(int	digit)
{
	printf("Answerer: \"Sorry, guess again.\"\n");
	//  Tell the guessing process that it guessed incorrectly
	kill(guesserId, INCORRECT_SIG);
}


//  PURPOSE:  To handle when the guesser guesses that a '0' is at binary
//	position 'binaryPlace'.  Ignores 'sig'.  No return value.
void zeroSigHandler(int	sig)
{
	if ((answer & binaryPlace) == 0)
	{
		correctGuess(0);
	}
	else
	{
		incorrectGuess(0);
	}
}


//  PURPOSE:  To handle when the guesser guesses that a '1' is at binary
//	position 'binaryPlace'.  Ignores 'sig'.  No return value.
void oneSigHandler(int sig)
{
	if ((answer & binaryPlace) == binaryPlace)
	{
		correctGuess(1);
	}
	else
	{
		incorrectGuess(1);
	}
}


//  PURPOSE:  To handle when the timing process tells this process that the
//	time is up.  Ignores 'sig'.  No return value.
void sigIntHandler(int sig)
{
	printf("Answerer: \"Whoops, time is up!\"\n");
	shouldContinue = 0;
	//  Tell the guessing process that the time is up by sending it SIGINT
	//Kill your child (you savage!!)
	kill(guesserId, SIGINT);
}


//  PURPOSE:  To run answering program.  Ignore command line arguments.
//	Returns 'EXIT_SUCCESS' to OS.
int		main()
{
	srand(getpid());
	rand();
	answer = rand() % 128;
	binaryPlace = 1 << 7;

	pid_t myID = getpid();

	printf("Answerer: \"(Don't tell, but the answer is %d)\"\n", answer);

	//  Install signal handlers
	struct sigaction myAct;
	memset(&myAct, '\0', sizeof(struct sigaction));
	myAct.sa_handler = sigIntHandler;
	sigaction(SIGINT, &myAct, NULL);

	//Zero action
	struct sigaction zeroAct;
	memset(&zeroAct, '\0', sizeof(struct sigaction));
	zeroAct.sa_handler = zeroSigHandler;
	sigaction(ZERO_SIG, &zeroAct, NULL);

	//One Action
	struct sigaction oneAct;
	memset(&oneAct, '\0', sizeof(struct sigaction));
	oneAct.sa_handler = oneSigHandler;
	sigaction(ONE_SIG, &oneAct, NULL);

	//  Start guessing process, put process id in 'guesserId'
	guesserId = fork();
	if (guesserId == 0)
	{
		execl(GUESSER_PROG, GUESSER_PROG, NULL);
		fprintf(stderr, "Could not run %s :'(\n", GUESSER_PROG);
		exit(EXIT_FAILURE);
	}

	while (shouldContinue && (binaryPlace > 0))
	{
		sleep(1);
	}

	printf("Answerer signing out!\n");

	// Wait for guessing process here
	return(EXIT_SUCCESS);
}