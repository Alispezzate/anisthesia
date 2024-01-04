#pragma once

#ifdef ANISTHESIA_EXPORTS
#define ANISTHESIA_API __declspec(dllexport)
#else
#define ANISTHESIA_API __declspec(dllimport)
#endif

#include <string>
#include <vector>

namespace anisthesia {

	enum class Strategy {
		WindowTitle,
		OpenFiles,
		UiAutomation,
	};

	enum class PlayerType {
		Default,
		WebBrowser,
	};

	struct Player {
		PlayerType type = PlayerType::Default;
		std::string name;
		std::string window_title_format;
		std::vector<std::string> windows;
		std::vector<std::string> executables;
		std::vector<Strategy> strategies;
	};

	extern "C" ANISTHESIA_API bool ParsePlayersData(const std::string & data, std::vector<Player>&players);
	extern "C" ANISTHESIA_API bool ParsePlayersFile(const std::string & path, std::vector<Player>&players);

}  // namespace anisthesia
