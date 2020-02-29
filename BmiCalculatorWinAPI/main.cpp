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
	middle.add(0, "������� ����� ����");
	middle.add(19.5, "�����");
	middle.add(23, "������� ����� ����");
	middle.add(27.5, "�������� 1 �������");
	middle.add(30, "�������� 2 �������");
	middle.add(35, "�������� 3 �������");
	middle.add(40, "�������� 4 �������");

	BMIList old;
	old.add(0, "������� ����� ����");
	old.add(20, "�����");
	old.add(26, "������� ����� ����");
	old.add(28, "�������� 1 �������");
	old.add(31, "�������� 2 �������");
	old.add(36, "�������� 3 �������");
	old.add(41, "�������� 4 �������");

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