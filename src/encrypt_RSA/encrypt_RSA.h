#ifndef __ENCRYPT_RSA__
#define __ENCRYPT_RSA__

#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>

/* displays all printed chars as DEC values on console */
//#define DEBUG_MODE
#define DEBUG_NUMBER_OF_COLUMNS 4

/* simplifying macros */
#define TRUE 1
#define FALSE 0
#define FILE_NAME_LENGTH 256

#define IS_IN_LETTER_RANGE(character) (((character[0] > 32) && (character[0] < 126)) ? character : "Ee")

typedef unsigned long long CALC_INT;
typedef unsigned char CALC_CHAR;

/* white characters definitions */
#define TAB 9
#define NEW_LINE '\n'
#define SPACE ' '


enum TError
	{
	Enullptr = 0,
	EBadName,
	EBadOpen,
	EBadScan
	};


int close_file(FILE *aInFile, FILE *aOutFile = nullptr);
void close_program(const char *aArgv0, char *aInput = nullptr, char *aOutput = nullptr, FILE *aFin = nullptr, FILE *aFout = nullptr);
void open_input_file(char ** const Aargv, char *aFilename, FILE **aFile);
void open_output_file(const int Aargc, char ** const Aargv, char *aFilename, FILE **aFile);
const CALC_CHAR encrypt_character(const CALC_CHAR aChar);
void encrypt_file(FILE * const aInfile, FILE * const aOutfile);
const CALC_INT calculate_key_pair(CALC_INT * const aPubE, CALC_INT *const aPrivE);

#endif /* __ENCRYPT_RSA__ */
