#include "Rect.h"

#include <iostream>
#include <algorithm>


#undef min
#undef max


// free functions
Rect RectFromRECT(const RECT& rect)
{
	return Rect{ Point{rect.left, rect.top}, Point{rect.right, rect.bottom} };
}

RECT RECTFromRect(const Rect& rect)
{
	const auto topLeft = rect.leftTop();
	const auto bottomRight = rect.rightBottom();
	return RECT{ topLeft.x(), topLeft.y(), bottomRight.x(), bottomRight.y() };
}

void PrintRect(const Rect& rect)
{
	const auto leftTop = rect.leftTop();
	std::cout << "Point{" << leftTop.x() << ", " << leftTop.y() << "}, " <<
		"{width: " << rect.width() << ", height: " << rect.height() << "}\n";
}

Rect MoveRect(const Rect& rect, const Point& pos)
{
	return Rect(pos, rect.size());
}

static Size ScaleSizeProportional(Size destSize, Size sourceSize)
{
	if (sourceSize.height() == 0 || sourceSize.width() == 0)
		return sourceSize;

	const double propSource = sourceSize.width() / static_cast<double>(sourceSize.height());
	if (propSource > 1.0)
	{
		const int width = (destSize.width() >= destSize.height()) 
			? destSize.width() 
			: std::min(static_cast<int>(destSize.height() * propSource), destSize.width());

		return Size(width, width * (propSource - 1.0));
	}

	if (propSource < 1.0)
	{
		const int height = (destSize.height() >= destSize.width())
			? destSize.height()
			: std::min(destSize.height(), static_cast<int>(destSize.width() / propSource));

		return Size(height * propSource, height);
	}

	return Size(std::min(destSize.width(), destSize.height()), std::min(destSize.width(), destSize.height()));
}

Rect ScaleRect(const Rect& dest, const Rect& source)
{
	const Size sz = ScaleSizeProportional(dest.size(), source.size());

	Point pos = dest.leftTop();
	if(dest.width() > sz.width())
		pos.setX(pos.x() + (dest.width() - sz.width()) / 2);

	if (dest.height() > sz.height())
		pos.setY(pos.y() + (dest.height() - sz.height()) / 2);

	return Rect(pos, sz);
}


// class definition
Rect::Rect(const Point& leftTop, const Point& rightBottom)
	: leftTop_{leftTop}
	, rightBottom_{rightBottom}
{
}

Rect::Rect(const Point& leftTop, const Size& size)
	: leftTop_{ leftTop }
	, rightBottom_{ leftTop.x() + size.width(), leftTop.y() + size.height() }
{
}

Rect::Rect(const Point& leftTop, int width, int height)
	: leftTop_{ leftTop }
	, rightBottom_{leftTop.x() + width, leftTop.y() + height}
{
}

Rect::Rect(int x, int y, int width, int height)
	: leftTop_{x, y}
	, rightBottom_{x + width, y + height}
{
}

Point Rect::leftTop() const
{
	return leftTop_;
}

void Rect::setLeftTop(const Point& leftTop)
{
	leftTop_ = leftTop;
}

Point Rect::leftBottom() const
{
	return Point{ leftTop_.x(), rightBottom_.y() };
}

void Rect::setLeftBottom(const Point& leftBottom)
{
	leftTop_.setX(leftBottom.x());
	rightBottom_.setY(leftBottom.y());
}

Point Rect::rightTop() const
{
	return Point{ rightBottom_.x(), leftTop_.y() };
}

void Rect::setRightTop(const Point& rightTop)
{
	rightBottom_.setX(rightTop.x());
	leftTop_.setY(rightTop.y());
}

Point Rect::rightBottom() const
{
	return rightBottom_;
}

void Rect::setRightBottom(const Point& rightBottom)
{
	rightBottom_ = rightBottom;
}

Size Rect::size() const
{
	return Size{ rightBottom_.x() - leftTop_.x(), rightBottom_.y() - leftTop_.y() };
}

void Rect::setSize(const Size& sz)
{
	rightBottom_.setX(leftTop_.x() + sz.width());
	rightBottom_.setY(leftTop_.y() + sz.height());
}

int Rect::width() const
{
	return rightBottom_.x() - leftTop_.x();
}

void Rect::setWidth(int width)
{
	rightBottom_.setX(leftTop_.x() + width);
}

int Rect::height() const
{
	return rightBottom_.y() - leftTop_.y();
}

void Rect::setHeight(int height)
{
	rightBottom_.setY(leftTop_.y() + height);
}
