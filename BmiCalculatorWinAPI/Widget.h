#pragma once

#include "HasID.h"
#include "AbstractLayoutItem.h"
#include "SizePolicy.h"
#include "Size.h"
#include "Font.h"
#include "PaintDevice.h"
#include "Color.h"

#include <Windows.h>

#include <vector>
#include <string>
#include <memory>
#include <unordered_set>


class Widget;
class Rect;
class Point;
class AbstractLayout;


class Widget : public HasID<Widget>, public AbstractLayoutItem, public PaintDevice
{
	using RegisteredClassesSet = std::unordered_set<std::wstring>;
public:
	virtual ~Widget();

protected:
	explicit Widget(
		Widget* pParent,
		const std::wstring& className,
		const std::wstring& windowName,
		DWORD style,
		const Rect& rect,
		void (*pRegisterClassProc)(const std::wstring&) = nullptr,
		bool isWindow = false
	);

public:
	HWND getWindowHandle() const override;

	Rect rect() const override;
	void setRect(const Rect& newRect) override;

	Rect clientRect() const;

	bool isLayout() const override;
	bool isWidget() const override;

	Size minSize() const override;
	Size maxSize() const override;
	Size normalSize() const override;

	void setMinSize(const Size& sz);
	void setMaxSize(const Size& sz);
	void setNormalSize(const Size& sz);

	Point position() const;
	void setPosition(const Point& pos);

	Size size() const;
	void setSize(const Size& sz);

	int width() const;
	void setWidth(int width);

	int height() const;
	void setHeight(int height);

	void show();
	void hide();

	SizePolicy horizontalSizePolicy() const;
	void setHorizontalSizePolicy(SizePolicy policy);

	SizePolicy verticalSizePolicy() const;
	void setVerticalSizePolicy(SizePolicy policy);

	Font font() const;
	void setFont(Font font);

	Color textColor() const;
	void setTextColor(Color color);

	Color backgroundColor() const;
	void setBackgroundColor(Color color);

public:
	Widget(const Widget&) = delete;
	Widget& operator=(const Widget&) = delete;

	Widget* parent() const;
	void setParent(Widget* pParent);

	void addChild(Widget* pChild);
	void removeChild(Widget* pChild);

	const std::vector<Widget*>& children() const;

protected:
	static LRESULT CALLBACK proxyWidgetProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT CALLBACK widgetProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void setMainLayout(AbstractLayout* pLayout);
	AbstractLayout* mainLayout() const;

private:
	HWND windowHandle_{ nullptr };

	AbstractLayout* pMainLayout_{ nullptr };

	SizePolicy horisontalSizePolicy_ = SizePolicy::Fill;
	SizePolicy verticalSizePolicy_ = SizePolicy::Normal;

	Size minSize_;
	Size maxSize_;
	Size normalSize_;

	static RegisteredClassesSet registeredClasses_;

	Widget* pParent_{nullptr};
	std::vector<Widget*> childrenPtrs_;

	Font font_;
	Color textColor_;
	Color backgroundColor_;
};
