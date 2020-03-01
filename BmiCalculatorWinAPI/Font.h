#pragma once

#include <memory>
#include <string>

#include <Windows.h>


class Font
{
public:
	enum FontStyle : int
	{
		Regular = 0,
		Italic = 1,
		Underline = 2,
		StrikeOut = 4
	};

	enum class FontWeight
	{
		DontCare = FW_DONTCARE,
		Thin = FW_THIN,
		ExtraLight = FW_EXTRALIGHT,
		Light = FW_LIGHT,
		Regular = FW_REGULAR,
		Medium = FW_MEDIUM,
		SemiBold = FW_SEMIBOLD,
		Bold = FW_BOLD,
		ExtraBold = FW_EXTRABOLD,
		Heavy = FW_HEAVY
	};

	Font() = default;
	explicit Font(
		const std::wstring& name,
		int height = 15,
		int width = 0,
		int style = FontStyle::Regular,
		FontWeight weight = FontWeight::Regular,
		int angle = 0
	);

	Font(const Font& other);
	Font& operator=(const Font& other);

	Font(Font&&) noexcept = default;
	Font& operator=(Font&&) noexcept = default;

	HFONT nativeHandle() const;

	bool isNull() const;

	void setFontStyle(int style);
	int fontStyle() const;

	void setFontWeight(FontWeight weight);
	FontWeight fontWeight() const;

	void setHeight(int height);
	int height() const;

	void setWidth(int width);
	int width() const;

	void setAngle(int angle);
	int angle() const;

private:
	class FontImpl;
	std::shared_ptr<const FontImpl> pImpl_;
};

