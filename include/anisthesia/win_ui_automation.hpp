#include <functional>
#include <string>

#include <windows.h>

#ifdef ANISTHESIA_EXPORTS
#define ANISTHESIA_API __declspec(dllexport)
#else
#define ANISTHESIA_API __declspec(dllimport)
#endif


namespace anisthesia::win::detail {

	enum class WebBrowserInformationType {
		Address,
		Tab,
		Title,
	};

	struct WebBrowserInformation {
		WebBrowserInformationType type = WebBrowserInformationType::Title;
		std::wstring value;
	};

	using web_browser_proc_t = std::function<void(const WebBrowserInformation&)>;

	extern "C" ANISTHESIA_API bool GetWebBrowserInformation(HWND hwnd, web_browser_proc_t web_browser_proc);

}  // namespace anisthesia::win::detail
