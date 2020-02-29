#pragma once

#include "signal.h"
#include "Widget.h"

#include <string>


class RadioButton : public Widget
{
public:
	explicit RadioButton(Widget* pParent, const std::wstring& text, bool beginGroup = false);
	~RadioButton() override;

	bool isChecked() const;
	void setChecked(bool flag = true);

protected:
	LRESULT CALLBACK widgetProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

public: // signals:
	signal<> selected;

private:
	bool checked_{ false };
};