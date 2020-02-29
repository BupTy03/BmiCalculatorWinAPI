#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "Application.h"
#include "BmiCalcWindow.h"

#include <Windows.h>

#include <cstdio>
#include <iostream>


#ifndef NDEBUG
void SetupConsoleOutput();
#endif

#ifdef TEST
void TestSegmentsList();
#endif


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nShowCmd)
{
	(void)hPrevInstance;
	(void)lpCmdLine;
	(void)nShowCmd;


#ifndef NDEBUG
	SetupConsoleOutput();
	system("chcp 1251");
#endif

	int returnCode = 0;
	{
		Application app(hInstance);

		BmiCalcWindow w;
		w.show();

		returnCode = app.exec();
	}

#ifndef NDEBUG
	system("pause");
#endif

	return returnCode;
}

#ifndef NDEBUG
void SetupConsoleOutput()
{
	AllocConsole();
	AttachConsole(GetProcessId(nullptr));

	auto pIn = stdin;
	auto pOut = stdout;

	freopen_s(&pIn, "CONIN$", "r", stdin);
	freopen_s(&pOut, "CONOUT$", "w", stdout);
}
#endif

#ifdef TEST
void TestSegmentsList()
{
	using BMIList = SegmentsList<double, std::string>;
	using AgesBMIList = SegmentsList<int, BMIList>;

	BMIList middle;
	middle.add(0, "Дефицит массы тела");
	middle.add(19.5, "Норма");
	middle.add(23, "Избыток массы тела");
	middle.add(27.5, "Ожирение 1 степени");
	middle.add(30, "Ожирение 2 степени");
	middle.add(35, "Ожирение 3 степени");
	middle.add(40, "Ожирение 4 степени");

	BMIList old;
	old.add(0, "Дефицит массы тела");
	old.add(20, "Норма");
	old.add(26, "Избыток массы тела");
	old.add(28, "Ожирение 1 степени");
	old.add(31, "Ожирение 2 степени");
	old.add(36, "Ожирение 3 степени");
	old.add(41, "Ожирение 4 степени");

	AgesBMIList list;
	list.add(18, middle);
	list.add(30, old);

	for (int age = 18; age < 100; ++age)
	{
		std::cout << "==========   age = " << age << "   ===========\n";
		for (double bmi = 0.0; bmi < 50; bmi += 0.5)
			std::cout << "BMI = " << bmi << "; " << list[age][bmi] << std::endl;
		std::cout << "===================================\n";
	}
}
#endif