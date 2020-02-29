#pragma once

#include "Window.h"
#include "Bitmap.h"

#include <string>


class Label;

class ShowBmiWindow : public Window
{
public:
	explicit ShowBmiWindow(Window* parent);
	~ShowBmiWindow() override;

	void setText(std::wstring text);
	void setBitmap(Bitmap bitmap);

private:
	Label* pBitmapLabel_{ nullptr };
	Label* pTextLabel_{ nullptr };
};

