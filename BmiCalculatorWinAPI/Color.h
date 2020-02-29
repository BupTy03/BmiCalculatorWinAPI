#pragma once

#include <Windows.h>

#include <cstdint>


class Color;
COLORREF ToColorRef(Color color);


class Color
{
public:
	Color() = default;
	explicit Color(std::uint32_t rgba);
	explicit Color(std::uint8_t red, std::uint8_t green, std::uint8_t blue, std::uint8_t alpha = 0);

	std::uint32_t rgba() const;
	void setRgba(std::uint32_t value);

	std::uint32_t rgb() const;
	void setRgb(std::uint32_t value);

	std::uint8_t red() const;
	void setRed(std::uint8_t red);

	std::uint8_t green() const;
	void setGreen(std::uint8_t green);

	std::uint8_t blue() const;
	void setBlue(std::uint8_t blue);

	std::uint8_t alpha() const;
	void setAlpha(std::uint8_t alpha);

private:
	std::uint32_t rgbaColor_ = 0;
};

