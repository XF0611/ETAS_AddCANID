
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <conio.h>

#define SHOW_INCA_STRING
//#define USE_ARRAY_NOTATION
#define USE_SIMPLE_NOTATION

typedef DWORD(__cdecl *MYPROC) (char *param, BYTE *key);

#define SEED_KEY_SIZE               4
#define START                       "000 \t\n "
#define SEP                         " \t\n "
#define CUSTOMER                    "GAEI"

int main()
{
	MYPROC procAddr;
	char SeedStr[50];
	BYTE key[SEED_KEY_SIZE];
	BYTE seed[SEED_KEY_SIZE] = { 0xAA, 0xBB, 0xCC, 0xDD };   /* {MSB, ... , LSB}  */
	BYTE Index = 0;
	char SeedS[4];
	char SizeS[4];

	printf("RTA-FBL S&K DLL Test\n");
	printf("--------------------\n\n");
	printf("Customer: %s\n", CUSTOMER);
	itoa(SEED_KEY_SIZE, SizeS, 10);
	printf("S&K size: %s", SizeS);
	printf("\n\n");

#ifdef USE_ARRAY_NOTATION
	printf("NOTE: index '0' points to MSB (Example of reply: 27 02 Key[0] Key[1])\n\n\n");
#endif

	/* Init seed */
	strcpy(SeedStr, START);
	for (Index = 0; Index < SEED_KEY_SIZE; Index++)
	{
		itoa(seed[Index], SeedS, 10);
		strcat(SeedStr, SeedS);
		strcat(SeedStr, SEP);
	}

#ifdef SHOW_INCA_STRING
	printf("Seed string passed to DLL: %s\n\n", SeedStr);
#endif

	/* Init key */
	for (Index = 0; Index < SEED_KEY_SIZE; Index++)
	{
		key[Index] = 0;
	}

	HINSTANCE hDll = LoadLibrary(L"SeedAndKey.dll");
	if (hDll != NULL)
	{
		procAddr = (MYPROC)GetProcAddress(hDll, "seed2key");

		if (procAddr != NULL)
		{
			printf("DLL Loaded...\n\n");
			printf("Seed&Key algorithm inputs:\n\n");
			/* Print seed */
#ifdef USE_ARRAY_NOTATION
			printf("seed:\n");
			for (Index = 0; Index < SEED_KEY_SIZE; Index++)
			{
				printf("seed[%d] = %x\n", Index, seed[Index]);
			}
#endif

#ifdef USE_SIMPLE_NOTATION
			printf("seed: ");
			for (Index = 0; Index < SEED_KEY_SIZE; Index++)
			{
				printf("%x ", seed[Index]);
			}
#endif

			(procAddr)(SeedStr, key);
			printf("\n\nSeed&Key algorithm result:\n\n");
			/* Print key */

#ifdef USE_ARRAY_NOTATION
			printf("key:\n");
			for (Index = 0; Index < SEED_KEY_SIZE; Index++)
			{
				printf("key[%d] = %x\n", Index, key[Index]);
			}
#endif

#ifdef USE_SIMPLE_NOTATION
			printf("key: ");
			for (Index = 0; Index < SEED_KEY_SIZE; Index++)
			{
				printf("%x ", key[Index]);
			}
#endif
		}
		FreeLibrary(hDll);
		getch();
	}
	else
	{
		printf("could not load the library.");
		getch();
	}

	return 0;
}
