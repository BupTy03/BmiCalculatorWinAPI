#pragma once

#include "AbstractLinearLayout.h"
#include "Rect.h"

class Widget;
class Size;

class HBoxLayout : public AbstractLinearLayout
{
public:
	explicit HBoxLayout(Widget* pParent);
	~HBoxLayout() override;

	void setRect(const Rect& rect) override;

	Size minSize() const override;
	Size maxSize() const override;
	Size normalSize() const override;
};

