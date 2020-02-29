#include "BmiCalculator.h"

#include <cassert>
#include <sstream>


BmiCalculator& BmiCalculator::instance()
{
	static BmiCalculator instance;
	return instance;
}


static std::wstring DoubleToString(double value, int precision)
{
	assert(precision >= 0);

	std::wostringstream sstr;
	sstr.precision(precision);
	sstr << value;
	return sstr.str();
}


BmiCalculationResult BmiCalculator::calculate(int height, int weight, int age, bool gender)
{
	// TODO: сделать чтобы gender(пол) на что-то влиял
	const double heightInMeters = height * 0.01;
	const double bmiValue = weight / static_cast<double>(heightInMeters * heightInMeters);

	const FatLevel fatLevel = bmiByAge_[age][bmiValue];
	const std::wstring& message = fatLevelsStrings_.at(static_cast<std::size_t>(fatLevel));

	return BmiCalculationResult(
		L"Ваш BMI: " + DoubleToString(bmiValue, 2) + L". " + message,
		fatLevelsImages_.at(static_cast<std::size_t>(fatLevel)),
		fatLevelsColors_.at(static_cast<std::size_t>(fatLevel))
	);
}


BmiCalculator::BmiCalculator()
	: fatLevelsStrings_{
		L"Дефицит массы тела",
		L"Норма",
		L"Избыток массы тела",
		L"Ожирение 1 степени",
		L"Ожирение 2 степени",
		L"Ожирение 3 степени",
		L"Ожирение 4 степени"
	},
	fatLevelsColors_{
		Color::makeRed(),
		Color::makeGreen(),
		Color::makeRed(),
		Color::makeRed(),
		Color::makeRed(),
		Color::makeRed(),
		Color::makeRed(),
	},
	fatLevelsImages_{
		Bitmap(),
		Bitmap(),
		Bitmap(),
		Bitmap(),
		Bitmap(),
		Bitmap(),
		Bitmap(),
	}
{
	// средний возраст
	FatLevelsList middleAge;
	middleAge.add(0, FatLevel::Underweight);
	middleAge.add(19.5, FatLevel::Normal);
	middleAge.add(23, FatLevel::Owerweight);
	middleAge.add(27.5, FatLevel::FirstGradeObesity);
	middleAge.add(30, FatLevel::SecondGradeObesity);
	middleAge.add(35, FatLevel::ThirdGradeObesity);
	middleAge.add(40, FatLevel::FourthGradeObesity);

	// старший возраст
	FatLevelsList oldAge;
	oldAge.add(0, FatLevel::Underweight);
	oldAge.add(20, FatLevel::Normal);
	oldAge.add(26, FatLevel::Owerweight);
	oldAge.add(28, FatLevel::FirstGradeObesity);
	oldAge.add(31, FatLevel::SecondGradeObesity);
	oldAge.add(36, FatLevel::ThirdGradeObesity);
	oldAge.add(41, FatLevel::FourthGradeObesity);

	// таблица возрастов
	bmiByAge_.add(18, std::move(middleAge));
	bmiByAge_.add(30, std::move(oldAge));
}
