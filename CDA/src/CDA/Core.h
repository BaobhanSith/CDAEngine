#pragma once

#ifdef CDA_PLATFORM_WINDOWS
	#ifdef CDA_BUILD_DLL
		#define CDA_API __declspec(dllexport)
	#else
		#define CDA_API __declspec(dllimport)
	#endif
#else
	#error CDA only supports windows!
#endif