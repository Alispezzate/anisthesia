#pragma once

#include <string>
#include <vector>

#include <windows.h>

#include "media.hpp"
#include "player.hpp"

namespace anisthesia::win {

	struct Process {
		DWORD id = 0;
		std::wstring name;
	};

	struct Window {
		HWND handle = nullptr;
		std::wstring class_name;
		std::wstring text;
	};

	struct Result {
		Player player;
		Process process;
		Window window;
		std::vector<Media> media;
	};

	extern "C" ANISTHESIA_API bool GetResults(const std::vector<Player>&players, media_proc_t media_proc,
		std::vector<Result>&results);

	namespace detail {

		extern "C" ANISTHESIA_API bool ApplyStrategies(media_proc_t media_proc, std::vector<Result>&results);

	}  // namespace detail

}  // namespace anisthesia::win
