#pragma once

#include <Windows.h>


class PaintDevice
{
public:
	virtual ~PaintDevice();

	PaintDevice(const PaintDevice&) = delete;
	PaintDevice& operator=(const PaintDevice&) = delete;

	virtual HWND getWindowHandle() const = 0;
	HDC nativePaintDeviceHandle() const;

protected:
	PaintDevice() = default;
	void initPaintDevice();

private:
	HDC hDC_ = nullptr;
};

