#include "ShowBmiWindow.h"

#include "logging.h"

#include "VBoxLayout.h"
#include "Label.h"
#include "Color.h"
#include "Button.h"
#include "Bitmap.h"


ShowBmiWindow::ShowBmiWindow(Window* parent)
	: Window(parent, L"ShowBmiWindow", L"BMI")
	, pBitmapLabel_{ new Label(this) }
	, pTextLabel_{ new Label(this) }
{
	auto pMainLayout = new VBoxLayout(this);

	pBitmapLabel_->setVerticalSizePolicy(SizePolicy::Fill);
	pMainLayout->addWidget(pBitmapLabel_);
	pMainLayout->setAlign(pBitmapLabel_, Align::ALIGN_HCENTER);

	Font labelFont(L"Consolas");
	labelFont.setFontWeight(Font::FontWeight::ExtraBold);
	labelFont.setHeight(35);
	pTextLabel_->setFont(labelFont);

	pMainLayout->addWidget(pTextLabel_);
	pMainLayout->setAlign(pTextLabel_, Align::ALIGN_HCENTER);

	auto pOkButton = new Button(this, L"OK");
	pOkButton->clicked.connect(this, &Window::close);
	pOkButton->setDefault(true);
	pMainLayout->addWidget(pOkButton);
	pMainLayout->setAlign(pOkButton, Align::ALIGN_HCENTER);

	pMainLayout->setMargins(Margins(6));
	setMainLayout(pMainLayout);
}

ShowBmiWindow::~ShowBmiWindow()
{
	LogDebug("~ShowBmiWindow()");
}

void ShowBmiWindow::setLabelText(std::wstring text)
{
	pTextLabel_->setText(std::move(text));
}

void ShowBmiWindow::setLabelTextColor(Color color)
{
	pTextLabel_->setTextColor(color);
}

void ShowBmiWindow::setLabelBitmap(const Bitmap& bitmap)
{
	pBitmapLabel_->setBitmap(bitmap);
}
