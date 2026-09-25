#include <Windows.h>

#include "Display.h"
#include "Logging.h"

unsigned int Display::GetWidth()
{
	RECT desktop;
	const HWND hwnd = GetDesktopWindow();

	GetWindowRect(hwnd, &desktop);

	return desktop.right;
}

unsigned int Display::GetHeight()
{
	RECT desktop;
	const HWND hwnd = GetDesktopWindow();

	GetWindowRect(hwnd, &desktop);

	return desktop.bottom;
}

unsigned int Display::GetRefreshRate()
{
	DEVMODE dm;

	dm.dmSize = sizeof(dm);

	if (EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &dm) == 0)
	{
		Logger::Error("Failed to retrieve display settings!");

		return 60;
	}

	return dm.dmDisplayFrequency;
}
