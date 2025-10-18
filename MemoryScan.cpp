#include "StdAfx.h"
#include "Pearl Engine.h"

void checkWriteProcessMemory() {
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);

    MEMORY_BASIC_INFORMATION mbi;
    char* address = nullptr;

    //Xtreme  Geçersiz kılmak istediğiniz bellek adresleri buraya ke
    const std::vector<void*> exemptAddresses = {
        (void*)0x4A5620, // Örneğin: G_private_key2472
        (void*)0x4A65C3, // Örneğin: G_private_key2472
        (void*)0x4DDD7F, // Örneğin: G_private_key2472
        // ve diğerleri bitmez mk
    };

    while (address < sysInfo.lpMaximumApplicationAddress) {
        if (VirtualQuery(address, &mbi, sizeof(mbi))) {
            // İzin verilen bellek bölgelerini tanımlayın
            bool isExempt = false;
            for (const auto& exemptAddress : exemptAddresses) {
                if (address == exemptAddress) {
                    isExempt = true;
                    break;
                }
            }

            // Eğer adres geçersiz kılınacaksa kontrolü atla
            if (!isExempt &&
                (mbi.Protect & PAGE_EXECUTE_READWRITE || mbi.Protect & PAGE_READWRITE) &&
                !(mbi.State == MEM_COMMIT && (mbi.Type == MEM_PRIVATE || mbi.Type == MEM_IMAGE))) {
                // Xtreme Oyun içinde gerekli olan bellek alanlarını geçersiz kılmak için kontrolü atla
                Engine->Shutdown(xorstr("Cheat Memory Edited Detected! 0x01M"));
                return;
            }
            address += mbi.RegionSize;
        }
        else {
            break;
        }
    }
}


DWORD WINAPI memory_loop(LPVOID) {
    while (true) {
        checkWriteProcessMemory();
        Sleep(2000);
    }
    return 0;
}

void memory_scan() {
    CreateThread(NULL, 0, memory_loop, NULL, 0, NULL);
}
