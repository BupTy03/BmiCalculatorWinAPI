#pragma once

#include <memory>

#include "Window.h"


class LineEdit;
class RadioButton;
class ShowBmiWindow;


class BmiCalcWindow : public Window
{
public:
	BmiCalcWindow();
	~BmiCalcWindow() override;

	void showBMI();

private:
	void tryCalculateBmi();
	void showError(const std::string& message);

private:
	LineEdit* pHeightInput_{ nullptr };
	LineEdit* pWeightInput_{ nullptr };
	LineEdit* pAgeInput_{ nullptr };
	RadioButton* pMenRadioButton_{ nullptr };
	ShowBmiWindow* pShowWindow_{ nullptr };
};
