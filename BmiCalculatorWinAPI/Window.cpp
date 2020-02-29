#include "Window.h"

#include "Application.h"
#include "Point.h"
#include "Rect.h"

#include <stdexcept>
#include <cassert>


Window::Window(Window* parent, const std::wstring& className, const std::wstring& title)
	: Widget(parent, className, title,
			WS_DLGFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_SIZEBOX | WS_MAXIMIZEBOX,
			Rect(Point(0, 0), 300, 210),
			registerClass, true)
{
	if(parent == nullptr && Application::instance().mainWindow() == nullptr)
		Application::instance().setMainWindow(this);
}

void Window::close()
{
	SendMessage(getWindowHandle(), WM_CLOSE, NULL, NULL);
}

void Window::registerClass(const std::wstring& className)
{
	WNDCLASSEX wc{};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = reinterpret_cast<HBRUSH>(CreateSolidBrush(0xF0F0F0));
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hInstance = Application::instance().getHInstance();
	wc.lpfnWndProc = proxyWidgetProcedure;
	wc.lpszClassName = className.c_str();
	wc.lpszMenuName = nullptr;
	wc.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&wc))
		throw std::runtime_error("Error, unable to register Widget class!");
}
