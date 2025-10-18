#pragma once
#include "stdafx.h"
#include "Pearl Engine.h"
// IPLISANS
#if XTREME_LISANS == 1


static string serverAdlari[] = { xorstr("SunucuName") };
// gün, ay, yýl
static uint32 lisansTarih[] = { 01, 01, 2040 };
// iki lisans þekli de ayný anda çalýþýr
//static string ipLisanslari[] = { xorstr("5.9.173.118"), xorstr("185.160.30.104"), xorstr("193.169.250.183") };
static std::string ipLisanslari[] = {xorstr("127.0.0.1")};
// x den öncesine bakar
static string subnetLisanlar[] = { xorstr("127.0.0.xx") };  //999 olan yere ip eklenecek son noktadan sonra xx koyulmalý. Örneðin : 127.0.0.xx

extern DWORD WINAPI LicenseEngine(PearlEngine* e);
extern bool IsLicensed(std::string ip);
#endif