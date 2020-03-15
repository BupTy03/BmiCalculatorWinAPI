#include "Bitmap.h"


class Bitmap::BitmapData final
{
public:
	explicit BitmapData(int resourceID)
		: resourceID_{ resourceID },
		hBitmap_{ LoadBitmap(Application::instance().getHInstance(), MAKEINTRESOURCE(resourceID)) }
	{
		if (hBitmap_ == nullptr || GetObject(hBitmap_, sizeof(bitmap_), &bitmap_) == 0)
			throw std::runtime_error("Unable to load bitmap from resource");
	}

	~BitmapData()
	{
		if (hBitmap_ != nullptr)
			DeleteObject(hBitmap_);
	}

	BitmapData(const BitmapData&) = delete;
	BitmapData& operator=(const BitmapData&) = delete;

	BitmapData(BitmapData&&) = delete;
	BitmapData& operator=(BitmapData&&) = delete;

	auto clone() const { return std::make_unique<const BitmapData>(resourceID_); }

	HBITMAP nativeHandle() const { return hBitmap_; }
	const BITMAP& getBitmap() const { return bitmap_; }
	Size size() const { return Size(bitmap_.bmWidth, bitmap_.bmHeight); }

private:
	int resourceID_{};
	HBITMAP hBitmap_{};
	BITMAP bitmap_{};
};


Bitmap::Bitmap()
{
}

Bitmap::~Bitmap()
{
}


Bitmap::Bitmap(int resourceID)
	: pData_{std::make_unique<BitmapData>(resourceID)}
{
}

Bitmap::Bitmap(const Bitmap& other)
	: pData_{ other.pData_ ? other.pData_->clone() : nullptr }
{
}

Bitmap& Bitmap::operator=(const Bitmap& other)
{
	*this = Bitmap(other);
	return *this;
}

Bitmap::Bitmap(Bitmap&&) noexcept = default;

Bitmap& Bitmap::operator=(Bitmap&&) noexcept = default;

void Bitmap::loadFromResource(int resourceID)
{
	auto loaded = std::make_unique<const BitmapData>(resourceID);
	pData_.swap(loaded);
}

bool Bitmap::isNull() const
{
	return !bool(pData_);
}

void Bitmap::swap(Bitmap& other) noexcept
{
	pData_.swap(other.pData_);
}

HBITMAP Bitmap::nativeHandle() const
{
	return pData_->nativeHandle();
}

Size Bitmap::size() const
{
	return pData_->size();
}

int Bitmap::width() const
{
	return size().width();
}

int Bitmap::height() const
{
	return size().height();
}

void swap(Bitmap& lhs, Bitmap& rhs) noexcept
{
	lhs.swap(rhs);
}
