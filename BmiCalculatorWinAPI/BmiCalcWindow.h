#pragma once

#include "Window.h"


class BmiCalcWindow : public Window
{
public:
	BmiCalcWindow();
	~BmiCalcWindow() override;

	void showBMI();
};
