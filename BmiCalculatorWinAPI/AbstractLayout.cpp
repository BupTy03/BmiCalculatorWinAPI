#include "AbstractLayout.h"
#include "Widget.h"
#include "Point.h"
#include "Size.h"
#include "Rect.h"

#include <cassert>
#include <algorithm>


AbstractLayout::AbstractLayout(Widget* pParent)
	: pParent_{pParent}
{
	assert(pParent_ != nullptr);
}

AbstractLayout::~AbstractLayout()
{
}

bool AbstractLayout::isLayout() const
{
	return true;
}

bool AbstractLayout::isWidget() const
{
	return false;
}

Margins AbstractLayout::margins() const
{
	return layoutMargins_;
}

void AbstractLayout::setMargins(const Margins& margins)
{
	layoutMargins_ = margins;
}

int AbstractLayout::spacing() const
{
	return layoutSpacing_;
}

void AbstractLayout::setSpacing(int spacing)
{
	layoutSpacing_ = spacing;
}

Rect AbstractLayout::rect() const
{
	return rect_;
}

void AbstractLayout::setRect(const Rect& rect)
{
	rect_ = rect;
}

std::optional<Align> AbstractLayout::getAlign(AbstractLayoutItem* pItem)
{
	auto it = std::lower_bound(std::begin(alignments_), std::end(alignments_), pItem, LayoutsImpl::CompareByItem());
	if (it == std::end(alignments_) || it->item != pItem)
		return {};

	return it->alignment;
}

bool AbstractLayout::setAlign(AbstractLayoutItem* pItem, Align align)
{
	if (!containsItem(pItem))
		return false;

	auto it = std::lower_bound(std::begin(alignments_), std::end(alignments_), pItem, LayoutsImpl::CompareByItem());
	if (it != std::end(alignments_) && it->item == pItem)
	{
		it->alignment = align;
	}
	else
	{
		alignments_.emplace(it, pItem, align);
	}
	return true;
}

Widget* AbstractLayout::parent() const
{
	return pParent_;
}

const AbstractLayout::Alignments& AbstractLayout::getAlignments() const
{
	return alignments_;
}

