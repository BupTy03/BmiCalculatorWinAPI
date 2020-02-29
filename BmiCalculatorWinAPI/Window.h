#pragma once

#include "Widget.h"

#include <string>


class Window : public Widget
{
public:
	void close();

protected:
	explicit Window(Window* parent, const std::wstring& className, const std::wstring& title);

private:
	static void registerClass(const std::wstring& className);
};


