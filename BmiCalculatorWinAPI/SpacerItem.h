#pragma once

#include "AbstractLayoutItem.h"
#include "Rect.h"


class SpacerItem : public AbstractLayoutItem
{
public:
	SpacerItem();
	~SpacerItem() override = default;

	Rect rect() const override;
	void setRect(const Rect& rect) override;

	bool isLayout() const override;
	bool isWidget() const override;

	Size minSize() const override;
	Size maxSize() const override;
	Size normalSize() const override;

private:
	Rect rect_;
};

