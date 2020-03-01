#pragma once

#include "Widget.h"
#include "Bitmap.h"
#include "Painter.h"
#include "Align.h"


class Label : public Widget
{
public:
	explicit Label(Widget* pParent, const std::wstring text = {});
	~Label() override;

	void setBitmap(const Bitmap& bitmap);
	
	std::wstring text() const;
	void setText(std::wstring text);

	int textAlign() const;
	void setTextAlign(int align);

protected:
	LRESULT CALLBACK widgetProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

private:
	std::wstring text_;
	Bitmap bitmap_;
	Painter painter_;
	int textAlign_ = Align::ALIGN_CENTER;
};