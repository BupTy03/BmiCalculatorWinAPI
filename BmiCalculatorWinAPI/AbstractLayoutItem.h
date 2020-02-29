#pragma once

#include <vector>


class Rect;
class Size;


class AbstractLayoutItem
{
public:
	virtual ~AbstractLayoutItem() = default;

	virtual Rect rect() const = 0;
	virtual void setRect(const Rect& rect) = 0;
	
	virtual bool isLayout() const = 0;
	virtual bool isWidget() const = 0;

	virtual Size minSize() const = 0;
	virtual Size maxSize() const = 0;
	virtual Size normalSize() const = 0;
};


using LayoutItemsList = std::vector<AbstractLayoutItem*>;


