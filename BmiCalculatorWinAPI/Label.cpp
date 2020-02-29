#include "Label.h"

#include "logging.h"

#include "Application.h"
#include "Rect.h"
#include "Point.h"
#include "Align.h"


Label::Label(Widget* pParent, const std::wstring text)
	: Widget{ pParent, L"Static", text, WS_CHILD | WS_VISIBLE, Rect(Point(0, 0), 100, 50) }
	, text_{ text }
	, bitmap_{}
	, painter_ { this }
{
	initPaintDevice();
	setMinSize(Size(80, 50));
}

Label::~Label()
{
	LogDebug("~Label()");
}

void Label::setBitmap(Bitmap bitmap)
{
	bitmap_ = std::move(bitmap);
}

std::wstring Label::text() const
{
	return text_;
}

void Label::setText(std::wstring text)
{
	text_ = std::move(text);
}

LRESULT Label::widgetProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_PAINT:
	{
		if(!bitmap_.isNull())
			painter_.drawBitmap(clientRect(), bitmap_, Rect(Point(0, 0), bitmap_.size()));

		if (!std::empty(text_))
			painter_.drawText(text_, font(), clientRect(), textColor(), Align::ALIGN_LEFT | Align::ALIGN_VCENTER);

		break;
	}
	}

	return Widget::widgetProcedure(hWnd, uMsg, wParam, lParam);
}
