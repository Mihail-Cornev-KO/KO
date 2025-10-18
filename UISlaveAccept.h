#pragma once

class CSlaveAccept;

#include "stdafx.h"
#include "Pearl Engine.h"

class CSlaveAccept : public CN3UIBase
{
	CN3UIButton* btn_ok, * btn_cancel, * btn_slave;
	bool m_bIsCheck = false;
public:
	CSlaveAccept();
	~CSlaveAccept();
	bool Load(HANDLE hFile);
	bool ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg);
	uint32_t MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld);
	bool OnKeyPress(int iKey);
	void Close();
	void Open();
	void Tick();
};