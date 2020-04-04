#include "decrypt_RSA.h"


int main(int argc, char **argv)
	{
	if (argc > 4)
		{
		fprintf(stderr, "%s: Too many input arguments.\n\n", argv[0]);
		CloseProgram();
		return -1;
		}

	char *input = new char[FILE_NAME_LENGTH]();
	FILE *fin = nullptr;
	char *output = new char[FILE_NAME_LENGTH]();
	FILE *fout = nullptr;

	try
		{
		OpenInputFile(input, &fin);
		OpenOutputFile(output, &fout);
		}
	catch (...)
		{
		CloseProgram(&input, &output, fin, fout);
		return -1;
		}

	try
		{
		DecryptFile(fin, fout);
		}
	catch (...)
		{
		fprintf(stderr, "%s: Error while attempting to scan file \"%s\".\n\n", argv[0], input);
		CloseProgram(&input, &output, fin, fout);
		return -2;
		}

	fprintf(stdout, "\n%s: File \"%s\" decrypted (\"%s\").\n\n", argv[0], input, output);
	CloseProgram(&input, &output, fin, fout);

	return 0;
	}