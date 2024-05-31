#ifndef METADATA_H
#define METADATA_H

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define PROJECT_NAME "Boilerplate Inserter"
#define PROJECT_DESCRIPTION ""
#define PROJECT_RELEASE_DATE __DATE__ " (Not Yet Released)"

#define PROJECT_LICENSE "(Not Yet Licensed)"
#define PROJECT_LICENSE_SUMMARY ""
#define PROJECT_LICENSE_TEXT ""

#define PROJECT_AUTHOR "Alexander Stanton"
#define PROJECT_CONTRIBUTORS "Alexander Stanton"

#define PROJECT_ORGANIZATION "Elathea"
#define PROJECT_ORGANIZATION_HOMEPAGE "https://elathea.github.io/"
#define PROJECT_PROJECT_HOMEPAGE "(none)"
#define PROJECT_CONTACT_INFORMATION "xavierharkonnen9@gmail.com"

#define PROJECT_MAJOR_VERSION 0
#define PROJECT_MINOR_VERSION 0
#define PROJECT_PATCH_VERSION 1

#define PROJECT_STATUS "WIP"

#define PROJECT_BUILD_DATE __DATE__ ", " __TIME__

#if defined(__GNUC__) && !defined(__clang__) && !defined(__INTEL_COMPILER)
	#define PROJECT_COMPILER "GCC " STR(__GNUC__) "." STR(__GNUC_MINOR__) "." STR(__GNUC_PATCHLEVEL__)
#elif defined(__clang__)
	#define PROJECT_COMPILER "Clang " STR(__clang_major__) "." STR(__clang_minor__) "." STR(__clang_patchlevel__)
#elif defined(_MSC_VER)
	#define PROJECT_COMPILER "MSVC " STR(_MSC_VER)
#else
	#define PROJECT_COMPILER "Unknown Compiler"
#endif

#if defined(__linux__)
	#define PROJECT_PLATFORM "Linux"
#elif defined(_WIN32)
	#define PROJECT_PLATFORM "Windows"
#elif defined(__APPLE__)
	#define PROJECT_PLATFORM "Apple"
#else
	#define PROJECT_PLATFORM "Unknown Platform"
#endif

#endif