#pragma once

#include <string>


class Font;
class Rect;
class Point;
class Color;
class Bitmap;
class PaintDevice;


class Painter
{
public:
	explicit Painter(PaintDevice* pPaintDevice);

	void drawBitmap(const Rect& dest, const Bitmap& bitmap, const Rect& source);
	void drawText(const std::wstring& text, const Font& font, const Rect& rect, const Color& color, int align);

private:
	PaintDevice* pPaintDevice_ = nullptr;
};

