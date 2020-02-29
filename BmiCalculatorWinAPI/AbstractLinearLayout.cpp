#include "AbstractLinearLayout.h"

#include "logging.h"

#include "SpacerItem.h"


AbstractLinearLayout::AbstractLinearLayout(Widget* pParent) 
	: AbstractLayout(pParent)
{
}

AbstractLinearLayout::~AbstractLinearLayout()
{
	for (auto pItem : layoutItems_)
	{
		if (pItem->isWidget())
			delete pItem;
	}

	LogDebug("~AbstractLinearLayout()");
}

const LayoutItemsList& AbstractLinearLayout::layoutItems() const
{
	return layoutItems_;
}

void AbstractLinearLayout::addSpacerItem(SpacerItem* pSpacer)
{
	layoutItems_.emplace_back(pSpacer);
}

void AbstractLinearLayout::addWidget(Widget* pWidget)
{
	assert(parent() != nullptr);
	if (containsItem(pWidget))
		return;

	if (pWidget->parent() == nullptr)
		parent()->addChild(pWidget);

	layoutItems_.emplace_back(pWidget);
}

void AbstractLinearLayout::addLayout(AbstractLayout* pLayout)
{
	if (containsItem(pLayout))
		return;

	layoutItems_.emplace_back(pLayout);
}

bool AbstractLinearLayout::containsItem(AbstractLayoutItem* pItem) const
{
	return std::find(std::cbegin(layoutItems_), std::cend(layoutItems_), pItem) != std::cend(layoutItems_);
}
