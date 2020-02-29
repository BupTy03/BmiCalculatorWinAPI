#include "Painter.h"

#include "Size.h"
#include "Point.h"
#include "Rect.h"
#include "PaintDevice.h"
#include "Bitmap.h"
#include "Font.h"
#include "Color.h"

#include <cassert>
#include <algorithm>


#undef min
#undef max


Painter::Painter(PaintDevice* pPaintDevice)
	: pPaintDevice_{pPaintDevice}
{
	assert(pPaintDevice_ != nullptr);
}

void Painter::drawBitmap(const Rect& dest, const Bitmap& bitmap, const Rect& source)
{
	PAINTSTRUCT ps{};
	HDC hdc = BeginPaint(pPaintDevice_->getWindowHandle(), &ps);

	SelectObject(pPaintDevice_->nativePaintDeviceHandle(), bitmap.nativeHandle());

	const Rect newRect = ScaleRect(dest, source);
	StretchBlt(hdc, newRect.leftTop().x(), newRect.leftTop().y(), newRect.width(), newRect.height(),
		pPaintDevice_->nativePaintDeviceHandle(), source.leftTop().x(), source.leftTop().y(), source.width(), source.height(), SRCCOPY);

	EndPaint(pPaintDevice_->getWindowHandle(), &ps);
}

void Painter::drawText(const std::wstring& text, const Font& font, const Rect& rect, const Color& textColor)
{
	if (std::empty(text))
		return;

	PAINTSTRUCT ps{};
	HDC hdc = BeginPaint(pPaintDevice_->getWindowHandle(), &ps);

	Point textPos = rect.leftTop();
	if (!font.isNull())
	{
		// getting text size
		Size textSize;
		HDC paintDeviceDC = GetDC(pPaintDevice_->getWindowHandle());
		SelectObject(paintDeviceDC, font.nativeHandle());

		TEXTMETRIC tm;
		GetTextMetrics(paintDeviceDC, &tm);

		textSize.setWidth(tm.tmAveCharWidth * std::size(text));
		textSize.setHeight(tm.tmHeight + tm.tmExternalLeading);

		ReleaseDC(pPaintDevice_->getWindowHandle(), paintDeviceDC);

		// aligning text by center
		if (rect.width() > textSize.width())
			textPos.setX(textPos.x() + (rect.width() - textSize.width()) / 2);

		if(rect.height() > textSize.height())
			textPos.setY(textPos.y() + (rect.height() - textSize.height()) / 2);

		// setting text color
		SelectObject(hdc, font.nativeHandle());
		SetTextColor(hdc, ToColorRef(textColor));
	}

	SetBkMode(hdc, TRANSPARENT);
	TextOut(hdc, textPos.x(), textPos.y(), std::data(text), std::size(text));
	EndPaint(pPaintDevice_->getWindowHandle(), &ps);
}
