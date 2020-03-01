#include "FormLayout.h"

#include "logging.h"

#include "Rect.h"
#include "Size.h"
#include "Widget.h"
#include "Label.h"

#include <cassert>
#include <algorithm>


FormLayout::FormLayout(Widget* pParent) : AbstractLayout(pParent)
{
	assert(pParent != nullptr);
	setRect(pParent->clientRect());
}

FormLayout::~FormLayout()
{
	for (auto& itemsPair : itemsPairs_)
	{
		if (!itemsPair.first->isWidget())
			delete itemsPair.first;

		if (!itemsPair.second->isWidget())
			delete itemsPair.second;
	}

	LogDebug("~FormLayout()");
}

void FormLayout::setRect(const Rect& rect)
{
	LayoutsImpl::ApplyRectForFormLayout(
		LayoutsImpl::WithoutMargins(rect, margins()), itemsPairs_, spacing(), getAlignments());

	AbstractLayout::setRect(rect);
}

Size FormLayout::minSize() const
{
	return LayoutsImpl::CalculateFormSummarySize(itemsPairs_, margins(), spacing(),
		[](const AbstractLayoutItem& item) { return item.minSize(); });
}

Size FormLayout::maxSize() const
{
	return LayoutsImpl::CalculateFormSummarySize(itemsPairs_, margins(), spacing(),
		[](const AbstractLayoutItem& item) { return item.maxSize(); });
}

Size FormLayout::normalSize() const
{
	return LayoutsImpl::CalculateFormSummarySize(itemsPairs_, margins(), spacing(),
		[](const AbstractLayoutItem& item) { return item.normalSize(); });
}

void FormLayout::addWidget(Widget* pWidget)
{
	addRow(L"", pWidget);
}

void FormLayout::addLayout(AbstractLayout* pLayout)
{
	addRow(L"", pLayout);
}

bool FormLayout::containsItem(AbstractLayoutItem* pItem) const
{
	return std::find_if(std::cbegin(itemsPairs_), std::cend(itemsPairs_),
		[pItem](const auto& itemsPair) { 
			return itemsPair.first == pItem || itemsPair.second == pItem; 
		}) != std::cend(itemsPairs_);
}

void FormLayout::addRow(const std::wstring& labelText, Widget* pWidget)
{
	assert(pWidget != nullptr);
	auto pLabel = new Label(parent(), labelText);
	pLabel->setHorizontalSizePolicy(SizePolicy::Minimum);
	pLabel->setTextAlign(Align::ALIGN_RIGHT | Align::ALIGN_VCENTER);
	pWidget->setParent(parent());
	itemsPairs_.emplace_back(pLabel, pWidget);
}

void FormLayout::addRow(const std::wstring& labelText, AbstractLayout* pLayout)
{
	assert(pLayout != nullptr);
	auto pLabel = new Label(parent(), labelText);
	pLabel->setHorizontalSizePolicy(SizePolicy::Minimum);
	pLabel->setTextAlign(Align::ALIGN_RIGHT | Align::ALIGN_VCENTER);
	itemsPairs_.emplace_back(pLabel, pLayout);
}
