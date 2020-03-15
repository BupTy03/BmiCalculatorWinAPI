#pragma once

#include "Widget.h"

#include <string>


class Window : public Widget
{
public:
	~Window() override;

	bool isWindow() const override;

	void close();

	bool modal() const;
	void setModal(bool modal);

protected:
	explicit Window(Window* parent, const std::wstring& className, const std::wstring& title);

	virtual LRESULT CALLBACK widgetProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	static void registerClass(const std::wstring& className);

	bool activated() const;
	void activate();
	void deactivate();
};


