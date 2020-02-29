#include "Color.h"


Color::Color(std::uint32_t rgba)
	: rgbaColor_{ rgba }
{
}

Color::Color(std::uint8_t red, std::uint8_t green, std::uint8_t blue, std::uint8_t alpha)
	: rgbaColor_(alpha | (blue << 8) | (green << 16) | (red << 24))
{
}

std::uint32_t Color::rgba() const
{
	return rgbaColor_;
}

void Color::setRgba(std::uint32_t value)
{
	rgbaColor_ = value;
}

std::uint32_t Color::rgb() const
{
	return rgbaColor_ >> 8;
}

void Color::setRgb(std::uint32_t value)
{
	rgbaColor_ = 0;
	rgbaColor_ |= (value << 8);
}

std::uint8_t Color::red() const
{
	return (rgbaColor_ >> 24) & 0xFF;
}

void Color::setRed(std::uint8_t red)
{
	rgbaColor_ &= 0x00FFFFFF;
	rgbaColor_ |= (red << 24);
}

std::uint8_t Color::green() const
{
	return (rgbaColor_ >> 16) & 0xFF;
}

void Color::setGreen(std::uint8_t green)
{
	rgbaColor_ &= 0xFF00FFFF;
	rgbaColor_ |= (green << 16);
}

std::uint8_t Color::blue() const
{
	return (rgbaColor_ >> 8) & 0xFF;
}

void Color::setBlue(std::uint8_t blue)
{
	rgbaColor_ &= 0xFFFF00FF;
	rgbaColor_ |= (blue << 8);
}

std::uint8_t Color::alpha() const
{
	return rgbaColor_ & 0xFF;
}

void Color::setAlpha(std::uint8_t alpha)
{
	rgbaColor_ &= 0xFFFFFF00;
	rgbaColor_ |= alpha;
}

Color Color::makeRed()
{
	return Color(255, 0, 0);
}

Color Color::makeGreen()
{
	return Color(0, 255, 0);
}

Color Color::makeBlue()
{
	return Color(0, 0, 255);
}

COLORREF ToColorRef(Color color)
{
	return RGB(color.red(), color.green(), color.blue());
}
