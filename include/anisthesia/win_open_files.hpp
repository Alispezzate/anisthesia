#pragma once

#ifdef ANISTHESIA_EXPORTS
#define ANISTHESIA_API __declspec(dllexport)
#else
#define ANISTHESIA_API __declspec(dllimport)
#endif

#include <functional>
#include <set>
#include <string>

#include <windows.h>

namespace anisthesia::win::detail {

	struct OpenFile {
		DWORD process_id;
		std::wstring path;
	};

	using open_file_proc_t = std::function<bool(const OpenFile&)>;

	extern "C" ANISTHESIA_API bool EnumerateOpenFiles(const std::set<DWORD>&process_ids,
		open_file_proc_t open_file_proc);

}  // namespace anisthesia::win::detail
