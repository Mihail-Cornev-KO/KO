#pragma once
class CUIMiniMenuPlug;
#include "stdafx.h"
#include "Pearl Engine.h"

class CUIMiniMenuPlug
{
public:
	DWORD m_dVTableAddr;
	DWORD base_default;
	DWORD m_btngiftpus;
	DWORD m_btnViewInfo;
	DWORD m_btnVsTeklif;

public:
	CUIMiniMenuPlug();
	~CUIMiniMenuPlug();
	void ParseUIElements();
	bool ReceiveMessage(DWORD* pSender, uint32_t dwMsg);
	void InitReceiveMessage();

private:

};