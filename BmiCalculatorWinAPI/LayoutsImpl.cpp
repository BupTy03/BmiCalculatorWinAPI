#include "LayoutsImpl.h"

#include "Widget.h"
#include "Margins.h"
#include "Point.h"
#include "Rect.h"

#include <numeric>
#include <cassert>
#include <algorithm>


#undef min;
#undef max;

namespace LayoutsImpl
{

	Rect WithoutMargins(const Rect& rect, const Margins& margins)
	{
		Point pos = rect.leftTop();

		return Rect{
			pos.x() + margins.left,
			pos.y() + margins.top,
			rect.width() - (margins.left + margins.right),
			rect.height() - (margins.top + margins.bottom)
		};
	}

	Size CalculateVerticalSummarySize(
		const ItemsList& items,
		const Margins& margins, 
		int spacing,
		std::function<Size(const AbstractLayoutItem&)> func)
	{
		if (std::empty(items))
			return Size();

		const auto summarySize = std::accumulate(std::cbegin(items), std::cend(items), Size{},
			[func](const Size& summary, auto pItem) {
				const auto itemSz = func(*pItem);
				return Size(std::max(summary.width(), itemSz.width()), summary.height() + itemSz.height());
			});

		return Size(summarySize.width() + margins.left + margins.right,
			summarySize.height() + margins.top + margins.bottom + (static_cast<int>(std::size(items)) - 1) * spacing);
	}

	Size CalculateHorizontalSummarySize(
		const ItemsList& items, 
		const Margins& margins, 
		int spacing, 
		std::function<Size(const AbstractLayoutItem&)> func)
	{
		if (std::empty(items))
			return {};

		const auto summarySize = std::accumulate(std::cbegin(items), std::cend(items), Size{},
			[func](const Size& summary, auto pItem) {
				const auto itemSz = func(*pItem);
				return Size(summary.width() + itemSz.width(), std::max(summary.height(), itemSz.height()));
			});

		return Size(summarySize.width() + margins.left + margins.right + (static_cast<int>(std::size(items)) - 1) * spacing,
			summarySize.height() + margins.top + margins.bottom);
	}

	Size CalculateFormSummarySize(
		const FormLayoutItems& items, 
		const Margins& margins, 
		int spacing, 
		std::function<Size(const AbstractLayoutItem&)> func)
	{
		if (std::empty(items))
			return {};

		const auto summarySize = std::accumulate(std::cbegin(items), std::cend(items), Size{},
			[func](const Size& summary, auto itemsPair) {
				const auto labelSize = func(*(itemsPair.first));
				const auto itemSize = func(*(itemsPair.second));
				return Size(std::max(summary.width(), labelSize.width() + itemSize.width()), 
					summary.height() + std::max(labelSize.height(), itemSize.height()));
			});

		return Size(summarySize.width() + margins.left + margins.right + (static_cast<int>(std::size(items)) - 1) * spacing,
			summarySize.height() + margins.top + margins.bottom);
	}


	Size ApplyItemsSizePolicy(const AbstractLayoutItem& item, const Size& sz, bool hasMaximized, bool vertical)
	{
		if (!item.isWidget())
			return sz;

		const auto& widget = dynamic_cast<const Widget&>(item);

		int width = sz.width();
		switch (widget.horizontalSizePolicy())
		{
		case SizePolicy::Fill:
			width = std::min(sz.width(), widget.maxSize().width());
			break;
		case SizePolicy::Minimum:
			width = widget.minSize().width();
			break;
		case SizePolicy::Normal:
			width = (!vertical) &&hasMaximized ? widget.minSize().width() : std::min(width, widget.normalSize().width());
			break;
		default:
			assert(false);
		}

		int height = sz.height();
		switch (widget.verticalSizePolicy())
		{
		case SizePolicy::Fill:
			height = std::min(sz.height(), widget.maxSize().height());
			break;
		case SizePolicy::Minimum:
			height = widget.minSize().height();
			break;
		case SizePolicy::Normal:
			height = vertical && hasMaximized ? widget.minSize().height() : std::min(height, widget.normalSize().height());
			break;
		default:
			assert(false);
		}

		return Size{ width, height };
	}
	
	Rect ApplyItemsVerticalAlignment(Rect itemRect, const Rect& maxRect, int align)
	{
		if (align & ALIGN_VCENTER)
		{
			itemRect = MoveRect(itemRect, Point{ itemRect.leftTop().x(), 
				maxRect.leftTop().y() + (maxRect.height() - itemRect.height()) / 2 });
		}
		else if (align & ALIGN_TOP)
		{
			itemRect = MoveRect(itemRect, Point{ itemRect.leftTop().x(), 
				maxRect.leftTop().y() });
		}
		else if (align & ALIGN_BOTTOM)
		{
			itemRect = MoveRect(itemRect, Point{ itemRect.leftTop().x(), 
				maxRect.leftBottom().y() - itemRect.height() });
		}
		
		return itemRect;
	}

	Rect ApplyItemsHorizontalAlignment(Rect itemRect, const Rect& maxRect, int align)
	{
		if (align & ALIGN_HCENTER)
		{
			itemRect = MoveRect(itemRect, Point{ 
				maxRect.leftBottom().x() + (maxRect.width() - itemRect.width()) / 2, 
				itemRect.leftTop().y() });
		}
		else if (align & ALIGN_LEFT)
		{
			itemRect = MoveRect(itemRect, Point{ maxRect.leftBottom().x(), itemRect.leftTop().y() });
		}
		else if (align & ALIGN_RIGHT)
		{
			itemRect = MoveRect(itemRect, Point{ maxRect.rightTop().x() - itemRect.height(), 
				itemRect.leftTop().y() });
		}

		return itemRect;
	}

	void ApplyRectForVerticalLayoutItems(const ItemsList& items, Point startPos, Size maxSz, 
		int spacing, const Alignments& alignments, bool hasMaximized)
	{
		for (auto pItem : items)
		{
			const auto sz = ApplyItemsSizePolicy(*pItem, maxSz, hasMaximized, true);
			Rect r(startPos, sz);
			pItem->setRect(r);

			// apply alignments
			int alignment = Align::ALIGN_VCENTER | Align::ALIGN_LEFT;
			auto it = std::lower_bound(std::cbegin(alignments), std::cend(alignments), pItem, CompareByItem());
			if (it != std::cend(alignments) && it->item == pItem)
				alignment = it->alignment;

			if (maxSz.width() > sz.width())
				r = ApplyItemsHorizontalAlignment(r, Rect(r.leftTop(), maxSz), alignment);

			if (!hasMaximized && maxSz.height() > sz.height())
				r = ApplyItemsVerticalAlignment(r, Rect(r.leftTop(), maxSz), alignment);

			// apply rect
			pItem->setRect(r);

			// make step
			if (hasMaximized)
				startPos.setY(startPos.y() + r.height() + spacing);
			else
				startPos.setY(startPos.y() + maxSz.height());
		}
	}

	void ApplyRectForVerticalLayout(const Rect& rect, const ItemsList& items, 
		int spacing, const Alignments& alignments)
	{
		int minSizedHeightSum = 0;
		int normalSizedHeightSum = 0;
		int normalMinSizedHeightSum = 0;

		int normalSizedCount = 0;
		int maxSizedCount = 0;
		int minSizeCount = 0;
		for (auto pItem : items)
		{
			if (!pItem->isWidget())
			{
				++maxSizedCount;
				continue;
			}

			auto pWidget = dynamic_cast<Widget*>(pItem);
			assert(pWidget != nullptr);
			switch (pWidget->verticalSizePolicy())
			{
			case SizePolicy::Normal:
				normalSizedHeightSum += pWidget->normalSize().height();
				normalMinSizedHeightSum += pWidget->minSize().height();
				++normalSizedCount;
				break;
			case SizePolicy::Minimum:
				minSizedHeightSum += pWidget->minSize().height();
				++minSizeCount;
				break;
			case SizePolicy::Fill:
				++maxSizedCount;
				break;
			default:
				assert(false);
				break;
			}
		}

		const auto sumSpacing = spacing * (std::size(items) - 1);
		if (maxSizedCount > 0)
		{
			const auto residualHeight = rect.height() - minSizedHeightSum - normalMinSizedHeightSum - sumSpacing;
			ApplyRectForVerticalLayoutItems(items, rect.leftTop(), Size(rect.width(), 
				static_cast<int>(residualHeight / maxSizedCount)), spacing, alignments, true);
		}
		else if(!items.empty())
		{
			ApplyRectForVerticalLayoutItems(items, rect.leftTop(), Size(rect.width(), 
				static_cast<int>(rect.height() / std::size(items))), spacing, alignments, false);
		}
	}

	void ApplyRectForHorizontalLayoutItems(const ItemsList& items, Point startPos, Size maxSz, 
		int spacing, const Alignments& alignments, bool hasMaximized)
	{
		for (auto pItem : items)
		{
			const auto sz = ApplyItemsSizePolicy(*pItem, maxSz, hasMaximized, false);
			Rect r(startPos, sz);
			pItem->setRect(r);

			// apply alignments
			int alignment = Align::ALIGN_VCENTER | Align::ALIGN_LEFT;
			auto it = std::lower_bound(std::cbegin(alignments), std::cend(alignments), pItem, CompareByItem());
			if (it != std::cend(alignments) && it->item == pItem)
				alignment = it->alignment;

			if (!hasMaximized && maxSz.width() > sz.width())
				r = ApplyItemsHorizontalAlignment(r, Rect(r.leftTop(), maxSz), alignment);

			if (maxSz.height() > sz.height())
				r = ApplyItemsVerticalAlignment(r, Rect(r.leftTop(), maxSz), alignment);

			// apply rect
			pItem->setRect(r);

			// make step
			if (hasMaximized)
				startPos.setX(startPos.x() + r.width() + spacing);
			else
				startPos.setX(startPos.x() + maxSz.width());
		}
	}

	void ApplyRectForHorizontalLayout(const Rect& rect, const ItemsList& items, 
		int spacing, const Alignments& alignments)
	{
		int minSizedWidthSum = 0;
		int normalSizedWidthSum = 0;
		int normalMinSizedWidthSum = 0;

		int normalSizedCount = 0;
		int maxSizedCount = 0;
		int minSizeCount = 0;
		for (auto pItem : items)
		{
			if (!pItem->isWidget())
			{
				++maxSizedCount;
				continue;
			}

			auto pWidget = dynamic_cast<Widget*>(pItem);
			assert(pWidget != nullptr);
			switch (pWidget->horizontalSizePolicy())
			{
			case SizePolicy::Normal:
				normalSizedWidthSum += pWidget->normalSize().width();
				normalMinSizedWidthSum += pWidget->minSize().width();
				++normalSizedCount;
				break;
			case SizePolicy::Minimum:
				minSizedWidthSum += pWidget->minSize().width();
				++minSizeCount;
				break;
			case SizePolicy::Fill:
				++maxSizedCount;
				break;
			default:
				assert(false);
				break;
			}
		}

		const auto sumSpacing = spacing * (std::size(items) - 1);
		if (maxSizedCount > 0)
		{
			const auto residualWidth = rect.width() - minSizedWidthSum - normalMinSizedWidthSum - sumSpacing;
			ApplyRectForHorizontalLayoutItems(items, rect.leftTop(), Size{ static_cast<int>(residualWidth / maxSizedCount),
				rect.height() }, spacing, alignments, true);
		}
		else if (!items.empty())
		{
			ApplyRectForHorizontalLayoutItems(items, rect.leftTop(), Size{ static_cast<int>(rect.width() / std::size(items)),
				rect.height() }, spacing, alignments, false);
		}
	}

	void ApplyRectForFormLayoutItems(const FormLayoutItems& items, Point startPos, Size maxSz, int spacing, const Alignments& alignments)
	{
		for (auto [pLabel, pItem] : items)
		{
			int labelWidth = std::min(pLabel->minSize().width(), maxSz.width());
			int itemWidth = maxSz.width() - labelWidth - spacing * 2;

			const auto szLabel = ApplyItemsSizePolicy(*pLabel, Size(labelWidth, maxSz.height()), false, true);
			const auto szItem = ApplyItemsSizePolicy(*pItem, Size(itemWidth, maxSz.height()), false, true);

			Rect labelRect(startPos, szLabel);
			Rect itemRect(Point(startPos.x() + labelWidth + spacing, startPos.y()), szItem);

			labelRect.setLeftTop(Point(
				labelRect.leftTop().x(),
				labelRect.leftTop().y() + (maxSz.height() - labelRect.height()) / 2
			));

			labelRect.setSize(szLabel);

			itemRect.setLeftTop(Point(
				itemRect.leftTop().x() + itemWidth - szItem.width(),
				itemRect.leftTop().y() + (maxSz.height() - itemRect.height()) / 2
			));

			itemRect.setSize(szItem);

			pLabel->setRect(labelRect);
			pItem->setRect(itemRect);

			// make step
			startPos.setY(startPos.y() + maxSz.height() + spacing);
		}
	}

	void ApplyRectForFormLayout(const Rect& rect, const FormLayoutItems& items, int spacing, const Alignments& alignments)
	{
		if (std::empty(items))
			return;

		int minSizedHeightSum = 0;
		int minSizeCount = 0;
		for (auto [pLabel, pItem] : items)
		{
			++minSizeCount;
			minSizedHeightSum += std::max(pLabel->minSize().height(), pItem->minSize().height());
		}

		const auto sumSpacing = spacing * (std::size(items) - 1);
		ApplyRectForFormLayoutItems(items, rect.leftTop(),
			Size(rect.width(), static_cast<int>(rect.height() / std::size(items))),
			spacing, alignments);
	}

}