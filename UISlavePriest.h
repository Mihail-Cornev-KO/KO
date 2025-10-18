#pragma once
class UISlavePriest;
#include "stdafx.h"
#include "Pearl Engine.h"


class UISlavePriest : public CN3UIBase
{
	CN3UIBase* slave_priest,*recovery;
	CN3UIButton* btn_close,* DOWN,*UP;
	CN3UIString* str_priest_name,* RecoveryHealing_str;
	CN3UIButton* btn_hp_recovery_off,* btn_hp_recovery_on;
	CN3UIButton* btn_ac_recovery_off, * btn_ac_recovery_on;
	CN3UIButton* btn_healing_recovery_off, * btn_healing_recovery_on;
	CN3UIScrollBar* list_scroll;

	
public:
	bool Heal, Buff, Ac;
	uint8 sayi;
	UISlavePriest();
	~UISlavePriest();
	bool Load(HANDLE hFile);
	bool ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg);
	uint32_t MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld);
	bool OnKeyPress(int iKey);
	void change(uint8 job);
	void Close();
	void Open();
	void Tick();
};