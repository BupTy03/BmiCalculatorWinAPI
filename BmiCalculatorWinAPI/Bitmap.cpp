#include "Bitmap.h"

#include "Size.h"
#include "Application.h"

#include <stdexcept>


class Bitmap::BitmapImpl final
{
public:
	explicit BitmapImpl(int resourceID)
		: hBitmap_ {LoadBitmap(Application::instance().getHInstance(), MAKEINTRESOURCE(resourceID))}
	{
		if (hBitmap_ == nullptr || GetObject(hBitmap_, sizeof(bitmap_), &bitmap_) == 0)
			throw std::runtime_error("Unable to load bitmap from resource");
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
		DeleteObject(hBitmap_);
	}

private:
	HBITMAP hBitmap_ = nullptr;
	BITMAP bitmap_{};
};


Bitmap::Bitmap(int resourceID)
	: pImpl_{std::make_shared<BitmapImpl>(resourceID)}
{
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

