#include "PaintDevice.h"

#include <cassert>


PaintDevice::~PaintDevice()
{
	if(hDC_) DeleteDC(hDC_);
}

HDC PaintDevice::nativePaintDeviceHandle() const
{
	return hDC_;
}

void PaintDevice::initPaintDevice()
{
	HDC hdc = GetDC(getWindowHandle());
	hDC_ = CreateCompatibleDC(hdc);
	ReleaseDC(getWindowHandle(), hdc);
}
