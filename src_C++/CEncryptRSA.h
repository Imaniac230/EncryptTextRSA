#ifndef __CENCRYPTRSA__
#define __CENCRYPTRSA__


#ifdef __cplusplus
#include <cstdlib>
#include <ctime>
#else
#include <stdlib.h>
#include <time.h>
#endif /* __cplusplus */


namespace CryptoRSA
	{

	typedef unsigned long long CALC_INT;
	typedef unsigned char CALC_CHAR;

	class CEncrypt
		{
		public:
			CALC_INT LowLimit = 0, HighLimit = 0, PubStartPoint = 0;
			CALC_INT Maxbound = 0, PubExponent = 0;

		private:
			CALC_INT Pprime = 0, Qprime = 0, PrivExponent = 0;
			static bool SeedInicialized;
			static unsigned int Seed;


			bool IsPrime(const CALC_INT aNum);
			const CALC_INT CalculateKeyPair();
			void GenerateRandomPrimes();
			void GeneratePrimes(CALC_INT aBound);

			static void InicializeSeed()
				{
				if (!SeedInicialized)
					{
					time_t t = 0;
					Seed = (unsigned int)time(&t);
					srand(Seed);
					SeedInicialized = true;
					}
				}


		public:
			CEncrypt() : LowLimit(RAND_MAX / 2), HighLimit(RAND_MAX), PubStartPoint(RAND_MAX / 2)
				{
				InicializeSeed();
				GenerateRandomPrimes();
				Maxbound = CalculateKeyPair();
				}

			CEncrypt(CALC_INT aLow, CALC_INT aHigh, CALC_INT aPubStart) : LowLimit(aLow), HighLimit(aHigh), PubStartPoint(aPubStart)
				{
				InicializeSeed();
				GenerateRandomPrimes();
				Maxbound = CalculateKeyPair();
				}

			CEncrypt(CALC_INT aLow, CALC_INT aHigh, CALC_INT aPubStart, CALC_INT aBound) : LowLimit(aLow), HighLimit(aHigh), PubStartPoint(aPubStart), Maxbound(aBound)
				{
				InicializeSeed();
				GeneratePrimes(Maxbound);
				Maxbound = CalculateKeyPair();
				}

			virtual ~CEncrypt()
				{
				Maxbound = PubExponent = Pprime = Qprime = PrivExponent = 0;
				LowLimit = HighLimit = PubStartPoint = 0;
				}


			const CALC_CHAR EncryptCharacter(const CALC_CHAR aChar);
			const CALC_CHAR DecryptCharacter(const CALC_CHAR aChar);

			void CalculateNewKeyPair()
				{
				GenerateRandomPrimes();
				Maxbound = CalculateKeyPair();
				}

			void CalculateNewKeyPair(CALC_INT aBound)
				{
				GeneratePrimes(aBound);
				Maxbound = CalculateKeyPair();
				}

			static void ChangeGlobalSeed(unsigned int aVal)
				{
				Seed = aVal;
				srand(Seed);
				}
		}; /* CEncrypt */
	}

#endif /* __CENCRYPTRSA__ */
