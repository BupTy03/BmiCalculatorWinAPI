#include "Application.h"

#include "Widget.h"

#include <algorithm>
#include <stdexcept>
#include <cassert>


Application* Application::pAppInstance_ = nullptr;


Application::Application(HINSTANCE hInstance)
	: hInstance_{ hInstance }
{
	if (pAppInstance_ != nullptr)
		throw std::runtime_error{ "Application was already constructed!" };

	pAppInstance_ = this;
}

Application::~Application()
{
}

int Application::exec()
{
	MSG message{};
	UpdateWindow(pMainWindow_->getWindowHandle());
	while (GetMessage(&message, nullptr, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	return 0;
}

void Application::setMainWindow(Window* pWindow)
{
	assert(pWindow != nullptr);
	assert(pMainWindow_ == nullptr);
	pMainWindow_ = pWindow;
}

Application& Application::instance()
{
	if (pAppInstance_ == nullptr)
		throw std::runtime_error{ "Application object isn't exist" };

	return *pAppInstance_;
}

HINSTANCE Application::getHInstance()
{
	return hInstance_;
}

Window* Application::mainWindow()
{
	return pMainWindow_;
}

