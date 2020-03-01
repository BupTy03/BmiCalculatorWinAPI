#include "Bitmap.h"

#include "Size.h"
#include "Application.h"

#include <stdexcept>


class Bitmap::BitmapImpl final
{
public:
	explicit BitmapImpl(int resourceID)
		: resourceID_{ resourceID },
		hBitmap_ {LoadBitmap(Application::instance().getHInstance(), MAKEINTRESOURCE(resourceID))}
	{
		if (hBitmap_ == nullptr || GetObject(hBitmap_, sizeof(bitmap_), &bitmap_) == 0)
			throw std::runtime_error("Unable to load bitmap from resource");
	}

	BitmapImpl(const BitmapImpl&) = delete;
	BitmapImpl& operator=(const BitmapImpl&) = delete;

	std::shared_ptr<BitmapImpl> clone() const
	{
		return std::make_shared<BitmapImpl>(resourceID_);
	}

	HBITMAP nativeHandle() const
	{
		return hBitmap_;
	}

	const BITMAP& getBitmap() const
	{
		return bitmap_;
	}

	Size size() const
	{
		return Size(bitmap_.bmWidth, bitmap_.bmHeight);
	}

	~BitmapImpl()
	{
		if(hBitmap_ != nullptr)
			DeleteObject(hBitmap_);
	}

private:
	int resourceID_ = 0;
	HBITMAP hBitmap_ = nullptr;
	BITMAP bitmap_{};
};


Bitmap::Bitmap(int resourceID)
	: pImpl_{std::make_shared<BitmapImpl>(resourceID)}
{
}

Bitmap::Bitmap(const Bitmap& other)
	: pImpl_{ other.pImpl_ ? other.pImpl_->clone() : nullptr }
{
}

Bitmap& Bitmap::operator=(const Bitmap& other)
{
	*this = Bitmap(other);
	return *this;
}

void Bitmap::loadFromResource(int resourceID)
{
	auto loaded = std::make_shared<const BitmapImpl>(resourceID);
	pImpl_.swap(loaded);
}

bool Bitmap::isNull() const
{
	return !bool(pImpl_);
}

HBITMAP Bitmap::nativeHandle() const
{
	return pImpl_->nativeHandle();
}

Size Bitmap::size() const
{
	return pImpl_->size();
}

int Bitmap::width() const
{
	return size().width();
}

int Bitmap::height() const
{
	return size().height();
}

