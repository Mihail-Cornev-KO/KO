#pragma once
class UISlavePriestSub;
#include "stdafx.h"
#include "Pearl Engine.h"


class UISlavePriestSub : public CN3UIBase
{
	CN3UIButton* btn_priest_open;
	CN3UIButton* btn_priest_stop;
	CN3UIButton* btn_priest_start;
	

public:
	UISlavePriestSub();
	~UISlavePriestSub();
	bool Load(HANDLE hFile);
	bool ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg);
	uint32_t MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld);
	bool OnKeyPress(int iKey);
	void Close();
	void Open();
};