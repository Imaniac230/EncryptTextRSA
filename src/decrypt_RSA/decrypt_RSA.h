#ifndef __DECRYPT_RSA__
#define __DECRYPT_RSA__

#define _CRT_SECURE_NO_WARNINGS
//#define _ALLOW_COMPILER_AND_STL_VERSION_MISMATCH

#ifdef __cplusplus
#include <cstdio>
#include <cstdlib>
#include <ctime>
#else
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#endif

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
const CALC_CHAR EncryptCharacter(const CALC_CHAR aChar, CALC_INT aExp, const CALC_INT aBound);
void DecryptFile(FILE * const aInfile, FILE * const aOutfile);
const CALC_INT CalculateKeyPair(CALC_INT * const aPubE, CALC_INT * const aPrivE = nullptr);
bool IsPrime(const CALC_INT aNum);
void GeneratePrimes(CALC_INT * const aP, CALC_INT * const aQ);
void GenerateRandomPrimes(CALC_INT * const aP, CALC_INT * const aQ);
const CALC_INT GetSecret(const int Aargc = 0, char ** const Aargv = nullptr);

#endif /* __DECRYPT_RSA__ */
