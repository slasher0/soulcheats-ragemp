#include <iostream>
#include <string>
#include <ctype.h>
#include <Windows.h>
#include <tlhelp32.h>
#include <Shlwapi.h>
#include <thread>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>
namespace AntiCrack
{

	int getProcID(const std::string& p_name)
	{
		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		PROCESSENTRY32 structprocsnapshot = { 0 };

		structprocsnapshot.dwSize = sizeof(PROCESSENTRY32);

		if (snapshot == INVALID_HANDLE_VALUE)return 0;
		if (Process32First(snapshot, &structprocsnapshot) == FALSE)return 0;

		while (Process32Next(snapshot, &structprocsnapshot))
		{
			if (!strcmp(structprocsnapshot.szExeFile, p_name.c_str()))
			{
				CloseHandle(snapshot);
				return structprocsnapshot.th32ProcessID;
			}
		}
		CloseHandle(snapshot);
		return 0;

	}

	bool isProcRunning(std::string process) {
		if (getProcID(process) == 0) {
			return false;
		}
		else
		{
			return true;
		}
	}

	void Tick() {
		if (isProcRunning("ida64.exe") || isProcRunning("dotPeek64.exe") || isProcRunning("ida32.exe") || isProcRunning("ida.exe") || isProcRunning("reclass.net.exe") || isProcRunning("reclass.exe") || isProcRunning("heyrays.exe") || isProcRunning("lighthouse.exe") || isProcRunning("cheatengine-x86_64.exe") || isProcRunning("classinformer.exe") || isProcRunning("ida-x86emu.exe") || isProcRunning("cffexplorer.exe") || isProcRunning("winhex.exe") || isProcRunning("hiew.exe") || isProcRunning("fiddler.exe") || isProcRunning("httpdebugger.exe") || isProcRunning("httpdebuggerpro.exe") || isProcRunning("scylla.exe") || isProcRunning("Cheat Engine.exe"))
		{
			MessageBox(NULL, "Please close any fucking reverse engineering tools and restart the program.", "Error", MB_ICONERROR);
			ExitProcess(0);
		}
	}

}