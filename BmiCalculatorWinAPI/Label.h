#pragma once

#include "Widget.h"
#include "Bitmap.h"
#include "Painter.h"


class Label : public Widget
{
public:
	explicit Label(Widget* pParent, const std::wstring text = {});
	~Label() override;

	void setBitmap(Bitmap bitmap);
	
	std::wstring text() const;
	void setText(std::wstring text);

protected:
	LRESULT CALLBACK widgetProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

private:
	std::wstring text_;
	Bitmap bitmap_;
	Painter painter_;
};