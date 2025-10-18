#pragma once

class CSlaveMerchant;

#include "stdafx.h"
#include "Pearl Engine.h"


class CSlaveMerchant : public CN3UIBase
{
	CN3UIBase* grp_merchant, * grp_inventory;
	CN3UIButton* btn_coin2;
	CN3UIArea* area;
	CN3UIButton* btn_kc, * btn_close, * btn_coin, * btn_cancel, * btn_confrim, * btn_useslavescroll;
	CN3UIString* txt_sure, * txt_durum, * text_cash, * text_money, * c[12], * d[28];
	CN3UIEdit* edit_price;
	//CTimer* m_Timer;
	//uint16 eventt;
	CTimer* m_Timer;
	RECT minimumRect, maximizeRect, rewardsRect;

	vector<SlaveItem> m_Data;

	vector<InventoryItem> m_Data2;

	struct ItemInfo
	{
		uint32 nItemID;
		CN3UIIcon* icon;
		TABLE_ITEM_BASIC* tbl;
		ItemInfo()
		{
			nItemID = 0;
			icon = NULL;
			tbl = nullptr;
		}
	};

	vector<ItemInfo*> itemsMerch;

	vector<ItemInfo*> itemsInventory;
	uint32 KC, COINS;
	uint8 TotalTime2;
	uint32 TotalTime;
public:
	CSlaveMerchant();
	~CSlaveMerchant();
	uint32 m_iRemainingTime;
	bool m_bIsCoin = false;
	bool Coin = false;
	bool kc = false;
	bool Load(HANDLE hFile);
	bool ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg);
	uint32_t MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld);
	bool OnKeyPress(int iKey);
	void Close();
	void Open();
	void Update(Packet pkt, vector<SlaveItem> drops, vector<InventoryItem> drops2);
	void UpdateTimer(Packet pkt);
	void Tick();
	void Clear();
	__IconItemSkill* GetItemStruct(CN3UIImage* img, std::string fileSz, __TABLE_ITEM_BASIC* tbl, uint32 realID);
};