#include "Button.h"

#include "logging.h"
#include "Point.h"
#include "Rect.h"

#include <commctrl.h>
	

Button::Button(Widget* pParent, const std::wstring text)
	: Widget{ pParent, L"BUTTON", text, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, Rect{Point{0, 0}, 100, 23} }
{
	SIZE szStruct;
	Button_GetIdealSize(getWindowHandle(), &szStruct);
	setNormalSize(Size(szStruct.cx + 60, szStruct.cy));

	setVerticalSizePolicy(SizePolicy::Minimum);
	setHorizontalSizePolicy(SizePolicy::Normal);
	setMinSize(Size(szStruct.cx + 20, szStruct.cy));
}

Button::~Button()
{
	LogDebug("~Button()");
}

void Button::setDefault(bool flag)
{
	SendMessage(getWindowHandle(), BM_SETSTYLE, BS_DEFPUSHBUTTON, flag);
}

LRESULT Button::widgetProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_COMMAND:
	{
		if (wParam == getID())
		{
			clicked.emit();
		}

		return EXIT_SUCCESS;
	}
	}

	return Widget::widgetProcedure(hWnd, uMsg, wParam, lParam);
}

