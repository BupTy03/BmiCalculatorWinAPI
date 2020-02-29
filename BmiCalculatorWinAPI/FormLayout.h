#pragma once

#include "AbstractLayout.h"
#include "LayoutsImpl.h"

#include "Rect.h"

#include <string>


class Size;
class Rect;
class Widget;
class Label;
class AbstractLayout;

class FormLayout : public AbstractLayout
{
	using FormLayoutItems = LayoutsImpl::FormLayoutItems;
public:
	explicit FormLayout(Widget* pParent);
	~FormLayout() override;

	void setRect(const Rect& rect) override;

	Size minSize() const override;
	Size maxSize() const override;
	Size normalSize() const override;

	void addWidget(Widget* pWidget) override;
	void addLayout(AbstractLayout* pLayout) override;
	bool containsItem(AbstractLayoutItem* pItem) const override;

	void addRow(const std::wstring& labelText, Widget* pWidget);
	void addRow(const std::wstring& labelText, AbstractLayout* pLayout);

private:
	FormLayoutItems itemsPairs_;
};

