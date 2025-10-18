#pragma once
class CVsKabul;
#include "stdafx.h"
#include "Pearl Engine.h"


class CVsKabul : public CN3UIBase
{
	CN3UIButton* btn_ok, * btn_cancel;
	CN3UIString* text_notice;
	CN3UIBase* grp_accept;
public:
	CVsKabul();
	~CVsKabul();
	uint8 kills;
	bool Load(HANDLE hFile);
	bool ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg);
	uint32_t MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld);
	bool OnKeyPress(int iKey);
	void Close();
	void Open();
	void Update(Packet pkt);
	void Tick();
};
#pragma once
