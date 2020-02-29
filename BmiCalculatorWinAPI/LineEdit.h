#pragma once

#include "Widget.h"


class LineEdit : public Widget
{
public:
	explicit LineEdit(Widget* pParent = nullptr);
	~LineEdit() override;

	std::wstring text() const;
	void setText(const std::wstring& text);
};

