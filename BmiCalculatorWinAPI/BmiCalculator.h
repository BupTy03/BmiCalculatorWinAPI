#pragma once

#include "SegmentsList.h"
#include "Bitmap.h"
#include "Color.h"

#include <string>
#include <array>


struct BmiCalculationResult
{
	explicit BmiCalculationResult(std::wstring text, Bitmap bitmap, Color color)
		: text_{std::move(text)}
		, bitmap_{std::move(bitmap)}
		, color_{color} 
	{}

	std::wstring text() const { return text_; }
	Bitmap bitmap() const { return bitmap_; }
	Color color() const { return color_; }

private:
	std::wstring text_;
	Bitmap bitmap_;
	Color color_;
};


class BmiCalculator
{
	enum class FatLevel { 
		Underweight,
		Normal, 
		Owerweight, 
		FirstGradeObesity, 
		SecondGradeObesity,
		ThirdGradeObesity,
		FourthGradeObesity
	};

	enum { FAT_LEVELS_COUNT = static_cast<std::size_t>(FatLevel::FourthGradeObesity) + 1 };

	using FatLevelsStrings = std::array<std::wstring, FAT_LEVELS_COUNT>;
	using FatLevelsColors = std::array<Color, FAT_LEVELS_COUNT>;
	using FatLevelsList = SegmentsList<double, FatLevel>;
	using AgesBMIList = SegmentsList<int, FatLevelsList>;
	using BitmapsArray = std::array<Bitmap, FAT_LEVELS_COUNT>;

public:
	static BmiCalculator& instance();

public:
	BmiCalculationResult calculate(int height, int weight, int age, bool gender);

private:
	BmiCalculator();
	BmiCalculator(const BmiCalculator&) = delete;
	BmiCalculator& operator=(const BmiCalculator&) = delete;

private:
	FatLevelsStrings fatLevelsStrings_;
	FatLevelsColors fatLevelsColors_;
	BitmapsArray fatLevelsImages_;
	AgesBMIList bmiByAge_;
};

