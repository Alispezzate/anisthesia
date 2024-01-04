#pragma once

#ifdef ANISTHESIA_EXPORTS
#define ANISTHESIA_API __declspec(dllexport)
#else
#define ANISTHESIA_API __declspec(dllimport)
#endif


#include <string>

namespace anisthesia::detail::util {

	bool ReadFile(const std::string & path, std::string & data);//TODO: check if this is needed to be exported

	extern "C" ANISTHESIA_API bool EqualStrings(const std::string & str1, const std::string & str2);
	extern "C" ANISTHESIA_API bool TrimLeft(std::string & str, const char* chars);
	extern "C" ANISTHESIA_API bool TrimRight(std::string & str, const char* chars);

}  // namespace anisthesia::detail::util
