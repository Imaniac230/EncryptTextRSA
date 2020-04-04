#ifndef __ENCRYPT_RSA__
#define __ENCRYPT_RSA__

#define _CRT_SECURE_NO_WARNINGS
//#define _ALLOW_COMPILER_AND_STL_VERSION_MISMATCH

#ifdef __cplusplus
#include <cstdio>
#else
#include <stdio.h>
#endif
#include "CEncryptRSA.h"

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


extern const char *ErrorStr[];

enum TError
	{
	ENullptr = 0,
	EBadName,
	EBadOpen,
	EBadScan,
	ENegativePrime
	};


int CloseFile(FILE *aInFile, FILE *aOutFile = nullptr);
void CloseProgram(char **aInput = nullptr, char **aOutput = nullptr, FILE *aFin = nullptr, FILE *aFout = nullptr, const char *aArgv0 = nullptr);
void OpenInputFile(char *aFilename, FILE **aFile, char ** const aArgv = nullptr);
void OpenOutputFile(char *aFilename, FILE **aFile, const int aArgc = 0, char ** const aArgv = nullptr);
const CALC_INT EncryptFile(FILE * const aInfile, FILE * const aOutfile);
const CALC_INT ShareSecret(const CALC_INT aVal);

#endif /* __ENCRYPT_RSA__ */
