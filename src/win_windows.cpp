#include <set>
#include <string>

#include <windows.h>

#include <anisthesia/win_platform.hpp>
#include <anisthesia/win_util.hpp>
#include <anisthesia/win_windows.hpp>

namespace anisthesia::win::detail {

	std::wstring GetWindowClassName(HWND hwnd) {
		// The maximum size for lpszClassName, according to the documentation of
		// WNDCLASSEX structure
		constexpr int kMaxSize = 256;

		WCHAR buffer[kMaxSize];
		const auto size = ::GetClassName(hwnd, buffer, kMaxSize);
		return std::wstring(buffer, size);
	}

	std::wstring GetWindowText(HWND hwnd) {
		// We could learn the actual size with GetWindowTextLength, but this arbitrary
		// value suffices for our purpose.
		constexpr int kMaxSize = 1024;

		WCHAR buffer[kMaxSize];
		const auto size = ::GetWindowText(hwnd, buffer, kMaxSize);
		return std::wstring(buffer, size);
	}

	DWORD GetWindowProcessId(HWND hwnd) {
		DWORD process_id = 0;
		::GetWindowThreadProcessId(hwnd, &process_id);
		return process_id;
	}

	std::wstring GetProcessPath(DWORD process_id) {
		// If we try to open a SYSTEM process, this function fails and the last error
		// code is ERROR_ACCESS_DENIED.
		//
		// Note that if we requested PROCESS_QUERY_INFORMATION access right instead
		// of PROCESS_QUERY_LIMITED_INFORMATION, this function would fail when used
		// to open an elevated process.
		Handle process_handle(::OpenProcess(
			PROCESS_QUERY_LIMITED_INFORMATION, FALSE, process_id));

		if (!process_handle)
			return std::wstring();

		WCHAR buffer[MAX_PATH];
		DWORD buffer_size = MAX_PATH;

		// Note that this function requires Windows Vista or above. You may use
		// GetProcessImageFileName or GetModuleFileNameEx on earlier versions.
		if (!::QueryFullProcessImageName(process_handle.get(), 0,
			buffer, &buffer_size)) {
			return std::wstring();
		}

		return std::wstring(buffer, buffer_size);
	}

	////////////////////////////////////////////////////////////////////////////////

	bool VerifyWindowStyle(HWND hwnd) {
		const auto window_style = ::GetWindowLong(hwnd, GWL_STYLE);
		const auto window_ex_style = ::GetWindowLong(hwnd, GWL_EXSTYLE);

		auto has_style = [&window_style](DWORD style) {
			return (window_style & style) != 0;
			};
		auto has_ex_style = [&window_ex_style](DWORD ex_style) {
			return (window_ex_style & ex_style) != 0;
			};

		// Toolbars, tooltips and similar topmost windows
		if (has_style(WS_POPUP) && has_ex_style(WS_EX_TOOLWINDOW))
			return false;
		if (has_ex_style(WS_EX_TOPMOST) && has_ex_style(WS_EX_TOOLWINDOW))
			return false;

		return true;
	}

	bool VerifyClassName(const std::wstring& name) {
		static const std::set<std::wstring> invalid_names = {
			// System classes
			L"#32770",         // Dialog box
			L"CabinetWClass",  // Windows Explorer
			L"ComboLBox",
			L"DDEMLEvent",
			L"DDEMLMom",
			L"DirectUIHWND",
			L"GDI+ Hook Window Class",
			L"IME",
			L"Internet Explorer_Hidden",
			L"MSCTFIME UI",
			L"tooltips_class32",
		};

		return !name.empty() && !invalid_names.count(name);
	}

	bool VerifyProcessPath(const std::wstring& path) {
		return !path.empty() && !IsSystemDirectory(path);
	}

	bool VerifyProcessFileName(const std::wstring& name) {
		static const std::set<std::wstring> invalid_names = {
			// System files
			L"explorer",    // Windows Explorer
			L"taskeng",     // Task Scheduler Engine
			L"taskhost",    // Host Process for Windows Tasks
			L"taskhostex",  // Host Process for Windows Tasks
			L"Taskmgr",     // Task Manager
		};

		return !name.empty() && !invalid_names.count(name);
	}

	////////////////////////////////////////////////////////////////////////////////

	BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM param) {
		if (!::IsWindowVisible(hwnd))
			return TRUE;

		if (!VerifyWindowStyle(hwnd))
			return TRUE;

		Window window;
		window.handle = hwnd;
		window.text = GetWindowText(hwnd);

		window.class_name = GetWindowClassName(hwnd);
		if (!VerifyClassName(window.class_name))
			return TRUE;

		Process process;
		process.id = GetWindowProcessId(hwnd);

		const auto path = GetProcessPath(process.id);
		if (!VerifyProcessPath(path))
			return TRUE;

		process.name = GetFileNameWithoutExtension(GetFileNameFromPath(path));
		if (!VerifyProcessFileName(process.name))
			return TRUE;

		auto& window_proc = *reinterpret_cast<window_proc_t*>(param);
		if (!window_proc(process, window))
			return FALSE;

		return TRUE;
	}

	bool EnumerateWindows(window_proc_t window_proc) {
		if (!window_proc)
			return false;

		const auto param = reinterpret_cast<LPARAM>(&window_proc);

		// Note that EnumWindows enumerates only top-level windows of desktop apps
		// (as opposed to UWP apps) on Windows 8 and above.
		return ::EnumWindows(EnumWindowsProc, param) != FALSE;
	}

}  // namespace anisthesia::win::detail
