#pragma once


class Size
{
public:
	Size() = default;
	explicit Size(int width, int height) : width_{width}, height_{height} {}

	int width() const { return width_; }
	void setWidth(int width) { width_ = width; }

	int height() const { return height_; }
	void setHeight(int height) { height_ = height; }

	friend const Size operator+(const Size& lhs, const Size& rhs) { return Size{ lhs.width() + rhs.width(), lhs.height() + rhs.height() }; }

private:
	int width_{ 0 };
	int height_{ 0 };
};