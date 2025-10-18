#include "BanSystem.h"
#include "stdafx.h"
#include "PearlGui.h"
#include "HDRReader.h"
#include "splash.h"
#include <TlHelp32.h>



BanSystem* banSystem = NULL;
void OpenConsoleWindow();

PearlEngine* Engine = NULL;
HDRReader* hdrReader = NULL;
std::string basePath = "";
int SkillBaseTime = GetTickCount() + 25000;

void LoadCrypto();



void Shutdown(string message = "");

bool _fexists(std::string& filename)
{
	std::ifstream ifile(filename.c_str());
	return (bool)ifile;
}

std::string getexepath()
{

	char result[MAX_PATH];
	string ret = std::string(result, GetModuleFileName(NULL, result, MAX_PATH));
	return ret.substr(0, ret.find_last_of("\\"));
}

DWORD GetFileSize(const char* FileName)
{
	std::ifstream in(FileName, std::ifstream::ate | std::ifstream::binary);
	return in.tellg();
}

void ExitTimer()
{
	while (true)
	{
		Sleep(5000);
		exit(0);
		FreeLibrary(GetModuleHandle(NULL));
		TerminateProcess(GetCurrentProcess(), 0);
	}
}

void Shutdown(string message)
{
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ExitTimer, NULL, NULL, NULL);
	Engine->power = false;
	string s1 = xorstr("The file is corrupted. Please contact with administrator.\n");
	string s2 = xorstr("If you constantly see this message, ");
	string s3 = xorstr("disable the anti-virus program.");

	if (message == "")
		MessageBoxA(NULL, string(s1 + s2 + s3).c_str(), xorstr("KnightOnline.exe"), MB_OK | MB_ICONEXCLAMATION);
	else
		MessageBoxA(NULL, message.c_str(), xorstr("KnightOnLine.exe"), MB_OK | MB_ICONEXCLAMATION);

	exit(0);
	FreeLibrary(GetModuleHandle(NULL));
	TerminateProcess(GetCurrentProcess(), 0);
}

// dosya yol değiştirme

void eraseSubStr(std::string& mainStr, std::string& toErase)
{
	std::transform(mainStr.begin(), mainStr.end(), mainStr.begin(), ::tolower);
	std::transform(toErase.begin(), toErase.end(), toErase.begin(), ::tolower);

	size_t pos = mainStr.find(toErase);
	if (pos != std::string::npos)
	{
		// If found then erase it from string
		mainStr.erase(pos, toErase.length());
	}
}

typedef HANDLE(WINAPI* tCreateFileA)(
	LPCSTR                lpFileName,
	DWORD                 dwDesiredAccess,
	DWORD                 dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD                 dwCreationDisposition,
	DWORD                 dwFlagsAndAttributes,
	HANDLE                hTemplateFile
	);

tCreateFileA oCreateFileA;
tCreateFileA oCreateFileCameraA;

HANDLE WINAPI hkCreateFileCameraA(LPCSTR                lpFileName,
	DWORD                 dwDesiredAccess,
	DWORD                 dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD                 dwCreationDisposition,
	DWORD                 dwFlagsAndAttributes,
	HANDLE                hTemplateFile)
{
	/*printf(lpFileName);
	printf("\n");*/


	return oCreateFileCameraA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

std::string __tmpfl = "";
DWORD _PID = 0;
HCRYPTKEY hKey = NULL;
std::string xCodeTMP = xorstr("Misc\\river\\caust36.dxt");

HANDLE WINAPI hkCreateFileA(LPCSTR                lpFileName,
	DWORD                 dwDesiredAccess,
	DWORD                 dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD                 dwCreationDisposition,
	DWORD                 dwFlagsAndAttributes,
	HANDLE                hTemplateFile)
{

	string fl = lpFileName;

	/**/
	/*if (fl.find(xorstr("Chr")) != string::npos)
	{

		Engine->str_replace(fl, "Chr", "Game\\Chr");
		return oCreateFileA(fl.c_str(), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	}*/
	/**/

	if ((int)dwFlagsAndAttributes != 137)

		return oCreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	else
	{

		xCodeTMP = string_format(xorstr("Misc\\river\\caust36_%d.dxt"), _PID);
		PBYTE pbBuffer = NULL;
		HANDLE hSourceFile = oCreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, FILE_ATTRIBUTE_NORMAL, hTemplateFile);
		HANDLE hDestinationFile = oCreateFileA(xCodeTMP.c_str(), FILE_WRITE_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE != hSourceFile && INVALID_HANDLE_VALUE != hDestinationFile)
		{

			bool fEOF = false;
			DWORD dwCount;
			LARGE_INTEGER size;
			GetFileSizeEx(hSourceFile, &size);
			LONG len = (size.u.HighPart << (LONG)32) | size.u.LowPart;
			DWORD dwBlockLen = len % 2 == 0 ? 32 : 31;
			DWORD dwBufferLen = dwBlockLen;
			if ((pbBuffer = (PBYTE)malloc(dwBufferLen)))
			{
				if (ReadFile(hSourceFile, pbBuffer, 4, &dwCount, NULL))
				{
					WriteFile(hDestinationFile, pbBuffer, dwCount, &dwCount, NULL);
				}
				ZeroMemory(pbBuffer, dwBufferLen);
				dwCount = 0;
				do
				{
					if (ReadFile(hSourceFile, pbBuffer, dwBlockLen, &dwCount, NULL))
					{
						if (dwCount < dwBlockLen)
						{
							fEOF = TRUE;
						}
						if (CryptDecrypt(hKey, 0, 1, 0, pbBuffer, &dwCount))
						{
							WriteFile(hDestinationFile, pbBuffer, dwCount, &dwCount, NULL);
						}
					}
				} while (!fEOF);
			}

			if (pbBuffer)
				free(pbBuffer);

			if (hSourceFile)
				CloseHandle(hSourceFile);

			if (hDestinationFile)
				CloseHandle(hDestinationFile);

			return oCreateFileA(xCodeTMP.c_str(), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, FILE_ATTRIBUTE_NORMAL, hTemplateFile);
		}
	}

	return oCreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}
// dosya yol değiştirme ---------------------

#include <windows.h>
#include <tchar.h>

typedef BOOL(WINAPI* LPFN_ISWOW64PROCESS)(HANDLE, PBOOL);
LPFN_ISWOW64PROCESS fnIsWow64Process;

bool IsWOW64()
{
	BOOL bIsWow64 = FALSE;

	// IsWow64Process fonksiyonunu dinamik olarak yüklemek için
	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process");

	if (fnIsWow64Process != NULL)
	{
		if (!fnIsWow64Process(GetCurrentProcess(), &bIsWow64))
		{
			// Burada bir hata durumu işleyebilirsiniz
			return false;
		}
	}
	return bIsWow64;
}

HANDLE myMutex = NULL;
#define CONSOLE_MODE 0  // 0 OLUNCA CONSOL KAPALI - 1 OLUNCA KONSOL AÇIK OLUYOR.
extern  "C"  __declspec(dllexport) void __cdecl Init()
{

	oCreateFileA = (tCreateFileA)DetourFunction((PBYTE)CreateFileA, (PBYTE)hkCreateFileA); // dosya yol değiştirme ---------------------
#if HOOK_SOURCE_VERSION == 1098
	//oCreateFileCameraA = (tCreateFileA)DetourFunction((PBYTE)0x004EFA8C, (PBYTE)hkCreateFileCameraA); // kamera dosya yol değiştirme ---------------------
	//*(DWORD*)(0x004EFA8C) = 0xE8; // for camera oCreateFileCameraA
	*(DWORD*)(0x004EFA7E) = 0x89;	// n3camera için şifreleme
	*(DWORD*)(0x004E97D1) = 0x89;	// n3camera için şifreleme
	*(DWORD*)(0x004EB736) = 0x89;	// n3camera için şifreleme
	*(DWORD*)(0x004EC31E) = 0x89;	// n3camera için şifreleme
	*(DWORD*)(0x004EB652) = 0x89;	// n3camera için şifreleme
	*(DWORD*)(0x004EB8ED) = 0x89;	// n3camera için şifreleme
	*(DWORD*)(0x004EC61E) = 0x89;	// n3camera için şifreleme
	*(DWORD*)(0x004EB9C6) = 0x89;	// n3camera için şifreleme //açılmicak
#endif

#if CONSOLE_MODE == 1
	AllocConsole();
	freopen(xorstr("CONOUT$"), xorstr("w"), stdout);
#endif	

	PatchMulti();

	basePath = getexepath();
	string myname(basePath + xorstr("\\KnightOnLine.exe"));
	if (!_fexists(myname))
	{
		MessageBoxA(NULL, xorstr("KnightOnLine.exe name invalid"), xorstr("Error"), MB_OK | MB_ICONEXCLAMATION);
		Shutdown();
		return;
	}

	uint8_t	multicount = 0;
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (Process32First(snapshot, &entry) == TRUE)
	{
		while (Process32Next(snapshot, &entry) == TRUE)
		{
			std::string a = entry.szExeFile;
			std::size_t pos = a.find(xorstr("KnightOnLine.exe"));

			if (pos != -1)
				multicount++;
		}
	}

	if (multicount > 3)		// Açılacak olan multi client sayısı
	{
		ExitProcess(0);
		DWORD skillbase2 = 0;
		DWORD skillid2 = 0;
		DWORD value3 = 0;
		int counter = 0;
		unsigned int a2 = 0;
		DWORD skillbase4 = 0;
		DWORD skillid5 = 0;
		DWORD value6 = 0;
		int counter6 = 0;

		__asm
		{
			push   0
			push  0
			mov eax, 0
			mov ecx, 0
			call eax
			mov a2, eax
		}
	}

	/*************************************/

	myname = basePath + xorstr("\\DeaFSoft.dll");
	if (!_fexists(myname))
	{
		MessageBoxA(NULL, xorstr("\\DeaFSoft.dll"), xorstr("Need This File In Directory"), MB_OK | MB_ICONEXCLAMATION);
		Shutdown();
		return;
	}

	char nm[100];
	GetModuleBaseNameA(GetCurrentProcess(), GetModuleHandle(NULL), nm, sizeof(nm));
	string _nm(nm);

	Engine = new PearlEngine(basePath + xorstr("\\"));
	hdrReader = new HDRReader(basePath + xorstr("\\"));
	//Xtreme new Start logger
	Engine->SetOSVersion();
	Engine->beginlog();
	char text[1024]; memset(text, 0, 1024);
	sprintf_s(text, "[KK][%x]", (((reinterpret_cast<unsigned long>(Engine->Settings->szReg) ^ 0xBB) * 0x94) & 0xCC));
	Engine->InsertLog(text);
	sprintf(text, "[SZ1][%x]", (((static_cast<unsigned long>(Engine->Settings->ModuleInfo.SizeOfImage) ^ 0xAC) * 0x75) & 93));
	Engine->InsertLog(text, false);
	sprintf_s(text, "[SZ2][%x]", (((reinterpret_cast<unsigned long>(Engine->Settings->ModuleInfo.lpBaseOfDll) ^ 0xB2) * 0x36) & 20));
	Engine->InsertLog(text, false);
	sprintf_s(text, "[SZ3][%x]", (((reinterpret_cast<unsigned long>(Engine->Settings->ModuleInfo.EntryPoint) ^ 0xDE) * 0x71) & 78));
	Engine->InsertLog(text, false);
	sprintf_s(text, "CSRSS process id: %i", Engine->Settings->csrsspid);
	Engine->InsertLog(text, false);
	sprintf_s(text, "WLG process id: %i", Engine->Settings->winlogonpid);
	Engine->InsertLog(text, false);
	sprintf_s(text, "[SM1][%x]", (((reinterpret_cast<unsigned long>(Engine->Settings->MyModule) ^ 0x72) * 0x41) & 33));
	Engine->InsertLog(text, false);
	sprintf_s(text, "[SM2][%x]", (((static_cast<unsigned long>(Engine->Settings->gamethread) ^ 0x67) * 0x88) & 0xE3));
	Engine->InsertLog(text, false);

	if (IsWOW64()) {
		Engine->InsertLog("(x64) sistem belirlendi");
		Engine->InsertLog("Sistem servisi başlatılıyor", false);
		Engine->InsertLog("Sistem servisi başarıyla başlatıldı", false);
	}

	Engine->InsertLog("Islemci calisma kumesi olusturuldu.");
	Engine->InsertLog("Bellek arayuzu entegrasyonu basladi");

	Engine->InsertLog("Bellek arayuzu entegrasyonu tamamlandi");
	Engine->InsertLog("Essiz kimlik olusturuldu");

	sprintf_s(text, "Yansı oluşturuldu [%x]", (((static_cast<unsigned long>(Engine->Settings->debugpid) ^ 0x72) * 0x41) & 33));
	Engine->InsertLog(text, false);

	Engine->InsertLog("AA sistemi aktif edildi", false);
	Engine->InsertLog("Haklar alınıyor", false);
	Engine->EnableDebugPrivilege();
	Engine->InsertLog("Haklar elde edildi", false);

	Engine->InsertLog("Sistem ayarları yapılıyor");
	Engine->ProtectLocalHooks((char*)"kernel32.dll", (char*)"WriteProcessMemory");
	Engine->ProtectLocalHooks((char*)"kernel32.dll", (char*)"ReadProcessMemory");
	Engine->ProtectLocalHooks((char*)"kernel32.dll", (char*)"LoadLibraryA");
	Engine->ProtectLocalHooks((char*)"kernel32.dll", (char*)"OpenProcess");
	Engine->ProtectLocalHooks((char*)"kernel32.dll", (char*)"GetTickCount");
	Engine->ProtectLocalHooks((char*)"kernel32.dll", (char*)"QueryPerformanceCounter");
	Engine->ProtectLocalHooks((char*)"Winmm.dll", (char*)"timeGetTime");

	//Engine->SystemSend();
	Engine->InsertLog("Sistem segmentleri kontrol ediliyor", false);
	
	/*Engine->SystemLog();*/ // Xtreme Yeni Log sistemi
	Engine->InsertLog("Log sistemi aktif ediliyor", false);
	//SystemPointers();
	Engine->InsertLog("Adres yönergeleri aktif ediliyor", false);
	//InterceptFunc(IMAGEBASE + 0x005C96E7, static_cast<void*>(getmygm));
	Engine->InsertLog("Kimlik yönetimi aktif ediliyor", false);
	//InterceptFunc(IMAGEBASE + 0x0064BC04, static_cast<void*>(WideScreen));
	Engine->InsertLog("Genis ekran desteği aktif edildi", false);

	//Xtreme new Logger end

	LoadCrypto();
	banSystem = new BanSystem;
	
	bool a = banSystem->RegCheck();
	bool b = banSystem->FileCheck();

#if(ANTICHEAT_MODE)
	if (a || b)
	{
#if 0
		banSystem->RemoveBan();
		Shutdown("Clienti tekrar başlatın");
#else
		Shutdown("Pc Blocked.");
#endif
		return;
	}

#endif
	UIMain();
}

void OpenConsoleWindow()
{
	AllocConsole();

	freopen("CONOUT$", "a", stdout);
	freopen("CONIN$", "r", stdin);
}

/*4 bot*/

#define lord_Intercept void  __declspec( naked ) __stdcall
#define _sosm __asm
#define IMAGEBASE 0x0

unsigned long Call_KelimeOyunu = IMAGEBASE + 0x0051E124;
unsigned long Call_Devam_mokoko = IMAGEBASE + 0x0051E5D5;
unsigned long Call_Devam_keko = IMAGEBASE + 0x0051E3F0;
unsigned long Call_Devam_ali = IMAGEBASE + 0x0051E4E1;

lord_Intercept BotChatBarDamage()
{
	_sosm{
		CMP EDI,EBX
		JGE Call_Devam
		CMP ESI,-1
		JE Call_Devam_keke
		CMP ESI,0x270F
		JG Call_Devam_keke
		JMP Call_KelimeOyunu
		Call_Devam :
		JLE Call_Devam_Devam
		MOV EBP,0x0F
		JMP Call_Devam_ali
		Call_Devam_Devam :
		MOV ECX,DWORD PTR DS : [0x00F58F70]
		JMP Call_Devam_mokoko
		Call_Devam_keke :
		MOV EBP,0x0F
		JMP Call_Devam_keko
	}
}

void InterceptFunc(unsigned long Offset, void* func) {
	char* yareyare;
	unsigned long oldProtect;
	unsigned long Dummy;
	char Buffer[] = "\xE9\x00\x00\x00\x00";
	yareyare = reinterpret_cast<char*>(Offset);
	VirtualProtect(yareyare, 5L, PAGE_EXECUTE_READWRITE, &oldProtect);
	*reinterpret_cast<long*>(Buffer + 1) = reinterpret_cast<long>(func) - reinterpret_cast<long>(yareyare) - 5;
	*Buffer = static_cast<unsigned char>(0xE9);
	RtlMoveMemory(yareyare, Buffer, 5);
	VirtualProtect(yareyare, 5L, oldProtect, &Dummy);
}
/*4 bot*/

// Kontrol edilecek pencere sınıflarını tanımlayın
std::vector<std::string> windowClasses = {
	"Window", // Cheat Engine
	"ThunderRT6FormDC", //Asd44,Alican koxp
	"WdcWindow",
	"WindowsForms10.Window.8.app.0.141b42a_r7_ad1",
	"PROCEXPLPROCEXPL",
	"TreeListWindowClass",
	"ProcessHacker",
	"PhTreeNew",
	"PROCEXPL",            // Process explorer
"TreeListWindowClass", // Process explorer (Process windows)
"ProcessHacker",       // Process Hacker	
"PhTreeNew",           // Process Hakcer (Process windows)
"SysListView32",       // Lista de processos do process explorer
"TformSettings",
"Afx:400000:8:10011:0:20575",
"WindowsForms10.STATIC.app.0.2bf8098_r10_ad1",// /*
"WindowsForms10.EDIT.app.0.2bf8098_r10_ad1",// */
"TUserdefinedform",
"TformAddressChange",
"TMemoryBrowser",
"TFoundCodeDialog",
"TXmHexEdit",
"TFormRAMOpen.UnicodeClass",
"OLLYDBG",
"Dll-Injector",
"AutoIt v3",
"AutoIt v3 GUI",
"N-Jector by NOOB",
"Projectoinsatntomaosmdoomqwd",
"Twnd_tm_Main.ProcessHacker",
"ProcessHacker",
"OllyDbg",
"ollydbg",
"CiM's",
"ConsoleWindow",
"Afx:400000:8:10011:0:4a07e3",
"procmon_window_class",
"processhacker",
"WindbgMainClass",
"procexpl",
"unhookerclass",
"WinDbgFrameClass",
"Zeta Debugger",
"ObsidianGUI",
"Rock Debugger",
"idawindow",
"idaview",
"TForm1",
"SysListView32"
"SysListView32"
};

// Pencere stili ve genişletilmiş stili için yapı tanımı
struct WindowStyle {
	LONG style;
	LONG exStyle;
};

// Kontrol edilecek pencere stillerini ve genişletilmiş stilleri tanımlayın
std::vector<WindowStyle> windowStyles = {
	{ 0x16CF0000, 0x00010110 }, // Cheat Engine
	{ 0x16C80000, 0x00040100 }
};

// Pencere sınıfını kontrol eden fonksiyon
BOOL CALLBACK EnumWindowsProcForClassScan(HWND hwnd, LPARAM lParam) {
	TCHAR className[256];
	if (GetClassName(hwnd, className, sizeof(className) / sizeof(TCHAR))) {
		for (const auto& windowClass : windowClasses) {
			if (_tcscmp(className, windowClass.c_str()) == 0) {
				if (_tcscmp(className, "ThunderRT6FormDC") == 0) {
					// ThunderRT6FormDC sınıfı için style ve exstyle kontrolü yap
					LONG style = GetWindowLong(hwnd, GWL_STYLE);
					LONG exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
					if (style == 0x16C80000 && exStyle == 0x00040100) {
						*reinterpret_cast<HWND*>(lParam) = hwnd;
						return FALSE;
					}
				}
				else {
					// Diğer sınıflar için sadece sınıf adı kontrolü yap
					*reinterpret_cast<HWND*>(lParam) = hwnd;
					return FALSE;
				}
			}
		}
	}
	return TRUE;
}

extern void Capture_Screen();
// Pencereleri kontrol eden ve kapatan fonksiyon
bool CaptureScreenStart = false;
void CheckClassThread() {
	while (true) {
		HWND foundHwnd = NULL;
#if ANTICHEAT_MODE == 1
		EnumWindows(EnumWindowsProcForClassScan, reinterpret_cast<LPARAM>(&foundHwnd));
#endif
		if (foundHwnd != NULL) {
			SendMessage(foundHwnd, WM_CLOSE, 0, 0);
			char wnd_titlee[256];
			GetWindowTextA(foundHwnd, wnd_titlee, sizeof(wnd_titlee));
			if (CaptureScreenStart)
				Capture_Screen();

			Shutdown(string_format("%s", wnd_titlee));
			RaiseException(EXCEPTION_NONCONTINUABLE_EXCEPTION, 0, 0, NULL);
		}
		Sleep(1000); // 1 saniye bekleme
		if (!CaptureScreenStart)
			CaptureScreenStart = true;
	}
}

//  2369 XTREME
DWORD WINAPI ASMYAMA()
{
	HANDLE hProcess = 0;
	hProcess = GetCurrentProcess();

	//OLD 0x1207500120128966  
	// 2369  new crypto 0x1208500120128998 
	BYTE G_private_key2472[] = { 0x68,0x01,0x50,0x08,0x12 };//Jvcrypton	
	BYTE G_private_keyop2472[] = { 0x68,0x98,0x89,0x12,0x20 };//Crypton

	// Hepsi 2383 tamam
	//PUSH 12075001 
	WriteProcessMemory(hProcess, (void*)0x4A5620, (void*)&G_private_key2472, sizeof(G_private_key2472), NULL);
	WriteProcessMemory(hProcess, (void*)0x4A65C3, (void*)&G_private_key2472, sizeof(G_private_key2472), NULL);
	WriteProcessMemory(hProcess, (void*)0x4DDD7F, (void*)&G_private_key2472, sizeof(G_private_key2472), NULL);

	//PUSH 20128966
	WriteProcessMemory(hProcess, (void*)0x4DDD84, (void*)&G_private_keyop2472, sizeof(G_private_keyop2472), NULL);
	WriteProcessMemory(hProcess, (void*)0x4A5625, (void*)&G_private_keyop2472, sizeof(G_private_keyop2472), NULL);
	WriteProcessMemory(hProcess, (void*)0x4A65C8, (void*)&G_private_keyop2472, sizeof(G_private_keyop2472), NULL);

	// 2369   15572 = 3CD4     | 43791 = AB0F
	BYTE G_GameExePort_2472[] = { 0x68,0xD4,0x3C };// Game Port	15572
	WriteProcessMemory(hProcess, (void*)0x82C248, (void*)&G_GameExePort_2472, sizeof(G_GameExePort_2472), NULL);
	WriteProcessMemory(hProcess, (void*)0x4E53AD, (void*)&G_GameExePort_2472, sizeof(G_GameExePort_2472), NULL);
	WriteProcessMemory(hProcess, (void*)0x82CA55, (void*)&G_GameExePort_2472, sizeof(G_GameExePort_2472), NULL);

	// VERSİON 2369 To 2472 = 9A8    |2239 8BF
	BYTE G_GameExeV1[] = { 0x81, 0x7C, 0x24, 0x14, 0xA8, 0x09 };
	WriteProcessMemory(hProcess, (void*)0x4DDE07, (void*)&G_GameExeV1, sizeof(G_GameExeV1), NULL);

	// VERSİON
	BYTE G_GameExeV2[] = { 0xB8, 0xA8, 0x09 };
	WriteProcessMemory(hProcess, (void*)0x4D3B20, (void*)&G_GameExeV2, sizeof(G_GameExeV2), NULL);
	return TRUE;
}


BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpvReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(hModule);
		
		/*SPLASH m_splash[6];
		for (int i = 0; i < 6; i++)
			m_splash[i].Init(NULL, hModule, IDB_BITMAP1 + i);
		for (int i = 0; i < 6; i++)
		{
			m_splash[i].Show();
			Sleep(600);
			m_splash[i].Hide();
		}*/
		Init();
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ASMYAMA, NULL, NULL, NULL);
		InterceptFunc(0x0051E107, (void*)BotChatBarDamage);
		remove(DELETE_CAPTURE_FILE1);
		Engine->ClassThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CheckClassThread, NULL, NULL, NULL);
	}
	return TRUE;
}

// Uif Şifreleme başlangıç
HCRYPTPROV hCryptProv = NULL;
HCRYPTHASH hHash = NULL;
DWORD Func_Load = 0;
const std::string& fileStr = "";


bool has_suffix(const std::string& str, const std::string& suffix)
{
	return str.size() >= suffix.size() &&
		str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

void __stdcall uifEncrpytOperation()
{
#if CONSOLE_MODE == 1
	if (!fileStr.empty() && (has_suffix(fileStr, xorstr(".dfsoft")) || has_suffix(fileStr, xorstr(".uif"))))
	{
		printf(fileStr.c_str());  //Oyun açılırken yüklediği herşeyi buradan görebiliyoruz.
		printf(" is loading...\n");
	}
#endif
	xCodeTMP = fileStr;

	if (!fileStr.empty() && has_suffix(fileStr, xorstr(".dfsoft")))

	{

		xCodeTMP = string_format(xorstr("Misc\\river\\lord_%d.dxt"), _PID);
		PBYTE pbBuffer = NULL;
		HANDLE hSourceFile = CreateFileA(fileStr.c_str(), FILE_READ_DATA, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		HANDLE hDestinationFile = CreateFileA(xCodeTMP.c_str(), FILE_WRITE_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE != hSourceFile && INVALID_HANDLE_VALUE != hDestinationFile)
		{
			bool fEOF = false;
			DWORD dwCount;
			LARGE_INTEGER size;
			GetFileSizeEx(hSourceFile, &size);
			LONG len = (size.u.HighPart << (LONG)32) | size.u.LowPart;
			DWORD dwBlockLen = len % 2 == 0 ? 32 : 31;
			DWORD dwBufferLen = dwBlockLen;
			if ((pbBuffer = (PBYTE)malloc(dwBufferLen)))
			{
				if (ReadFile(hSourceFile, pbBuffer, 4, &dwCount, NULL))
				{
					WriteFile(hDestinationFile, pbBuffer, dwCount, &dwCount, NULL);
				}
				ZeroMemory(pbBuffer, dwBufferLen);
				dwCount = 0;
				do
				{
					if (ReadFile(hSourceFile, pbBuffer, dwBlockLen, &dwCount, NULL))
					{
						if (dwCount < dwBlockLen)
						{
							fEOF = TRUE;
						}
						if (CryptDecrypt(hKey, 0, 1, 0, pbBuffer, &dwCount))
						{
							WriteFile(hDestinationFile, pbBuffer, dwCount, &dwCount, NULL);
						}
					}
				} while (!fEOF);
			}
		}
		if (pbBuffer)
		{
			free(pbBuffer);
		}
		if (hSourceFile)
		{
			CloseHandle(hSourceFile);
		}
		if (hDestinationFile)
		{
			CloseHandle(hDestinationFile);
		}
	}
}

void __declspec(naked) hkLoad()
{
	__asm {
		mov edx, [esp + 4]
		mov fileStr, edx
		pushad
		pushfd
		call uifEncrpytOperation
		popfd
		popad
		mov edx, offset xCodeTMP
		mov[esp + 4], edx
		jmp Func_Load
	}
}

void __fastcall RemoveTMP()
{
	std::string file = string_format(xorstr("Misc\\river\\lord_%d.dxt"), _PID);
	std::remove(file.c_str());
}

void __declspec(naked) hkEndOfLoad()
{
	__asm {
		pushad
		pushfd
	}

	__asm {
		popfd
		popad
		call KO_UIF_FILE_LOAD_ORG_CALL
		pushad
		pushfd
		call RemoveTMP
		popfd
		popad
		jmp KO_UIF_FILE_LOAD_RET
	}
}

void LoadCrypto()
{
	_PID = GetCurrentProcessId();
	if (CryptAcquireContext(&hCryptProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, 0))
	{
		if (CryptCreateHash(hCryptProv, 0x8004, 0, 0, &hHash))
		{
			//if (CryptHashData(hHash, (BYTE*)xorstr("pe0arl1g!uard!xfeknr69b%!@%44"), 29, CRYPT_USERDATA))  // Uif cryptosu buradan değişiyor. Sonu böyle olmak zorunda "%!@%51"
			//if (CryptHashData(hHash, (BYTE*)xorstr("Msoft2021!!&%Fil0z0f2021rd!xfeknr69b%!@%44"), 29, CRYPT_USERDATA))  // Uif cryptosu buradan değişiyor. Sonu böyle olmak zorunda "%!@%51"
			if (CryptHashData(hHash, (BYTE*)xorstr("(A;dq1DPVFgVs1Aez$VS3R0hge@NvM_TJvblD44.af1h@r5bUzp"), 29, CRYPT_USERDATA))  // Uif cryptosu buradan değişiyor. Sonu böyle olmak zorunda "%!@%51"

			{
				if (!CryptDeriveKey(hCryptProv, 0x6801, hHash, 0x00800000, &hKey))
				{
					TerminateProcess(GetCurrentProcess(), 0);
				}
			}
		}
	}

	Func_Load = (DWORD)DetourFunction((PBYTE)KO_UIF_FILE_LOAD, (PBYTE)hkLoad);
	DetourFunction((PBYTE)(KO_UIF_FILE_LOAD + 0x56), (PBYTE)hkEndOfLoad);
}
//----------------------------------------------