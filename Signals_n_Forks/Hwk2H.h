#ifndef HWK2H_H
#define HWK2H_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

//
//		Definition of constants:
//

//  PURPOSE:  To tell the name of the program the guessing process should
//	execute.
#define		GUESSER_PROG		"Guesser"


//  PURPOSE:  To tell the name of the program the answering process should
//	execute.
#define		ANSWERER_PROG		"Answerer"

//  PURPOSE:  To tell the time limit in seconds.
const int	TIME_LIMIT_NUM_SECONDS = 13;

//  PURPOSE:  To tell the signal the answerer send to the guesser when the
//	wrong digit is guessed.
const int	INCORRECT_SIG = SIGUSR1;

//  PURPOSE:  To tell the signal the answerer send to the guesser when the
//	right digit is guessed.
const int	CORRECT_SIG = SIGUSR2;

//  PURPOSE:  To tell the signal the guesser send to the answerer to guess
//	digit '0' at the current binary position.
const int	ZERO_SIG = SIGUSR1;

//  PURPOSE:  To tell the signal the guesser send to the answerer to guess
//	digit '1' at the current binary position.
const int	ONE_SIG = SIGUSR2;

#endif