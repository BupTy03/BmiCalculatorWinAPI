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

LRESULT Window::widgetProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CLOSE)
	{
		if (parent() != nullptr)
		{
			Window& parentWindow = dynamic_cast<Window&>(*parent());
			if (!parentWindow.activated())
			{
				parentWindow.activate();
			}
		}

		return Widget::widgetProcedure(hWnd, uMsg, wParam, lParam);
	}

	return Widget::widgetProcedure(hWnd, uMsg, wParam, lParam);
}

bool Window::activated() const
{
	return IsWindowEnabled(getWindowHandle());;
}

void Window::activate()
{
	EnableWindow(getWindowHandle(), TRUE);
}

void Window::deactivate()
{
	EnableWindow(getWindowHandle(), FALSE);
}

Window::~Window()
{
	if (parent() == nullptr)
		return;

	dynamic_cast<Window&>(*parent()).activate();
}

bool Window::isWindow() const
{
	return true;
}

void Window::close()
{
	SendMessage(getWindowHandle(), WM_CLOSE, NULL, NULL);
}

bool Window::modal() const
{
	auto pParent = parent();
	if (pParent == nullptr)
		return false;

	return !dynamic_cast<Window&>(*pParent).activated();
}

void Window::setModal(bool modal)
{
	auto pParent = parent();
	if (pParent == nullptr)
		return;

	Window& parentWindow = dynamic_cast<Window&>(*pParent);
	if (modal)
	{
		parentWindow.deactivate();
	}
	else
	{
		parentWindow.activate();
	}
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
