#include "SeedAndKey.h"

#ifdef USE_GET_MODULE_FILE_NAME
char MyPath[200];
DWORD len;
#endif

#define DUMMY_ALGO

/* --------------------------------------------------------------- */
/* ------------------ Actual seed&key algorithm ------------------ */
/* --------------------------------------------------------------- */
DWORD seedToKey(BYTE *seed, DWORD SeedLen, BYTE *key, DWORD *RetLen)
{
#ifdef DUMMY_ALGO
	BYTE Index = 0;
#endif

	/* !!!!!!!!!!!!!!!!!!!!!!!!!!!! IMPORTANT NOTE !!!!!!!!!!!!!!!!!!!!!!!!!!!! */
	/* In INCA index of seed is shifted by 1, this must not be done on ECU side */
	/* Seed[0] just contains 000, as a TAG.                                     */
	/* Seed MSB in INCA --> seed[1]                                             */
	/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

#ifdef DUMMY_ALGO
	/* Dummy algo, add 1 to each byte */
	for (Index = 0; Index < SeedLen; Index++)
	{
		key[Index] = seed[Index + 1] + 1;
	}
#endif

#ifdef REAL_ALGO
	/* Put your real algo here */
#endif

	/* Copy seed len */
	*RetLen = SeedLen;

	return TRUE;
}

/* seed2key is mandatory name for this method, since is the one invoked by INCA */
DWORD DLL_EXPORT seed2key(char *param, BYTE *key)
{
	BYTE seed[50];
	DWORD i, retLen, SeedLen;
	char *scanPtr;

	i = 0;
	scanPtr = strtok(param, " \t\n");
	while (scanPtr && i < 50)
	{
		seed[i] = atoi(scanPtr);
		scanPtr = strtok(NULL, " \t\n");
		i++;
	}
	SeedLen = i - 1;
	seedToKey(seed, SeedLen, key, &retLen);

	return (retLen);
}

/* DLL main function */
extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		/* attach to process, return FALSE to fail DLL load */
#ifdef USE_GET_MODULE_FILE_NAME
		len = GetModuleFileName(hinstDLL, MyPath, sizeof(MyPath));
		if (!len)
		{
			return 0;
		}
#endif
		break;

	case DLL_PROCESS_DETACH:
		/* detach from process */
		break;

	case DLL_THREAD_ATTACH:
		/* attach to thread */
		break;

	case DLL_THREAD_DETACH:
		/* detach from thread */
		break;
	}

	return TRUE;
}
