#ifndef __DECRYPT_RSA__
#define __DECRYPT_RSA__

#define _CRT_SECURE_NO_WARNINGS
//#define _ALLOW_COMPILER_AND_STL_VERSION_MISMATCH

#include <cstdio>
#include <cstdlib>
#include <ctime>

/* displays all printed chars as DEC values on console */
//#define DEBUG_MODE
#define DEBUG_NUMBER_OF_COLUMNS 4

/* simplifying macros */
#define TRUE 1
#define FALSE 0
#define FILE_NAME_LENGTH 256
#define LIMIT (CALC_INT)255
#define LOW_LIMIT (CALC_INT)227
#define START_POINT (CALC_INT)2

#define IS_IN_LETTER_RANGE(character) (((character[0] > (CALC_CHAR)32) && (character[0] < (CALC_CHAR)126)) ? character : (CALC_CHAR*)"Ee")

/* white characters definitions */
#define TAB 9
#define NEW_LINE '\n'
#define SPACE ' '


typedef unsigned long long CALC_INT;
typedef unsigned char CALC_CHAR;

enum TError
	{
	Enullptr = 0,
	EBadName,
	EBadOpen,
	EBadScan,
	ENegativePrime
	};


int close_file(FILE *aInFile, FILE *aOutFile = nullptr);
void close_program(const char *aArgv0, char *aInput = nullptr, char *aOutput = nullptr, FILE *aFin = nullptr, FILE *aFout = nullptr);
void open_input_file(char ** const Aargv, char *aFilename, FILE **aFile);
void open_output_file(const int Aargc, char ** const Aargv, char *aFilename, FILE **aFile);
const CALC_CHAR encrypt_character(const CALC_CHAR aChar, CALC_INT aExp, const CALC_INT aBound);
void decrypt_file(FILE * const aInfile, FILE * const aOutfile);
const CALC_INT calculate_key_pair(CALC_INT * const aPubE, CALC_INT * const aPrivE = nullptr);
bool is_prime(const CALC_INT aNum);
void generate_primes(CALC_INT * const aP, CALC_INT * const aQ);
void generate_random_primes(CALC_INT * const aP, CALC_INT * const aQ);
const CALC_INT get_secret(const int Aargc, char ** const Aargv);

#endif /* __DECRYPT_RSA__ */
