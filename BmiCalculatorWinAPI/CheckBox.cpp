#include "CheckBox.h"

#include "logging.h"
#include "Point.h"
#include "Rect.h"


CheckBox::CheckBox(Widget* pParent, const std::wstring& text)
	: Widget{ pParent, L"BUTTON", text, WS_VISIBLE | WS_CHILD | BS_CHECKBOX, Rect{Point{0, 0}, 100, 23} }
{
	setNormalSize(Size{ 120, 23 });
	setMinSize(Size{ 50, 23 });
}

CheckBox::~CheckBox()
{
	LogDebug("~CheckBox()");
}

bool CheckBox::isChecked() const
{
	return checked_;
}

void CheckBox::setChecked(bool flag)
{
	if (flag == checked_) return;

	checked_ = flag;

	auto pParent = parent();
	if (pParent == nullptr) return;

	CheckDlgButton(pParent->getWindowHandle(), getID(), flag);
}

LRESULT CheckBox::widgetProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	const auto selfID = getID();
	switch (uMsg)
	{
	case WM_COMMAND:
	{
		if (wParam == selfID)
		{
			checked_ = !checked_;
			CheckDlgButton(hWnd, selfID, checked_);
			checked.emit(bool(checked_));
		}

		break;
	}
	}

	return Widget::widgetProcedure(hWnd, uMsg, wParam, lParam);
}
