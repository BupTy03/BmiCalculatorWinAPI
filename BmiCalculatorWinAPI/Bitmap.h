#pragma once

#include "Size.h"
#include "Application.h"

#include <memory>
#include <stdexcept>

#include <Windows.h>


class Bitmap final
{
public:
	Bitmap();
	~Bitmap();

	explicit Bitmap(int resourceID);

	Bitmap(const Bitmap&);
	Bitmap& operator=(const Bitmap&);

	Bitmap(Bitmap&&) noexcept;
	Bitmap& operator=(Bitmap&&) noexcept;

	void swap(Bitmap& other) noexcept;

	void loadFromResource(int resourceID);
	bool isNull() const;

	HBITMAP nativeHandle() const;

	Size size() const;
	int width() const;
	int height() const;

private:
	class BitmapData;
	std::unique_ptr<const BitmapData> pData_;
};

void swap(Bitmap& lhs, Bitmap& rhs) noexcept;

