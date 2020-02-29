#include "HBoxLayout.h"

#include "logging.h"

#include "Rect.h"
#include "Size.h"
#include "Widget.h"

#include <algorithm>
#include <cassert>


#undef min;
#undef max;


HBoxLayout::HBoxLayout(Widget* pParent) : AbstractLinearLayout(pParent)
{
	setRect(pParent->clientRect());
}

HBoxLayout::~HBoxLayout()
{
	LogDebug("~HBoxLayout()");
}

void HBoxLayout::setRect(const Rect& rect)
{
	LayoutsImpl::ApplyRectForHorizontalLayout(
		LayoutsImpl::WithoutMargins(rect, margins()), layoutItems(), spacing(), getAlignments());

	AbstractLayout::setRect(rect);
}

Size HBoxLayout::minSize() const
{
	return LayoutsImpl::CalculateHorizontalSummarySize(layoutItems(), margins(), spacing(),
		[](const AbstractLayoutItem& item) { return item.minSize(); });
}

Size HBoxLayout::maxSize() const
{
	return LayoutsImpl::CalculateHorizontalSummarySize(layoutItems(), margins(), spacing(),
		[](const AbstractLayoutItem& item) { return item.maxSize(); });
}

Size HBoxLayout::normalSize() const
{
	return LayoutsImpl::CalculateHorizontalSummarySize(layoutItems(), margins(), spacing(),
		[](const AbstractLayoutItem& item) { return item.normalSize(); });
}
