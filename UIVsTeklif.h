#pragma once
class CVsTeklif;
#include "stdafx.h"
#include "Pearl Engine.h"

class CVsTeklif : public CN3UIBase
{
	CN3UIButton* btn_eksi, * btn_close, * btn_arti, * btn_1v1;
	CN3UIEdit* edit_cashvs;
	CN3UIString* edit_str2, * txt_kill;
public:
	CVsTeklif();
	~CVsTeklif();
	uint8 kills;
	bool Load(HANDLE hFile);
	bool ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg);
	uint32_t MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld);
	bool OnKeyPress(int iKey);
	void Close();
	void Open();
	void Update();
	void Tick();
};
#pragma once
#pragma once
