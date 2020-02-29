#pragma once

#include "Widget.h"
#include "signal.h"

#include <string>


class CheckBox : public Widget
{
public:
	explicit CheckBox(Widget* pParent, const std::wstring& text);
	~CheckBox() override;

	bool isChecked() const;
	void setChecked(bool flag = true);

protected:
	LRESULT CALLBACK widgetProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

public: // signals:
	signal<bool> checked;

private:
	bool checked_{ false };
};

