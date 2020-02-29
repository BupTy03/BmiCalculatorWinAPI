#include "SpacerItem.h"

#include <limits>


#undef min;
#undef max;


SpacerItem::SpacerItem()
{
}

Rect SpacerItem::rect() const
{
	return Rect();
}

void SpacerItem::setRect(const Rect& rect)
{
	rect_ = rect;
}

bool SpacerItem::isLayout() const
{
	return false;
}

bool SpacerItem::isWidget() const
{
	return false;
}

Size SpacerItem::minSize() const
{
	return Size(0, 0);
}

Size SpacerItem::maxSize() const
{
	return Size(std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
}

Size SpacerItem::normalSize() const
{
	return maxSize();
}
