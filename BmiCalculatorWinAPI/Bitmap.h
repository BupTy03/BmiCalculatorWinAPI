#pragma once

#include <memory>

#include <Windows.h>


class Size;

class Bitmap final
{
public:
	Bitmap() = default;
	explicit Bitmap(int resourceID);

	Bitmap(const Bitmap& other);
	Bitmap& operator=(const Bitmap& other);

	Bitmap(Bitmap&&) noexcept = default;
	Bitmap& operator=(Bitmap&&) noexcept = default;

	void loadFromResource(int resourceID);
	bool isNull() const;

	HBITMAP nativeHandle() const;

	Size size() const;
	int width() const;
	int height() const;

private:
	class BitmapImpl;
	std::shared_ptr<const BitmapImpl> pImpl_;
};

