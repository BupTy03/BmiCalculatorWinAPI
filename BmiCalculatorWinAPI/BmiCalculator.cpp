#include "BmiCalculator.h"

#include "resource.h"

#include <cassert>
#include <sstream>
#include <iomanip>


const BmiCalculator& BmiCalculator::instance()
{
	static const BmiCalculator instance;
	return instance;
}


BmiCalculationResult BmiCalculator::calculate(int height, int weight, int age, bool gender) const
{
	const double heightInMeters = height * 0.01;
	const double bmiValue = weight / (heightInMeters * heightInMeters);

	const FatLevel fatLevel = bmiByAge_[age][bmiValue];
	const std::wstring& message = fatLevelsStrings_.at(static_cast<std::size_t>(fatLevel));

	return BmiCalculationResult(
		L"Ваш BMI: " + std::to_wstring(bmiValue) + L". " + message,
		gender 
			? mensFatLevelsImages_.at(static_cast<std::size_t>(fatLevel)) 
			: womensFatLevelsImages_.at(static_cast<std::size_t>(fatLevel)),
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
		Color(149, 188, 221),
		Color(125, 195, 159),
		Color(253, 216, 20),
		Color(244, 160, 70),
		Color(231, 55, 65),
		Color(231, 55, 65),
		Color(231, 55, 65)
	},
	mensFatLevelsImages_{
		Bitmap(IDB_BITMAP6),
		Bitmap(IDB_BITMAP7),
		Bitmap(IDB_BITMAP8),
		Bitmap(IDB_BITMAP9),
		Bitmap(IDB_BITMAP10),
		Bitmap(IDB_BITMAP10),
		Bitmap(IDB_BITMAP10)
	},
	womensFatLevelsImages_{
		Bitmap(IDB_BITMAP1),
		Bitmap(IDB_BITMAP2),
		Bitmap(IDB_BITMAP3),
		Bitmap(IDB_BITMAP4),
		Bitmap(IDB_BITMAP5),
		Bitmap(IDB_BITMAP5),
		Bitmap(IDB_BITMAP5)
	}
{
	FatLevelsList littleAge;
	littleAge.add(0, FatLevel::Underweight);
	littleAge.add(15.3, FatLevel::Normal);
	littleAge.add(17, FatLevel::Owerweight);
	littleAge.add(18.3, FatLevel::FirstGradeObesity);
	littleAge.add(20.3, FatLevel::SecondGradeObesity);
	littleAge.add(25, FatLevel::ThirdGradeObesity);
	littleAge.add(35, FatLevel::FourthGradeObesity);

	FatLevelsList middleAge;
	middleAge.add(0, FatLevel::Underweight);
	middleAge.add(19.5, FatLevel::Normal);
	middleAge.add(23, FatLevel::Owerweight);
	middleAge.add(27.5, FatLevel::FirstGradeObesity);
	middleAge.add(30, FatLevel::SecondGradeObesity);
	middleAge.add(35, FatLevel::ThirdGradeObesity);
	middleAge.add(40, FatLevel::FourthGradeObesity);

	FatLevelsList oldAge;
	oldAge.add(0, FatLevel::Underweight);
	oldAge.add(20, FatLevel::Normal);
	oldAge.add(26, FatLevel::Owerweight);
	oldAge.add(28, FatLevel::FirstGradeObesity);
	oldAge.add(31, FatLevel::SecondGradeObesity);
	oldAge.add(36, FatLevel::ThirdGradeObesity);
	oldAge.add(41, FatLevel::FourthGradeObesity);


	bmiByAge_.add(0, std::move(littleAge));
	bmiByAge_.add(18, std::move(middleAge));
	bmiByAge_.add(30, std::move(oldAge));
}
