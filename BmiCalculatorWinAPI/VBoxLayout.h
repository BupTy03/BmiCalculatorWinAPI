#pragma once

#include "AbstractLinearLayout.h"

#include "Rect.h"

class Widget;
class Size;

class VBoxLayout : public AbstractLinearLayout
{
public:
	explicit VBoxLayout(Widget* pParent);
	~VBoxLayout() override;

	void setRect(const Rect& rect) override;

	Size minSize() const override;
	Size maxSize() const override;
	Size normalSize() const override;
};

