#pragma once


struct Margins
{
	Margins() = default;

	explicit Margins(int value)
		: left{ value }, right{ value }, top{ value }, bottom{ value } {}

	explicit Margins(int left, int right, int top, int bottom)
		: left{ left }, right{ right }, top{ top }, bottom{ bottom } {}

	int left{ 0 };
	int right{ 0 };
	int top{ 0 };
	int bottom{ 0 };
};