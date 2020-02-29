#include "LineEdit.h"

#include "logging.h"
#include "Point.h"
#include "Rect.h"


LineEdit::LineEdit(Widget* pParent)
	: Widget{ pParent, L"EDIT", L"",
	WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
	Rect{Point{0, 0}, 200, 23} }
{
	setNormalSize(Size{ 120, 23 });
	setMinSize(Size{ 50, 23 });
}

LineEdit::~LineEdit()
{
	LogDebug("~LineEdit()");
}

std::wstring LineEdit::text() const
{
	wchar_t buffer[256];
	return std::wstring{ std::cbegin(buffer), 
		std::cbegin(buffer) + GetWindowText(getWindowHandle(), std::data(buffer), std::size(buffer)) 
	};
}

void LineEdit::setText(const std::wstring& text)
{
	SetWindowText(getWindowHandle(), text.c_str());
}
