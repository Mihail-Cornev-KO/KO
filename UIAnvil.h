#pragma once
class CUIAnvil;
#include "stdafx.h"
#include "Pearl Engine.h"
class CUIAnvil
{
public:
	CUIAnvil();
	~CUIAnvil();

	void ParseUIElements();
	void Tick();
	bool ReceiveMessage(DWORD* pSender, uint32_t dwMsg);
	void InitReceiveMessage();

	uint32 UpgradeString;
	uint32 UpgradeString2;
	DWORD m_dVTableAddr;
	DWORD m_txtUprate;
	DWORD m_txtreqmoney;
	DWORD m_areaUpgrade, m_btncancel, m_btnclose;
	DWORD m_areaSlot[9];

	void SetResult(uint32 oran1, uint32 oran2);
};