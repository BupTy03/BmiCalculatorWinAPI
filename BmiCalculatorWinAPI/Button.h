#pragma once

#include "signal.h"
#include "Widget.h"

#include <string>


class Button : public Widget
{
public:
	explicit Button(Widget* pParent, const std::wstring text);
	~Button() override;

	void setDefault(bool flag = true);

protected:
	LRESULT CALLBACK widgetProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

public: // signals:
	signal<> clicked;
};

