#pragma once

#include "Point.h"
#include "Size.h"

#include <Windows.h>


class Rect;

Rect RectFromRECT(const RECT& rect);
RECT RECTFromRect(const Rect& rect);
void PrintRect(const Rect& rect);
Rect MoveRect(const Rect& rect, const Point& pos);
Rect ScaleRect(const Rect& dest, const Rect& source);


class Rect 
{
public:
	Rect() = default;
	explicit Rect(const Point& leftTop, const Point& rightBottom);
	explicit Rect(const Point& leftTop, const Size& size);
	explicit Rect(const Point& leftTop, int width, int height);
	explicit Rect(int x, int y, int width, int height);

	Point leftTop() const;
	void setLeftTop(const Point& leftTop);

	Point leftBottom() const;
	void setLeftBottom(const Point& leftBottom);

	Point rightTop() const;
	void setRightTop(const Point& rightTop);

	Point rightBottom() const;
	void setRightBottom(const Point& rightBottom);

	Size size() const;
	void setSize(const Size& sz);

	int width() const;
	void setWidth(int width);

	int height() const;
	void setHeight(int height);

private:
	Point leftTop_;
	Point rightBottom_;
};
