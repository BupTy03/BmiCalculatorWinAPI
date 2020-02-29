#pragma once

#include "SizePolicy.h"
#include "AbstractLayoutItem.h"
#include "Align.h"
#include "Label.h"

#include <cassert>
#include <vector>
#include <utility>
#include <functional>


class Size;
class Point;
class Rect;
struct Margins;


namespace LayoutsImpl 
{

	using ItemsList = LayoutItemsList;

	struct AlignmentEntry
	{
		explicit AlignmentEntry(AbstractLayoutItem* pItem, Align alignment)
			: item{ pItem }, alignment{ alignment } { assert(pItem != nullptr); }

		AbstractLayoutItem* item = nullptr;
		Align alignment = Align::ALIGN_CENTER;
	};

	using Alignments = std::vector<AlignmentEntry>;

	struct CompareByItem
	{
		bool operator()(const AlignmentEntry& lhs, const AlignmentEntry& rhs) const
		{
			return lhs.item < rhs.item;
		}

		bool operator()(const AbstractLayoutItem* lhs, const AlignmentEntry& rhs) const
		{
			return lhs < rhs.item;
		}

		bool operator()(const AlignmentEntry& lhs, const AbstractLayoutItem* rhs) const
		{
			return lhs.item < rhs;
		}
	};

	using FormLayoutRow = std::pair<Label*, AbstractLayoutItem*>;
	using FormLayoutItems = std::vector<FormLayoutRow>;

	Rect WithoutMargins(const Rect& rect, const Margins& margins);

	Size CalculateVerticalSummarySize(
		const ItemsList& items,
		const Margins& margins, 
		int spacing,
		std::function<Size(const AbstractLayoutItem&)> func);

	Size CalculateHorizontalSummarySize(
		const ItemsList& items,
		const Margins& margins,
		int spacing,
		std::function<Size(const AbstractLayoutItem&)> func);

	Size CalculateFormSummarySize(
		const FormLayoutItems& items,
		const Margins& margins,
		int spacing,
		std::function<Size(const AbstractLayoutItem&)> func);


	Size ApplyItemsSizePolicy(const AbstractLayoutItem& item, const Size& sz, bool hasMaximized, bool vertical);

	Rect ApplyItemsVerticalAlignment(Rect itemRect, const Rect& maxRect, int align);
	Rect ApplyItemsHorizontalAlignment(Rect itemRect, const Rect& maxRect, int align);

	void ApplyRectForVerticalLayoutItems(const ItemsList& items, Point startPos, Size maxSz, 
		int spacing, const Alignments& alignments, bool hasMaximized);
	void ApplyRectForVerticalLayout(const Rect& rect, const ItemsList& items, 
		int spacing, const Alignments& alignments);

	void ApplyRectForHorizontalLayoutItems(const ItemsList& items, Point startPos, Size maxSz, 
		int spacing, const Alignments& alignments, bool hasMaximized);
	void ApplyRectForHorizontalLayout(const Rect& rect, const ItemsList& items, 
		int spacing, const Alignments& alignments);

	void ApplyRectForFormLayoutItems(const FormLayoutItems& items, Point startPos, Size maxSz,
		int spacing, const Alignments& alignments);
	void ApplyRectForFormLayout(const Rect& rect, const FormLayoutItems& items,
		int spacing, const Alignments& alignments);
}