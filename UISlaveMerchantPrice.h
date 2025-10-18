#pragma once

class CSlaveMerchantPrice;

#include "stdafx.h"
#include "Pearl Engine.h"


class CSlaveMerchantPrice : public CN3UIBase
{
	CN3UIButton* btn_coin2;


	CN3UIBase * grp_takecoin;
	CN3UIButton* btn_ok, * btn_cancel2;
	CN3UIString* text_price;
	CN3UIEdit* edit_price;

	uint8 result;


public:
	CSlaveMerchantPrice();
	~CSlaveMerchantPrice();
	uint32 KC, COINS;
	

	bool m_bIsCoin = false;
	bool Coin = false;
	bool kc = false;
	bool Load(HANDLE hFile);
	bool ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg);
	uint32_t MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld);
	bool OnKeyPress(int iKey);
	void Close();
	void Open();
	void GetCoinCash();
	void Update(Packet pkt);
	void Tick();
};