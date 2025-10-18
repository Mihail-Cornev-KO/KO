#pragma once
class CRebith;
#include "stdafx.h"
#include "Pearl Engine.h"


class CRebith : public CN3UIBase
{
	CN3UIBase* base_stat,* use_helps,* use_confirm,* use_rebirth;
	CN3UIButton* btn_save;
	CN3UIButton* btn_statclose,* btn_helps,* btn_use_rebirth,* btn_close,* rebirth_confirm,* btn_confirm,* btn_canceled;
	CN3UIButton* btn_up[5];
	CN3UIButton* btn_down[5];
	CN3UIString* str[5];
	CN3UIString* str_point;
	uint8 m_RebPoint;
	uint8 stats[5];

public:
	CRebith();
	~CRebith();
	bool Load(HANDLE hFile);
	bool ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg);
	uint32_t MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld);
	bool OnKeyPress(int iKey);
	void Close();
	void Open();
	void Update(Packet pkt);
};