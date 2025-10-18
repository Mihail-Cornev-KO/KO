//#pragma once
//class CDropResult;
//#include "stdafx.h"
//#include "Pearl Engine.h"
//
//class CDropResult
//{
//	struct DropItems
//	{
//		uint32 nItemID;
//		DWORD icon;
//		TABLE_ITEM_BASIC* tbl;
//		std::string fileName;
//		DropItems()
//		{
//			fileName = "";
//		}
//	};
//
//	DWORD txt_title;
//	DWORD btn_close;
//	DropItems dropitems[5];
//
//
//public:
//	DWORD m_dVTableAddr;
//	CDropResult();
//	~CDropResult();
//	void ParseUIElements();
//	bool ReceiveMessage(DWORD* pSender, uint32_t dwMsg);
//	void InitReceiveMessage();
//	uint32_t MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld);
//
//	void Close();
//	void Open();
//	void SetTitle(std::string text);
//	void AddItem(uint32 itemID);
//	void OpenDropResult();
//};
#pragma once
class CDropResult;
#include "stdafx.h"
#include "Pearl Engine.h"
#include "Timer.h"

struct AutoUpData
{
	uint8 type;
	uint32 nitemID;
	uint8 spos;
	uint32 price;
	TABLE_ITEM_BASIC* tbl;
	AutoUpData()
	{
		nitemID = 0;
		spos = 0;
		price = 0;
		type = 0;
	}
};

struct ChestSlotBase2
{
	DWORD icon, areax, strrate, spItemSkill;//24.08.2023 droplara oran ekleme strrate
	uint32 itemID;
	uint16 itemrate;//24.08.2023 droplara oran ekleme itemrate
	TABLE_ITEM_BASIC* tbl;
	ChestSlotBase2() :itemID(0), itemrate(0), tbl(NULL), icon(0), areax(0), strrate(0), spItemSkill(0) {} //24.08.2023 droplara oran ekleme itemrate strrate
};

class CDropResult
{
	struct DropItems
	{
		uint32 nItemID;
		DWORD icon;
		TABLE_ITEM_BASIC* tbl;
		std::string fileName;
		DropItems()
		{
			fileName = "";
		}
	};
	struct UpItems
	{
		uint32 nItemIDu;
		DWORD iconu;
		TABLE_ITEM_BASIC* tblu;
		std::string fileNameu;
		UpItems()
		{
			fileNameu = "";
		}
	};

	struct UpItemss
	{
		uint32 nItemIDus;
		DWORD iconus;
		TABLE_ITEM_BASIC* tblus;
		std::string fileNameus;
		UpItemss()
		{
			fileNameus = "";
		}
	};


	ChestSlotBase2* dropBase[12];

public:
	DWORD txt_title;
	DWORD btn_close;
	UpItems upitems;
	UpItemss upitemss;
	DropItems dropitems[5];
	DWORD m_dVTableAddr;
	DWORD dresult;
	DWORD itemanime;
	DWORD yananime;
	DWORD autoupgrade;
	uint8 autoupissucces;

	DWORD upitemsico;
	DWORD sc[4];
	DWORD btn_exit;
	DWORD btn_start;
	DWORD btn_stop;
	DWORD totalcoast;
	DWORD totalfail;
	DWORD totalup;
	DWORD totalsuccess;
	DWORD img_succeed2;
	DWORD img_burn;

	DWORD btn_fast;
	DWORD fastestcount;
	uint16 autoupCount;

	uint8 autoupisFast;
	uint8 autoupFailCount;
	uint32 autoupcoins;
	uint8 autoupSuccessCount;
	uint8 autoupfastCount;
	uint8 autoupScrollID;
	CDropResult();
	~CDropResult();
	void ParseUIElements();
	bool ReceiveMessage(DWORD* pSender, uint32_t dwMsg);
	void InitReceiveMessage();
	uint32_t MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld);
	CTimer* m_Timer;
	time_t remainingTime;
	uint8 slaveitemcount;
	DWORD animate[12];
	DWORD btn_close2;

	vector<AutoUpData> autoupList;
	void Close();
	void Open();
	void CloseUp();
	void OpenUp();
	void SetTitle(std::string text);
	void AddItem(uint32 itemID);
};