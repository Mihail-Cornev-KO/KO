#pragma once
#include "stdafx.h"
class CUITargetBarPlug;
#include "Pearl Engine.h"
#include "CSpell.h"//target sc new end
#include "HDRReader.h"//target sc new end

class CUITargetBarPlug
{
public:
	CUITargetBarPlug();
	~CUITargetBarPlug();

	void ParseUIElements();

	DWORD str_name;

	DWORD m_dVTableAddr;

	DWORD icon_skill1_[20];
	DWORD skill_group;
	DWORD m_btnDrop;
	DWORD m_user_info;
	DWORD m_dTextTargetHp;

	void SetTargetHp(Packet& pkt);
	void TargetNameSet(uint16 GetID);
	bool ReceiveMessage(DWORD* pSender, uint32_t dwMsg);
	void InitReceiveMessage();

	void Update(Packet& pkt);
	void SetInfo(DWORD color, uint8 lvl, std::string name);

private:

};