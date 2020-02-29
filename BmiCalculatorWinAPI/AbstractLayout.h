#pragma once

#include "LayoutsImpl.h"
#include "AbstractLayoutItem.h"
#include "Margins.h"
#include "SizePolicy.h"
#include "Align.h"
#include "Rect.h"

#include <vector>
#include <optional>


class Widget;

class AbstractLayout : public AbstractLayoutItem
{
	using Alignments = LayoutsImpl::Alignments;
public:
	static constexpr auto DEFAULT_LAYOUT_SPACING = 6;

	explicit AbstractLayout(Widget* pParent);
	~AbstractLayout() override;

	bool isLayout() const override;
	bool isWidget() const override;

	Margins margins() const;
	void setMargins(const Margins& margins);

	int spacing() const;
	void setSpacing(int spacing);

	Rect rect() const override;
	void setRect(const Rect& rect) override;

	std::optional<Align> getAlign(AbstractLayoutItem* pItem);
	bool setAlign(AbstractLayoutItem* pItem, Align align);

	virtual void addWidget(Widget* pWidget) = 0;
	virtual void addLayout(AbstractLayout* pLayout) = 0;
	virtual bool containsItem(AbstractLayoutItem* pItem) const = 0;

	Widget* parent() const;
	const Alignments& getAlignments() const;

private:
	Widget* pParent_{ nullptr };
	int layoutSpacing_{ DEFAULT_LAYOUT_SPACING };
	Margins layoutMargins_{1};
	Alignments alignments_;
	Rect rect_;
};

