#include "decrypt_RSA.h"


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

const CALC_INT GetSecret(const int aArgc, char ** const aArgv)
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

	CALC_INT Sec = 0;
	if (argc <= 3)
		{
		fprintf(stdout, "%s: Secret? ", argv[0]);
		if (!fscanf(stdin, "%lld", &Sec))
			{
			fprintf(stderr, "%s: ERROR! Could not read secret.\n\n", argv[0]);
			throw EBadName;
			}
		}
	else
		{
		Sec = atoll(argv[3]);
		}

	Sec /= START_POINT * (CALC_INT)2;
	return Sec;
	}

bool IsPrime(const CALC_INT aNum)
	{
	if (aNum == 0 || aNum == 1)
		return false;
	if (aNum <= 3)
		return true;

	if (aNum % 2 == 0 || aNum % 3 == 0)
		return false;
	for (CALC_INT i = 5; i*i <= aNum; i = i + 6)
		if (aNum % i == 0 || aNum % (i + 2) == 0)
			return false;

	return true;
	}

void GenerateRandomPrimes(CALC_INT * const aP, CALC_INT * const aQ)
	{
	if (!aP || !aQ)
		throw ENullptr;

	time_t t = 0;
	srand((unsigned)time(&t));

	do
		{
		*aP = rand() % LIMIT/2;
		while (!IsPrime(*aP))
			*aP = rand() % LIMIT/2;

		*aQ = rand() % LIMIT/2;
		while (!IsPrime(*aQ))
			*aQ = rand() % LIMIT/2;

		} while ((*aP * *aQ > LIMIT) || (*aP * *aQ < LOW_LIMIT));
	}

void GeneratePrimes(CALC_INT * const aP, CALC_INT * const aQ)
	{
	if (!aP || !aQ)
		throw ENullptr;

	CALC_INT Sec = GetSecret();

	bool end = false;
	for (*aP = 2; *aP < LIMIT/2; ++*aP)
		{
		while (!IsPrime(*aP))
			++*aP;
		for (*aQ = 2; *aQ < LIMIT/2; ++*aQ)
			{
			while (!IsPrime(*aQ))
				++*aQ;
			if (*aP * *aQ == Sec)
				{
				end = true;
				break;
				}
			}
		if (end)
			break;
		}

	if (!end)
		GenerateRandomPrimes(aP, aQ);
	}

const CALC_INT CalculateKeyPair(CALC_INT * const aPubE, CALC_INT * const aPrivE)
	{
	if (!aPubE)
		throw ENullptr;

	CALC_INT P_prime = 0, Q_prime = 0;
	GeneratePrimes(&P_prime, &Q_prime);

	CALC_INT totient_phi = (P_prime - 1)*(Q_prime - 1);

	CALC_INT mod_res = 0, factor_a = 0, factor_b = 0, factor_b_out = 0;
	*aPubE = START_POINT;
	while ((*aPubE < totient_phi) && (factor_b_out != 1))
		{
		mod_res = -1;
		factor_a = ++*aPubE;
		factor_b = totient_phi;
		factor_b_out = 0;
		while (mod_res != 0)
			{
			mod_res = factor_a % factor_b;
			factor_b_out = factor_b;

			factor_a = factor_b;
			factor_b = mod_res;
			}
		}

	if (aPrivE)
		{
		CALC_INT k_integer = START_POINT * START_POINT;
		CALC_INT zero_condition = (1 + k_integer * totient_phi) % *aPubE;
		while (zero_condition != 0)
			zero_condition = (1 + ++k_integer * totient_phi) % *aPubE;

		*aPrivE = (k_integer * totient_phi + 1) / *aPubE;
		}

	return P_prime * Q_prime;
	}

const CALC_CHAR EncryptCharacter(const CALC_CHAR aChar, CALC_INT aExp, const CALC_INT aBound)
	{
	if (aBound == 1)
		return (CALC_CHAR)0;

	CALC_INT result = 1, ch = (CALC_INT)aChar;
	ch %= aBound;
	while (aExp > 0)
		{
		if (aExp % 2 == 1)
			result = (result * ch) % aBound;
		aExp >>= 1;
		ch = (ch * ch) % aBound;
		}
	return (CALC_CHAR)result;
	}

void DecryptFile(FILE * const aInfile, FILE * const aOutfile)
	{
	CALC_CHAR current_char = 0;
	CALC_INT pub_exponent = 0, priv_exponent = 0, max_bound = CalculateKeyPair(&pub_exponent, &priv_exponent);
#ifdef DEBUG_MODE
	size_t count = 0;
	CALC_CHAR curr_char_str[2] = { 0, }, curr_echar_str[2] = { 0, };
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
			putc(EncryptCharacter(current_char, priv_exponent, max_bound), aOutfile);
#ifdef DEBUG_MODE
			curr_char_str[0] = current_char;
			curr_echar_str[0] = EncryptCharacter(current_char, priv_exponent, max_bound);
			fprintf(stdout, "[%d(%s) > %d(%s)]%-12s", current_char, IS_IN_LETTER_RANGE(curr_char_str), EncryptCharacter(current_char, priv_exponent, max_bound), IS_IN_LETTER_RANGE(curr_echar_str), "");
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
	}