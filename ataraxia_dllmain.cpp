// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <cstdio>
#include <Windows.h>
#include <iostream>
#include <Minhook.h>



uintptr_t rs2Module = (uintptr_t)GetModuleHandleA("ataraxia.exe");

typedef void(__fastcall* _OSDoAction)(int a1, unsigned int a2, signed int actionIndex, int a4, int a5, int a6, __int64 a7, int a8, __int64 a9);
_OSDoAction OSDoAction;

bool has_tinder = false;
bool has_log = false;

//========================================================================== =
//2       9764864 37
//1511    1997368 828981008       2865623897504
//- 289614831      908367755168
//========================================================================== =
//========================================================================== =
//26      9764864 37
//1511    2018401 613536672       1662933602848
//- 289614831      438269114240
//========================================================================== =
void test_chop()
{
	//for (int i = 2; i < 28; i++)
	//{
	for (;;)
	{
		//OSDoAction(47, 52, 3, 10820, -1, -1256303912, 140702549636128, 1, 181230630336);
		OSDoAction(53, 48, 3, 1276, -1, -1256303912, 140702549636128, 1, 181230630336);
		Sleep(6500);
		OSDoAction(2, 9764864, 37, 1511, 1997368, 828981008, 2865623897504, -289614831, 908367755168);
		Sleep(1000);
		OSDoAction(53, 52, 3, 1276, -1, -1256303912, 140702549636128, 1, 181230630336);
		Sleep(6000);
		OSDoAction(2, 9764864, 37, 1511, 1997368, 828981008, 2865623897504, -289614831, 908367755168);
		Sleep(1000);
		OSDoAction(50, 54, 3, 1276, -1, -1256303912, 140702549636128, 1, 181230630336);
		Sleep(7000);
		OSDoAction(2, 9764864, 37, 1511, 1997368, 828981008, 2865623897504, -289614831, 908367755168);
		Sleep(1000);
		OSDoAction(47, 51, 3, 1276, -1, -1256303912, 140702549636128, 1, 181230630336);
		Sleep(7000);
		OSDoAction(2, 9764864, 37, 1511, 1997368, 828981008, 2865623897504, -289614831, 908367755168);
		Sleep(1000);
		OSDoAction(47, 57, 3, 1276, -1, -1256303912, 140702549636128, 1, 181230630336);
		Sleep(7000);
		OSDoAction(2, 9764864, 37, 1511, 1997368, 828981008, 2865623897504, -289614831, 908367755168);
		Sleep(1000);

	}
		//OSDoAction(2, 9764864, 37, 1521, 1879176, 1102953936, 2032568210656, -289614831, 279886952016);
	//}
}

void __fastcall OsDoActionHook(int a1, unsigned int a2, signed int actionIndex, int a4, int a5, int a6, __int64 a7, int a8, __int64 a9)
{
	//if (!has_tinder)
	//{
	//	a1 = 1;
	//	a2 = 9764864;
	//	actionIndex = 38;
	//	a4 = 590;
	//}
	//if (has_tinder && !has_log)
	//{
	//	a1 = 8;
	//	a2 = 9764864;
	//	actionIndex = 31;
	//	a4 = 1511;
	//}
	printf("===========================================================================\n");
	std::cout << a1 << "\t" << a2 << "\t" << actionIndex << "\n";
	std::cout << a4 << "\t" << a5 << "\t" << a6 << "\t" << a7 << "\n";
	std::cout << a8 << "\t" << a9 << "\t" << "\n";
	printf("===========================================================================\n");

	if (!has_tinder)
		has_tinder = true;

	return OSDoAction(a1, a2, actionIndex, a4, a5, a6, a7, a8, a9);
}

 uintptr_t WINAPI Ataraxia(HMODULE hModule)
{
	//--------------------------------------------------
	// Create Console
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	MH_Initialize();

	auto do_action_hook = MH_CreateHook((LPVOID)(rs2Module + 0x73880), &OsDoActionHook, (LPVOID*)&OSDoAction);
	if (do_action_hook != 0)
		printf("Packet hook boom\n");


	auto do_action_enable = MH_EnableHook((void*)do_action_hook);
	if (do_action_enable != 0)
		printf("Packet hook not enable wtf men\n");

	for ( ;; )
	{
		if (GetAsyncKeyState(VK_F2) & 1)
			test_chop();
			//OSDoAction(1, 9764864, 38, 590, 1256898896, 1256828904, 140702529655392, -603965555, 279886952128);

		if (GetAsyncKeyState(VK_F1) & 1)
			break;

		Sleep(2);
	}

	MH_DisableHook((void*)do_action_hook);
	fclose(f);
	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
					   DWORD  ul_reason_for_call,
					   LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CloseHandle(
			CreateThread(nullptr,
				0,
				(LPTHREAD_START_ROUTINE)Ataraxia,
				hModule,
				0,
				nullptr
			)
		);
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
