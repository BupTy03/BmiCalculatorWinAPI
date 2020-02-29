#include "VBoxLayout.h"

#include "logging.h"

#include "Rect.h"
#include "Size.h"
#include "Widget.h"
#include "SizePolicy.h"

#include <algorithm>
#include <numeric>
#include <cassert>
#include <functional>


#undef min;
#undef max;


VBoxLayout::VBoxLayout(Widget* pParent) : AbstractLinearLayout(pParent)
{
	setRect(pParent->clientRect());
}

VBoxLayout::~VBoxLayout()
{
	LogDebug("~VBoxLayout()");
}

void VBoxLayout::setRect(const Rect& rect)
{
	LayoutsImpl::ApplyRectForVerticalLayout(
		LayoutsImpl::WithoutMargins(rect, margins()), layoutItems(), spacing(), getAlignments());

	AbstractLayout::setRect(rect);
}

Size VBoxLayout::minSize() const
{
	return LayoutsImpl::CalculateVerticalSummarySize(layoutItems(), margins(), spacing(),
		[](const AbstractLayoutItem& item) { return item.minSize(); });
}

Size VBoxLayout::maxSize() const
{
	return LayoutsImpl::CalculateVerticalSummarySize(layoutItems(), margins(), spacing(),
		[](const AbstractLayoutItem& item) { return item.maxSize(); });
}

Size VBoxLayout::normalSize() const
{
	return LayoutsImpl::CalculateVerticalSummarySize(layoutItems(), margins(), spacing(),
		[](const AbstractLayoutItem& item) { return item.normalSize(); });
}