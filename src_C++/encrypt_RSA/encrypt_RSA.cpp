#include "encrypt_RSA.h"


#define TOSTR(a) (#a)

const char *ErrorStr[] = { TOSTR(ENullptr), TOSTR(EBadName), TOSTR(EBadOpen), TOSTR(EBadScan) };


int CloseFile(FILE *aInFile, FILE *aOutFile)
	{
	int outf = 0, inf = 0;
	if (aInFile)
		inf = fclose(aInFile);
	if (aOutFile)
		outf = fclose(aOutFile);

	if (outf == EOF)
		fprintf(stderr, "\nWARNING! Attempting to close a non-existing output file.");
	if (inf == EOF)
		fprintf(stderr, "\nWARNING! Attempting to close a non-existing input file.\n\n");

	return ((outf == EOF || inf == EOF) ? EOF : 0);
	}

void CloseProgram(char **aInput, char **aOutput, FILE *aFin, FILE *aFout, const char * const aArgv0)
	{
	const char *argv0 = nullptr;
	if (aArgv0)
		argv0 = aArgv0;
	else
		argv0 = __argv[0];

	CloseFile(aFin, aFout);
	if (aInput)
		{
		delete[](*aInput);
		*aInput = nullptr;
		}
	if (aOutput)
		{
		delete[](*aOutput);
		*aOutput = nullptr;
		}

	fprintf(stdout, "\n%s: Press \"enter\" to exit.", argv0);
	fseek(stdin, 0, SEEK_SET);
	while ((getchar()) != NEW_LINE);
	}

void OpenInputFile(char *aFilename, FILE **aFile, char ** const aArgv)
	{
	char **argv = nullptr;
	if (aArgv)
		argv = aArgv;
	else
		argv = __argv;

	if (!argv[1])
		{
		fprintf(stdout, "%s: Enter input file: ", argv[0]);
		if (!fscanf(stdin, "%s", aFilename))
			{
			fprintf(stderr, "%s: ERROR! Could not scan input file name.\n\n", argv[0]);
			throw EBadName;
			}
		}
	else
		{
		size_t i = 0;
		while (argv[1][i] != 0 && i < FILE_NAME_LENGTH)
			aFilename[i] = argv[1][i++];
		}
	fprintf(stdout, "%s: Input file: \"%s\"\n\n", argv[0], aFilename);

	if (fopen_s(aFile, aFilename, "rb"))
		{
		fprintf(stderr, "%s: ERROR! Could not open input file \"%s\".\n\n", argv[0], aFilename);
		throw EBadOpen;
		}
	}

void OpenOutputFile(char *aFilename, FILE **aFile, const int aArgc, char ** const aArgv)
	{
	int argc = 0;
	if (aArgc)
		argc = aArgc;
	else
		argc = __argc;

	char **argv = nullptr;
	if (aArgv)
		argv = aArgv;
	else
		argv = __argv;

	if (argc <= 2)
		{
		fprintf(stdout, "%s: Enter output file: ", argv[0]);
		if (!fscanf(stdin, "%s", aFilename))
			{
			fprintf(stderr, "%s: ERROR! Could not scan output file name.\n\n", argv[0]);
			throw EBadName;
			}
		}
	else
		{
		size_t i = 0;
		while (argv[2][i] != 0 && i < FILE_NAME_LENGTH)
			aFilename[i] = argv[2][i++];
		}
	fprintf(stdout, "%s: Output file: \"%s\"\n\n", argv[0], aFilename);

	if (fopen_s(aFile, aFilename, "wb"))
		{
		fprintf(stderr, "%s: ERROR! Could not open output file \"%s\".\n\n", argv[0], aFilename);
		throw EBadOpen;
		}
	}

const CryptoRSA::CALC_INT ShareSecret(const CryptoRSA::CALC_INT aVal)
	{
	CryptoRSA::CALC_INT res = aVal * START_POINT * (CryptoRSA::CALC_INT)2;
	return res;
	}

const CryptoRSA::CALC_INT EncryptFile(FILE * const aInfile, FILE * const aOutfile)
	{
	CryptoRSA::CALC_CHAR current_char = 0;
	CryptoRSA::CEncrypt RSA_keys(LOW_LIMIT,LIMIT,START_POINT);
#ifdef DEBUG_MODE
	size_t count = 0;
	CryptoRSA::CALC_CHAR curr_char_str[2] = { 0, }, curr_echar_str[2] = { 0, };
#endif /* DEBUG_MODE */
	while (!feof(aInfile))
		{
		current_char = getc(aInfile);
		if (ferror(aInfile))
			{
			CloseFile(aInfile, aOutfile);
			throw EBadScan;
			}

		if (current_char != LIMIT)
			{
			putc(RSA_keys.EncryptCharacter(current_char), aOutfile);
#ifdef DEBUG_MODE
			curr_char_str[0] = current_char;
			curr_echar_str[0] = RSA_keys.EncryptCharacter(current_char);
			fprintf(stdout, "[%d(%s) > %d(%s)]%-12s", current_char, IS_IN_LETTER_RANGE(curr_char_str), curr_echar_str[0], IS_IN_LETTER_RANGE(curr_echar_str), "");
			++count;
			if (count % DEBUG_NUMBER_OF_COLUMNS == 0)
				fprintf(stdout, "\n");
#endif /* DEBUG_MODE */
			}
		}
#ifdef DEBUG_MODE
	fprintf(stdout, "\nNumber of characters: %d", count);
	fprintf(stdout, "\n");
#endif /* DEBUG_MODE */
	return ShareSecret(RSA_keys.Maxbound);
	}