#pragma once

#include "Window.h"
#include "Bitmap.h"
#include "Color.h"

#include <string>


class Label;

class ShowBmiWindow : public Window
{
public:
	explicit ShowBmiWindow(Window* parent);
	~ShowBmiWindow() override;

	void setLabelText(std::wstring text);
	void setLabelTextColor(Color color);
	void setLabelBitmap(Bitmap bitmap);

private:
	Label* pBitmapLabel_{ nullptr };
	Label* pTextLabel_{ nullptr };
};

