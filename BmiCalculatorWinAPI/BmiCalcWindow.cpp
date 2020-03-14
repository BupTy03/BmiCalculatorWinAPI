#include "BmiCalcWindow.h"

#include "logging.h"
#include "resource.h"

#include "BmiCalculator.h"
#include "ShowBmiWindow.h"
#include "FormLayout.h"
#include "HBoxLayout.h"
#include "VBoxLayout.h"
#include "LineEdit.h"
#include "RadioButton.h"
#include "Button.h"
#include "SpacerItem.h"


BmiCalcWindow::BmiCalcWindow()
	: Window(nullptr, L"BmiCalcWindow", L"Калькулятор BMI")
	, pHeightInput_{ new LineEdit(this) }
	, pWeightInput_{ new LineEdit(this) }
	, pAgeInput_{ new LineEdit(this) }
	, pMenRadioButton_{ new RadioButton(this, L"мужской", true) }
{
	setMinSize(Size(400, 180));

	auto pVBoxLayout = new VBoxLayout(this);
	pVBoxLayout->setMargins(Margins(10));
	setMainLayout(pVBoxLayout);

	auto pFormLayout = new FormLayout(this);
	pFormLayout->setSpacing(6);
	pVBoxLayout->addLayout(pFormLayout);

	pFormLayout->addRow(L"Рост:", pHeightInput_);
	pFormLayout->addRow(L"Вес:", pWeightInput_);
	pFormLayout->addRow(L"Возраст:", pAgeInput_);
	
	auto pRadioButtonsLayout = new HBoxLayout(this);

	pRadioButtonsLayout->addWidget(pMenRadioButton_);

	auto pWomenRadioButton = new RadioButton(this, L"женский");
	pRadioButtonsLayout->addWidget(pWomenRadioButton);

	pRadioButtonsLayout->setAlign(pMenRadioButton_, Align::ALIGN_CENTER);
	pRadioButtonsLayout->setAlign(pWomenRadioButton, Align::ALIGN_CENTER);

	pFormLayout->addRow(L"Пол:", pRadioButtonsLayout);

	
	auto pWindowButtonsLayout = new HBoxLayout(this);
	auto pSpacer = new SpacerItem();
	pWindowButtonsLayout->addSpacerItem(pSpacer);

	auto pCalculateButton = new Button(this, L"Рассчитать");
	pCalculateButton->setDefault(true);
	pCalculateButton->clicked.connect(this, &BmiCalcWindow::showBMI);
	pWindowButtonsLayout->addWidget(pCalculateButton);

	auto pExitButton = new Button(this, L"Выход");
	pExitButton->clicked.connect(this, &Window::close);
	pWindowButtonsLayout->addWidget(pExitButton);

	pWindowButtonsLayout->setAlign(pCalculateButton, Align::ALIGN_BOTTOM);
	pWindowButtonsLayout->setAlign(pExitButton, Align::ALIGN_BOTTOM);

	pVBoxLayout->addLayout(pWindowButtonsLayout);
}

BmiCalcWindow::~BmiCalcWindow()
{
	LogDebug("~BmiCalcWindow()");
}

void BmiCalcWindow::showBMI()
{
	tryCalculateBmi();
}

void BmiCalcWindow::tryCalculateBmi()
{
	BmiCalculationResult calcResult;
	try
	{
		const int height = std::stoi(pHeightInput_->text());
		const int weight = std::stoi(pWeightInput_->text());
		const int age = std::stoi(pAgeInput_->text());
		const bool gender = pMenRadioButton_->isChecked();

		calcResult = BmiCalculator::instance().calculate(height, weight, age, gender);

		auto pShowWindow = new ShowBmiWindow(this);
		pShowWindow->setLabelBitmap(calcResult.bitmap());
		pShowWindow->setLabelText(calcResult.text());
		pShowWindow->setLabelTextColor(calcResult.color());
		pShowWindow->show();
	}
	catch (std::runtime_error& err)
	{
		showError(err.what());
		return;
	}
	catch (...)
	{
		showError("Произошла непредвиденная ошибка :(");
		return;
	}
}

void BmiCalcWindow::showError(const std::string& message)
{
	MessageBoxA(getWindowHandle(), message.c_str(), "Ошибка", MB_ICONERROR | MB_OK);
}
