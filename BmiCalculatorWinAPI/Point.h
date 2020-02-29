#pragma once


class Point
{
public:
	Point() = default;
	explicit Point(int x, int y) : x_{x}, y_{y} {}

	int x() const { return x_; }
	int y() const { return y_; }

	void setX(int x) { x_ = x; }
	void setY(int y) { y_ = y; }

private:
	int x_{0};
	int y_{0};
};