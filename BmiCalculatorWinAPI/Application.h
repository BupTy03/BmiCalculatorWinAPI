#pragma once

#include "Window.h"

#include <Windows.h>


class Application final
{
	friend class Window;
public:
	explicit Application(HINSTANCE hInstance);
	~Application();

	static Application& instance();

	HINSTANCE getHInstance();
	int exec();

	Window* mainWindow();
	void setMainWindow(Window* pWindow);

private:
	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

private:
	static Application* pAppInstance_;

	Window* pMainWindow_{ nullptr };
	HINSTANCE hInstance_{ nullptr };
};

