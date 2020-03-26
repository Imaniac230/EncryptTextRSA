#include "decrypt_RSA.h"


int main(int argc, char **argv)
	{
	if (argc > 3)
		{
		fprintf(stderr, "%s: Too many input arguments.\n\n", argv[0]);
		close_program(argv[0]);
		return -1;
		}

	char *input = new char[FILE_NAME_LENGTH]();
	FILE *fin = nullptr;
	char *output = new char[FILE_NAME_LENGTH]();
	FILE *fout = nullptr;

	try
		{
		open_input_file(argv, input, &fin);
		open_output_file(argc, argv, output, &fout);
		}
	catch (...)
		{
		close_program(argv[0], input, output, fin, fout);
		return -1;
		}

	try
		{
		encrypt_file(fin, fout);
		}
	catch (...)
		{
		fprintf(stderr, "%s: Error scanning file \"%s\".\n\n", argv[0], input);
		close_program(argv[0], input, output, fin, fout);
		return -2;
		}

	fprintf(stdout, "\n%s: File \"%s\" decrypted (\"%s\").\n\n", argv[0], input, output);
	close_program(argv[0], input, output, fin, fout);

	return 0;
	}