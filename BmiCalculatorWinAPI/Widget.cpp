#include "Widget.h"

#include "Application.h"
#include "Rect.h"
#include "Point.h"
#include "Size.h"
#include "predicates.h"
#include "AbstractLayout.h"
#include "logging.h"

#include <cassert>
#include <algorithm>
#include <stdexcept>
#include <limits>


#undef min;
#undef max;


Widget::RegisteredClassesSet Widget::registeredClasses_;


Widget::~Widget()
{
	if (windowHandle_ != nullptr)
		CloseWindow(windowHandle_);

	if (pParent_ != nullptr)
		pParent_->removeChild(this);

	delete pMainLayout_;

	for (auto pChild : childrenPtrs_)
	{
		pChild->pParent_ = nullptr;
		delete pChild;
	}
}

Widget::Widget(
	Widget* pParent,
	const std::wstring& className,
	const std::wstring& windowName,
	DWORD style,
	const Rect& rect,
	void (*pRegisterClass)(const std::wstring&),
	bool isWindow
)
	: minSize_{0, 0}
	, maxSize_{ std::numeric_limits<int>::max(), std::numeric_limits<int>::max() }
	, normalSize_{ rect.size() }
	, pParent_{ pParent }
	, textColor_{}
	, backgroundColor_{ 0xF0F0F000 }
{
	if (pParent_ != nullptr)
		pParent_->addChild(this);

	if (pRegisterClass != nullptr && registeredClasses_.find(className) == std::cend(registeredClasses_))
	{
		pRegisterClass(className);
		registeredClasses_.emplace(className);
	}

	const auto leftTop = rect.leftTop();
	windowHandle_ = CreateWindow(
		className.c_str(),
		windowName.c_str(),
		style,
		leftTop.x(),
		leftTop.y(),
		rect.width(),
		rect.height(),
		(pParent != nullptr) ? pParent->windowHandle_ : nullptr,
		isWindow ? nullptr : reinterpret_cast<HMENU>(getID()),
		Application::instance().getHInstance(),
		this
	);

	if (windowHandle_ == nullptr)
	{
		LogDebug("Last error code: " + std::to_string(GetLastError()));
		throw std::runtime_error("Error, unable to create Widget!");
	}

	setFont(Font(L"Segoe UI"));
}

Rect Widget::rect() const
{
	RECT wRect{};
	GetWindowRect(windowHandle_, &wRect);
	return RectFromRECT(wRect);
}

void Widget::setRect(const Rect& newRect)
{
	const auto topLeft = newRect.leftTop();
	MoveWindow(windowHandle_, topLeft.x(), topLeft.y(), newRect.width(), newRect.height(), TRUE);
}

Rect Widget::clientRect() const
{
	RECT wRect{};
	GetClientRect(windowHandle_, &wRect);
	return RectFromRECT(wRect);
}

bool Widget::isLayout() const
{
	return false;
}

bool Widget::isWidget() const
{
	return true;
}

bool Widget::isWindow() const
{
	return false;
}

Size Widget::minSize() const
{
	return minSize_;
}

Size Widget::maxSize() const
{
	return maxSize_;
}

Size Widget::normalSize() const
{
	return normalSize_;
}

void Widget::setMinSize(const Size& sz)
{
	minSize_ = sz;
}

void Widget::setMaxSize(const Size& sz)
{
	maxSize_ = sz;
}

void Widget::setNormalSize(const Size& sz)
{
	normalSize_ = sz;
}

Point Widget::position() const
{
	return rect().leftTop();
}

void Widget::setPosition(const Point& pos)
{
	auto r = rect();
	r.setLeftTop(pos);
	setRect(r);
}

Size Widget::size() const
{
	return rect().size();
}

void Widget::setSize(const Size& sz)
{
	auto r = rect();
	r.setSize(sz);
	setRect(r);
}

int Widget::width() const
{
	return rect().width();
}

void Widget::setWidth(int width)
{
	auto r = rect();
	r.setWidth(width);
	setRect(r);
}

int Widget::height() const
{
	return rect().height();
}

void Widget::setHeight(int height)
{
	auto r = rect();
	r.setHeight(height);
	setRect(r);
}

HWND Widget::getWindowHandle() const
{
	return windowHandle_;
}

LRESULT Widget::proxyWidgetProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Widget* pWidget = nullptr;
	if (uMsg == WM_NCCREATE) 
	{
		pWidget = static_cast<Widget*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
		SetLastError(0);
		if (!SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWidget)))
		{
			if (GetLastError() != 0) 
				return FALSE;
		}
	}
	else 
	{
		pWidget = reinterpret_cast<Widget*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	}

	if (pWidget == nullptr)
		return DefWindowProc(hWnd, uMsg, wParam, lParam);

	pWidget->windowHandle_ = hWnd;
	return pWidget->widgetProcedure(hWnd, uMsg, wParam, lParam);
}

LRESULT Widget::widgetProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) 
	{
	case WM_DESTROY:
	{
		if(dynamic_cast<Window*>(this) == Application::instance().mainWindow())
			PostQuitMessage(EXIT_SUCCESS);
		return EXIT_SUCCESS;
	}
	case WM_COMMAND: 
	{
		const auto childWidgets = children();
		for (auto pChild : childWidgets)
			pChild->widgetProcedure(hWnd, uMsg, wParam, lParam);

		return EXIT_SUCCESS;
	}
	case WM_PAINT:
	{
		const auto childWidgets = children();
		if(std::empty(childWidgets))
			return EXIT_SUCCESS;

		for (auto pChild : childWidgets)
			pChild->widgetProcedure(hWnd, uMsg, wParam, lParam);

		break;
	}
	case WM_SIZE:
	{
		if (pMainLayout_ != nullptr)
		{
			const auto newWidth = static_cast<int>(LOWORD(lParam));
			const auto newHeight = static_cast<int>(HIWORD(lParam));
			pMainLayout_->setRect(Rect(0, 0, newWidth, newHeight));

			return EXIT_SUCCESS;
		}
		break;
	}
	case WM_GETMINMAXINFO:
	{
		const auto pMainLayout = mainLayout();
		const auto minSz = (pMainLayout != nullptr) 
			? Size{ std::max(pMainLayout->minSize().width(), minSize().width()), 
					std::max(pMainLayout->minSize().height(), minSize().height()) }
			: minSize();

		auto lpMMI = reinterpret_cast<LPMINMAXINFO>(lParam);
		lpMMI->ptMinTrackSize.x = minSz.width();
		lpMMI->ptMinTrackSize.y = minSz.height();
		return EXIT_SUCCESS;
	}

	}

	return DefWindowProc(windowHandle_, uMsg, wParam, lParam);
}

void Widget::setMainLayout(AbstractLayout* pLayout)
{
	if (pMainLayout_ != nullptr)
		delete pMainLayout_;

	pMainLayout_ = pLayout;
}

AbstractLayout* Widget::mainLayout() const
{
	return pMainLayout_;
}

void Widget::show()
{
	ShowWindow(getWindowHandle(), SW_SHOWDEFAULT);
}

void Widget::hide()
{
	ShowWindow(getWindowHandle(), SW_HIDE);
}

SizePolicy Widget::horizontalSizePolicy() const
{
	return horisontalSizePolicy_;
}

void Widget::setHorizontalSizePolicy(SizePolicy policy)
{
	horisontalSizePolicy_ = policy;
}

SizePolicy Widget::verticalSizePolicy() const
{
	return verticalSizePolicy_;
}

void Widget::setVerticalSizePolicy(SizePolicy policy)
{
	verticalSizePolicy_ = policy;
}

void Widget::setFont(const Font& font)
{
	font_ = font;
	if (font_.isNull())
		return;

	SendMessage(windowHandle_, WM_SETFONT, reinterpret_cast<WPARAM>(font_.nativeHandle()), NULL);
}

Color Widget::textColor() const
{
	return textColor_;
}

void Widget::setTextColor(Color color)
{
	textColor_ = color;
}

Color Widget::backgroundColor() const
{
	return backgroundColor_;
}

void Widget::setBackgroundColor(Color color)
{
	backgroundColor_ = color;
}

Font Widget::font() const
{
	return font_;
}

Widget* Widget::parent() const
{
	return pParent_;
}

void Widget::setParent(Widget* pParent)
{
	if (pParent_ != nullptr)
	{
		pParent_->removeChild(this);
		pParent_ = nullptr;
	}

	if (pParent == nullptr)
		return;

	pParent->addChild(this);
	pParent_ = pParent;
}

void Widget::addChild(Widget* pChild)
{
	if (pChild == nullptr)
		return;

	auto it = std::lower_bound(std::cbegin(childrenPtrs_), std::cend(childrenPtrs_), pChild);
	if (it == std::cend(childrenPtrs_) || *it != pChild)
	{
		pChild->pParent_ = this;
		childrenPtrs_.emplace(it, pChild);
	}
}

void Widget::removeChild(Widget* pChild)
{
	if (pChild == nullptr)
		return;

	auto it = std::lower_bound(std::begin(childrenPtrs_), std::end(childrenPtrs_), pChild);
	if (it != std::end(childrenPtrs_) && *it == pChild)
	{
		childrenPtrs_.erase(it);
	}
}

std::vector<Widget*> Widget::children() const
{
	return childrenPtrs_;
}