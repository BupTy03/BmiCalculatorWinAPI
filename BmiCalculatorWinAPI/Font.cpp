#include "Font.h"

#include <stdexcept>


class Font::FontImpl final
{
public:
	explicit FontImpl(
		const std::wstring& name,
		int height,
		int width,
		int style,
		FontWeight weight,
		int angle
	)
		: height_{ height }
		, width_{ width }
		, angle_{ angle }
		, style_{ style }
		, weight_{ weight }
		, name_{ name }
		, fontHandle_{
		CreateFont(
			height,
			width,
			angle,
			angle,
			static_cast<int>(weight),
			style & FontStyle::Italic,
			style & FontStyle::Underline,
			style & FontStyle::StrikeOut,
			DEFAULT_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			0,
			name_.c_str()) }
	{
		if (fontHandle_ == nullptr)
			throw std::runtime_error{ "Unable to create font!" };
	};

	~FontImpl() 
	{ 
		if(fontHandle_ != nullptr) 
			DeleteObject(fontHandle_); 
	}

	auto clone() const
	{
		return std::make_unique<const FontImpl>(name_, height_, width_, style_, weight_, angle_);
	}

	static auto create(
		const std::wstring& name,
		int height,
		int width,
		int style,
		FontWeight weight,
		int angle
	)
	{
		return std::make_unique<const FontImpl>(name, height, width, style, weight, angle);
	}

	FontImpl(const FontImpl&) = delete;
	FontImpl& operator=(const FontImpl&) = delete;

	FontImpl(FontImpl&&) = delete;
	FontImpl& operator=(FontImpl&&) = delete;

	HFONT nativeHandle() const { return fontHandle_; }

	int width() const { return width_; }
	int height() const { return height_; }

	int angle() const { return angle_; }

	FontWeight weight() const { return weight_; }
	int style() const { return style_; }

	const std::wstring& name() const { return name_; }

private:
	int height_{};
	int width_{};
	int angle_{};
	int style_{ FontStyle::Regular };
	FontWeight weight_{ FontWeight::DontCare };
	std::wstring name_;
	HFONT fontHandle_{};
};


Font::Font()
{
}

Font::~Font()
{
}

Font::Font(const std::wstring& name, int height, int width, int style, FontWeight weight, int angle)
	: pImpl_{FontImpl::create(name, height, width, style, weight, angle)}
{
}

Font::Font(const Font& other)
	: pImpl_{ other.pImpl_ ? other.pImpl_->clone() : nullptr }
{
}

Font& Font::operator=(const Font& other)
{
	*this = Font(other);
	return *this;
}

Font::Font(Font&&) noexcept = default;

Font& Font::operator=(Font&&) noexcept = default;

void Font::swap(Font& other) noexcept
{
	pImpl_.swap(other.pImpl_);
}

HFONT Font::nativeHandle() const
{
	return pImpl_->nativeHandle();
}

bool Font::isNull() const
{
	return !bool(pImpl_);
}

void Font::setFontStyle(int style)
{
	if (style == pImpl_->style())
		return;

	pImpl_ = FontImpl::create(pImpl_->name(), pImpl_->height(), pImpl_->width(), 
		style, pImpl_->weight(), pImpl_->angle());
}

int Font::fontStyle() const
{
	return pImpl_->style();
}

void Font::setFontWeight(FontWeight weight)
{
	if (weight == pImpl_->weight())
		return;

	pImpl_ = FontImpl::create(pImpl_->name(), pImpl_->height(), pImpl_->width(),
		pImpl_->style(), weight, pImpl_->angle());
}

Font::FontWeight Font::fontWeight() const
{
	return pImpl_->weight();
}

void Font::setHeight(int height)
{
	if (height == pImpl_->height())
		return;

	pImpl_ = FontImpl::create(pImpl_->name(), height, pImpl_->width(),
		pImpl_->style(), pImpl_->weight(), pImpl_->angle());
}

int Font::height() const
{
	return pImpl_->height();
}

void Font::setWidth(int width)
{
	if (width == pImpl_->width())
		return;

	pImpl_ = FontImpl::create(pImpl_->name(), pImpl_->height(), width,
		pImpl_->style(), pImpl_->weight(), pImpl_->angle());
}

int Font::width() const
{
	return pImpl_->width();
}

void Font::setAngle(int angle)
{
	if (angle == pImpl_->angle())
		return;

	pImpl_ = FontImpl::create(pImpl_->name(), pImpl_->height(), pImpl_->width(),
		pImpl_->style(), pImpl_->weight(), angle);
}

int Font::angle() const
{
	return pImpl_->angle();
}

void swap(Font& lhs, Font& rhs) noexcept
{
	lhs.swap(rhs);
}
