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

#ifdef CDA_DEBUG
	#define CDA_ENABLE_ASSERTS
#endif

#ifdef CDA_ENABLE_ASSERTS
	#define CDA_ASSERTS(x, ...) {if(!(x)) {CDA_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
	#define CDA_CORE_ASSERTS(x, ...) {if(!(x)) {CDA_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
	#define CDA_ASSERTS(x, ...)
	#define CDA_CORE_ASSERTS(x, ...)
#endif

#define BIT(x) (1 << x)

#define CDA_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)