#include "RadioButton.h"

#include "logging.h"
#include "Rect.h"
#include "Point.h"


RadioButton::RadioButton(Widget* pParent, const std::wstring& text, bool beginGroup)
	: Widget{ pParent, L"BUTTON", text,
		WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | (beginGroup ? WS_GROUP : 0),
		Rect{Point{0, 0}, 200, 23} }
{
	if (beginGroup && pParent != nullptr)
	{
		CheckDlgButton(pParent->getWindowHandle(), getID(), true);
		checked_ = true;
	}

	setNormalSize(Size{ 120, 23 });
	setMinSize(Size{ 30, 23 });
}

RadioButton::~RadioButton()
{
	LogDebug("~RadioButton()");
}

bool RadioButton::isChecked() const
{
	return IsDlgButtonChecked(parent()->getWindowHandle(), getID());
}

void RadioButton::setChecked(bool flag)
{
	if (isChecked() == flag) return;
	CheckDlgButton(parent()->getWindowHandle(), getID(), flag);
}

LRESULT RadioButton::widgetProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	const auto selfID = getID();
	switch (uMsg)
	{
	case WM_COMMAND:
	{
		if (wParam == selfID)
		{
			selected.emit();
		}
		return EXIT_SUCCESS;
	}
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
