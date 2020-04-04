#include "CEncryptRSA.h"


bool CEncryptRSA::SeedInicialized = false;
unsigned int CEncryptRSA::Seed = 0;

bool CEncryptRSA::IsPrime(const CALC_INT aNum)
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

void CEncryptRSA::GenerateRandomPrimes()
	{
	do
		{
			Pprime = rand() % (HighLimit + 1) / 2;
			while (!IsPrime(Pprime))
				Pprime = rand() % (HighLimit + 1) / 2;

			Qprime = rand() % (HighLimit + 1) / 2;
			while (!IsPrime(Qprime))
				Qprime = rand() % (HighLimit + 1) / 2;

		} while ((Pprime * Qprime > HighLimit) || (Pprime * Qprime < LowLimit));
	}

void CEncryptRSA::GeneratePrimes(CALC_INT aBound)
	{
	bool end = false;
	for (Pprime = 2; Pprime < (HighLimit + 1) / 2; ++Pprime)
		{
		while (!IsPrime(Pprime))
			++Pprime;
		for (Qprime = 2; Qprime < (HighLimit + 1) / 2; ++Qprime)
			{
			while (!IsPrime(Qprime))
				++Qprime;
			if (Pprime * Qprime == aBound)
				{
				end = true;
				break;
				}
			}
		if (end)
			break;
		}

	if (!end)
		GenerateRandomPrimes();
	}

const CALC_INT CEncryptRSA::CalculateKeyPair()
	{
	CALC_INT totient_phi = (Pprime - 1)*(Qprime - 1);

	CALC_INT mod_res = 0, factor_a = 0, factor_b = 0, factor_b_out = 0;
	PubExponent = PubStartPoint;
	while ((PubExponent < totient_phi) && (factor_b_out != 1))
		{
		mod_res = -1;
		factor_a = ++PubExponent;
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

	CALC_INT k_integer = PubStartPoint * PubStartPoint;
	CALC_INT zero_condition = (1 + k_integer * totient_phi) % PubExponent;
	while (zero_condition != 0)
		zero_condition = (1 + ++k_integer * totient_phi) % PubExponent;

	PrivExponent = (k_integer * totient_phi + 1) / PubExponent;

	return Pprime * Qprime;
	}

const CALC_CHAR CEncryptRSA::EncryptCharacter(const CALC_CHAR aChar)
	{
	if (Maxbound == 1)
		return (CALC_CHAR)0;

	CALC_INT result = 1, ch = (CALC_INT)aChar, exp = PubExponent;
	ch %= Maxbound;
	while (exp > 0)
		{
		if (exp % 2 == 1)
			result = (result * ch) % Maxbound;
		exp >>= 1;
		ch = (ch * ch) % Maxbound;
		}
	return (CALC_CHAR)result;
	}

const CALC_CHAR CEncryptRSA::DecryptCharacter(const CALC_CHAR aChar)
	{
	if (Maxbound == 1)
		return (CALC_CHAR)0;

	CALC_INT result = 1, ch = (CALC_INT)aChar, exp = PrivExponent;
	ch %= Maxbound;
	while (exp > 0)
		{
		if (exp % 2 == 1)
			result = (result * ch) % Maxbound;
		exp >>= 1;
		ch = (ch * ch) % Maxbound;
		}
	return (CALC_CHAR)result;
	}