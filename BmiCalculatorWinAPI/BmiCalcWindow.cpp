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
	: Window(nullptr, L"BmiCalcWindow", L"����������� BMI")
{
	setMinSize(Size(400, 180));

	auto pVBoxLayout = new VBoxLayout(this);
	pVBoxLayout->setMargins(Margins(10));
	setMainLayout(pVBoxLayout);

	auto pFormLayout = new FormLayout(this);
	pFormLayout->setSpacing(6);
	pVBoxLayout->addLayout(pFormLayout);

	pFormLayout->addRow(L"����:", new LineEdit(this));
	pFormLayout->addRow(L"���:", new LineEdit(this));
	pFormLayout->addRow(L"�������:", new LineEdit(this));
	
	auto pRadioButtonsLayout = new HBoxLayout(this);

	auto pMenRadioButton = new RadioButton(this, L"�������", true);
	pRadioButtonsLayout->addWidget(pMenRadioButton);

	auto pWomenRadioButton = new RadioButton(this, L"�������");
	pRadioButtonsLayout->addWidget(pWomenRadioButton);

	pRadioButtonsLayout->setAlign(pMenRadioButton, Align::ALIGN_CENTER);
	pRadioButtonsLayout->setAlign(pWomenRadioButton, Align::ALIGN_CENTER);

	pFormLayout->addRow(L"���:", pRadioButtonsLayout);

	
	auto pWindowButtonsLayout = new HBoxLayout(this);
	auto pSpacer = new SpacerItem();
	pWindowButtonsLayout->addSpacerItem(pSpacer);

	auto pCalculateButton = new Button(this, L"����������");
	pCalculateButton->setDefault(true);
	pCalculateButton->clicked.connect(this, &BmiCalcWindow::showBMI);
	pWindowButtonsLayout->addWidget(pCalculateButton);

	auto pExitButton = new Button(this, L"�����");
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
	// TODO: get inputs from LineEdits
	auto calcResult = BmiCalculator::instance().calculate(180, 70, 20, true);

	auto pShowWindow = new ShowBmiWindow(this);
	pShowWindow->setLabelBitmap(calcResult.bitmap());
	pShowWindow->setLabelText(calcResult.text());
	pShowWindow->setLabelTextColor(calcResult.color());
	pShowWindow->show();
}
