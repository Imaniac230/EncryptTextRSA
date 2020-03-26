#include "decrypt_RSA.h"


int close_file(FILE *aInFile, FILE *aOutFile)
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

void close_program(const char *aArgv0, char *aInput, char *aOutput, FILE *aFin, FILE *aFout)
	{
	if (!aArgv0)
		throw Enullptr;

	close_file(aFin, aFout);
	if (aInput)
		delete[](aInput);
	if (aOutput)
		delete[](aOutput);

	fprintf(stdout, "\n%s: Press \"enter\" to exit.", aArgv0);
	while ((getchar()) != NEW_LINE);
	while ((getchar()) != NEW_LINE);
	}

void open_input_file(char ** const Aargv, char *aFilename, FILE **aFile)
	{
	if (!Aargv[1])
		{
		fprintf(stdout, "%s: Enter input file: ", Aargv[0]);
		if (!fscanf(stdin, "%s", aFilename))
			{
			fprintf(stderr, "ERROR! Could not scan input file name.\n\n");
			//delete[](aFilename);
			throw EBadName;
			}
		}
	else
		{
		for (size_t i = 0; i < FILE_NAME_LENGTH; ++i)
			aFilename[i] = Aargv[1][i];
		}
	fprintf(stdout, "%s: Input file: \"%s\"\n\n", Aargv[0], aFilename);

	if (fopen_s(aFile, aFilename, "rb"))
		{
		fprintf(stderr, "ERROR! Could not open input file \"%s\".\n\n", aFilename);
		//delete[](aFilename);
		throw EBadOpen;
		}
	}

void open_output_file(const int Aargc, char ** const Aargv, char *aFilename, FILE **aFile)
	{
	if (Aargc <= 2)
		{
		fprintf(stdout, "%s: Enter output file: ", Aargv[0]);
		if (!fscanf(stdin, "%s", aFilename))
			{
			fprintf(stderr, "ERROR! Could not scan output file name.\n\n");
			//delete[](aFilename);
			throw EBadName;
			}
		}
	else
		{
		for (size_t i = 0; i < FILE_NAME_LENGTH; ++i)
			aFilename[i] = Aargv[2][i];
		}
	fprintf(stdout, "%s: Output file: \"%s\"\n\n", Aargv[0], aFilename);

	if (fopen_s(aFile, aFilename, "wb"))
		{
		fprintf(stderr, "%s: ERROR! Could not open output file \"%s\".\n\n", Aargv[0], aFilename);
		//delete[](aFilename);
		throw EBadOpen;
		}
	}

const CALC_INT calculate_key_pair(CALC_INT *const aPubE, CALC_INT *const aPrivE)
	{
	if (!aPubE || !aPrivE)
		throw Enullptr;

	CALC_INT P_prime = 11, Q_prime = 23;

	CALC_INT totient_phi = (P_prime - 1)*(Q_prime - 1);

	CALC_INT mod_res = 0, factor_a = 0, factor_b = 0, factor_b_out = 0;
	*aPubE = 1;
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

	CALC_INT k_integer = 0;
	CALC_INT zero_condition = (1 + k_integer * totient_phi) % *aPubE;
	while (zero_condition != 0)
		{
		++k_integer;
		zero_condition = (1 + k_integer * totient_phi) % *aPubE;
		}

	CALC_INT N_max = P_prime * Q_prime;
	*aPrivE = (k_integer * totient_phi + 1) / *aPubE;

	return N_max;
	}

const CALC_CHAR encrypt_character(const CALC_CHAR aChar)
	{
	CALC_INT pub_exponent = 0, priv_exponent = 0;
	CALC_INT max_bound = calculate_key_pair(&pub_exponent, &priv_exponent);

	CALC_INT result = (CALC_INT)aChar;
	for (size_t i = 2; i <= priv_exponent; ++i)
		{
		result *= (CALC_INT)aChar;
		result %= max_bound;
		}
	result %= max_bound;

	return (CALC_CHAR)result;
	}

void encrypt_file(FILE * const aInfile, FILE * const aOutfile)
	{
	CALC_CHAR current_char = 0;
#ifdef DEBUG_MODE
	int count = 0;
	char curr_char_str[2] = { 0, }, curr_echar_str[2] = { 0, };
#endif /* DEBUG_MODE */
	while (!feof(aInfile))
		{
		current_char = getc(aInfile);
		if (ferror(aInfile))
			{
			close_file(aInfile, aOutfile);
			throw EBadScan;
			}

		if (current_char != 255)
			{
			putc(encrypt_character(current_char), aOutfile);
#ifdef DEBUG_MODE
			curr_char_str[0] = current_char;
			curr_echar_str[0] = encrypt_character(current_char);
			fprintf(stdout, "[%d(%s) > %d(%s)]%-12s", current_char, IS_IN_LETTER_RANGE(curr_char_str), encrypt_character(current_char), IS_IN_LETTER_RANGE(curr_echar_str), "");
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