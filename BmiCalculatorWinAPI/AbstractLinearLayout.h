#pragma once

#include "AbstractLayout.h"


class Widget;
class SpacerItem;


class AbstractLinearLayout : public AbstractLayout
{
public:
	explicit AbstractLinearLayout(Widget* pParent);
	~AbstractLinearLayout() override;

	const LayoutItemsList& layoutItems() const;

	void addSpacerItem(SpacerItem* pSpacer);
	void addWidget(Widget* pWidget) override;
	void addLayout(AbstractLayout* pLayout) override;
	bool containsItem(AbstractLayoutItem* pItem) const override;

private:
	LayoutItemsList layoutItems_;
};

