#include "stdafx.h"
#include "UIAnvil.h"

CUIAnvil::CUIAnvil()
{
	vector<int>offsets;
	offsets.push_back(0x2A8);
	offsets.push_back(0);

	m_dVTableAddr = Engine->rdword(KO_DLG, offsets);
	// aakinci ekleme 30.07.2022
	UpgradeString = 0;
	UpgradeString2 = 0;
	m_txtreqmoney = 0;
	m_txtUprate = 0;
	m_areaUpgrade = 0;
	m_btncancel = 0;
	m_btnclose = 0;
	memset(m_areaSlot, 0, sizeof(m_areaSlot));

	ParseUIElements();
	InitReceiveMessage();
}

CUIAnvil::~CUIAnvil()
{
}

void CUIAnvil::ParseUIElements()
{
	std::string find = xorstr("txt_uprate");
	Engine->GetChildByID(m_dVTableAddr, find, m_txtUprate);
	find = xorstr("txt_reqmoney");
	Engine->GetChildByID(m_dVTableAddr, find, m_txtreqmoney);
	find = xorstr("a_upgrade");
	Engine->GetChildByID(m_dVTableAddr, find, m_areaUpgrade);
	find = xorstr("btn_cancel");
	Engine->GetChildByID(m_dVTableAddr, find, m_btncancel);
	find = xorstr("btn_close");
	Engine->GetChildByID(m_dVTableAddr, find, m_btnclose);

	find = xorstr("a_m_%d");
	for (int i = 0; i < 9; i++)
		Engine->GetChildByID(m_dVTableAddr, string_format(find, i), m_areaSlot[i]);
}

void CUIAnvil::SetResult(uint32 oran1, uint32 oran2)
{
	UpgradeString = oran1;
	UpgradeString2 = oran2;

	float per = (float)((float)UpgradeString / (float)100) > 100 ? 100 : (float)((float)UpgradeString / (float)100);
	Engine->SetString(m_txtUprate, string_format(xorstr("%.2lf%%"), per));

	Engine->SetString(m_txtreqmoney, std::to_string(oran2));
}

void CUIAnvil::Tick()
{
	if (!Engine->IsVisible(m_dVTableAddr) && UpgradeString != 0)
		SetResult(0, 0);
	// anvil ile oto up ayný anda açýlmasýn
	if (Engine->IsVisible(m_dVTableAddr) && Engine->IsVisible(Engine->uiDropResult->autoupgrade))
		Engine->uiDropResult->CloseUp();
	// anvil ile oto up ayný anda açýlmasýn end
}

DWORD uiAnvilVTable;
DWORD Func_Anvil;
bool CUIAnvil::ReceiveMessage(DWORD* pSender, uint32_t dwMsg)
{
	uiAnvilVTable = m_dVTableAddr;

	if (!pSender || pSender == 0 || dwMsg != UIMSG_BUTTON_CLICK)
		return false;

	if (pSender == (DWORD*)m_btncancel || pSender == (DWORD*)m_btnclose)
		SetResult(0, 0);

	return true;
}

void __stdcall UiAnvilReceiveMessage_Hook(DWORD* pSender, uint32_t dwMsg)
{
	Engine->uiAnvil->ReceiveMessage(pSender, dwMsg);
	__asm
	{
		MOV ECX, uiAnvilVTable
		PUSH dwMsg
		PUSH pSender
		MOV EAX, Func_Anvil
		CALL EAX
	}
}

void CUIAnvil::InitReceiveMessage()
{
	DWORD ptrMsg = Engine->GetRecvMessagePtr(m_dVTableAddr);
	Func_Anvil = *(DWORD*)ptrMsg;
	*(DWORD*)ptrMsg = (DWORD)UiAnvilReceiveMessage_Hook;
}
