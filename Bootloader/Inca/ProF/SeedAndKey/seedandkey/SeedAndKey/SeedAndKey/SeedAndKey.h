#ifndef __SEEDANDKEY_H__
#define __SEEDANDKEY_H__

#include <windows.h>

/*  To use this exported function of dll, include this header
*  in your project.
*/

#ifdef BUILD_DLL
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C"
{
#endif

	DWORD DLL_EXPORT seed2key(char *param, BYTE *key);

#ifdef __cplusplus
}
#endif

#endif // __SEEDANDKEY_H__
