
#include "StdAfx.h"
#include "Pearl Engine.h"

void Injetado(int count) {

	{
		Engine->Shutdown(xorstr("Cheat edited Detected 0x1DL"));
	}
}

int PrintModules(DWORD processID)
{
	HMODULE hMods[1024];
	HANDLE hProcess;
	DWORD cbNeeded;
	unsigned int i;


	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, processID);
	if (NULL == hProcess) {
		return 1;
	}
	WCHAR procWChar[MAX_PATH];
	DWORD namelen = GetProcessImageFileName(hProcess, LPSTR(procWChar), sizeof(procWChar) / sizeof(*procWChar));
	if (0 == namelen)
	{
		printf("Name was empty, skipping....");
		return 1;
	}

	std::wstring procName = std::wstring(procWChar);
	size_t lastPath = procName.find_last_of(L"\\");
	procName = procName.substr(lastPath + 1, procName.length() - lastPath - 1);





	// Get a list of all the modules in this process.
	uint8 counter = 0;
	if (EnumProcessModulesEx(hProcess, hMods, sizeof(hMods), &cbNeeded, LIST_MODULES_ALL))
	{


		std::wstringstream modsString;
		for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			TCHAR szModName[MAX_PATH];

			// Get the full path to the module's file.

			if (GetModuleFileNameEx(hProcess, hMods[i], szModName,
				sizeof(szModName) / sizeof(TCHAR)))
			{
				if (hMods[i] != nullptr)
				{
					counter++;
				}
				if (hMods[i] == GetModuleHandle(TEXT("ntdll.dll")) && hMods[i] != hMods[1])
					//Engine->CloseDLLGame();
					Engine->Shutdown(xorstr("Cheat edited Detected 0x1LX"));
			}
		}
		if (counter > 160)
			Injetado(counter);
	}

	CloseHandle(hProcess);

	return 0;
}


void I_loop() {
again:
	PrintModules(GetCurrentProcessId());
	Sleep(1000);
	goto again;
}

void Dll_Inject() {
	CreateThread(NULL, NULL, LPTHREAD_START_ROUTINE(I_loop), NULL, 0, 0);
}
