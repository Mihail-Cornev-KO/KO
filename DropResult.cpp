#include "DropResult.h"


CDropResult::CDropResult()
{
	vector<int>offsets;
	offsets.push_back(0x464);  // co_movie_save Satýr : 222
	//offsets.push_back(0x518);  //re_caht_main Satýr : 217 // orijinal
	offsets.push_back(0);

	m_dVTableAddr = Engine->rdword(KO_DLG, offsets);

	btn_fast = NULL;

	fastestcount = NULL;
	// upgrade system
	autoupissucces = 0;
	autoupgrade = NULL;
	itemanime = NULL;
	yananime = NULL;

	//for (int i = 0; i < 11; i++) {
	//	s[i] = NULL;
	//	u[i] = NULL;
	//}

	for (int i = 0; i < 4; i++) {
		sc[i] = NULL;
	}

	upitemss.iconus = NULL;
	upitemss.nItemIDus = 0;
	upitemss.tblus = nullptr;

	upitems.iconu = NULL;
	upitems.nItemIDu = 0;
	upitems.tblu = nullptr;
	btn_exit = NULL;
	upitemsico = NULL;
	btn_start = NULL;
	btn_stop = NULL;
	totalcoast = NULL;
	totalfail = NULL;
	totalup = NULL;
	totalsuccess = NULL;
	img_succeed2 = NULL;
	img_burn = NULL;
	autoupCount = 0;
	autoupcoins = 0;
	autoupFailCount = 0;
	autoupSuccessCount = 0;
	autoupfastCount = 0;
	autoupScrollID = 0;

	txt_title = NULL;
	btn_close = NULL;
	for (int i = 0; i < 5; i++) {
		dropitems[i].icon = NULL;
		dropitems[i].nItemID = 0;
		dropitems[i].tbl = nullptr;
	}
	ParseUIElements();
	InitReceiveMessage();
}

CDropResult::~CDropResult()
{

}

void CDropResult::ParseUIElements()
{

	autoupgrade = Engine->GetChildByID(m_dVTableAddr, xorstr("autoupgrade"));
	yananime = Engine->GetChildByID(autoupgrade, xorstr("yananime"));
	itemanime = Engine->GetChildByID(autoupgrade, xorstr("itemanime"));

	btn_fast = Engine->GetChildByID(autoupgrade, "btn_fast");
	fastestcount = Engine->GetChildByID(autoupgrade, "fastestcount");

	img_burn = Engine->GetChildByID(autoupgrade, "img_burn");
	img_succeed2 = Engine->GetChildByID(autoupgrade, "img_succeed2");
	upitemss.iconus = Engine->GetChildByID(autoupgrade, "img_succeed");
	//totals
	totalsuccess = Engine->GetChildByID(autoupgrade, "totalsuccess");
	totalup = Engine->GetChildByID(autoupgrade, "totalup");
	totalfail = Engine->GetChildByID(autoupgrade, "totalfail");
	totalcoast = Engine->GetChildByID(autoupgrade, "totalcoast");
	//buttons
	btn_stop = Engine->GetChildByID(autoupgrade, "btn_stop");
	btn_start = Engine->GetChildByID(autoupgrade, "btn_start");
	btn_exit = Engine->GetChildByID(autoupgrade, "btn_exit");
	for (int i = 1; i <= 4; i++)
	{
		sc[i - 1] = Engine->GetChildByID(autoupgrade, string_format(xorstr("sc%d"), i));
		Engine->SetState(sc[i - 1], UI_STATE_BUTTON_NORMAL);
		//autoupScrollID[i - 1] = i;
	}
	upitems.iconu = Engine->GetChildByID(autoupgrade, xorstr("img_upitem"));
	Engine->SetVisible(img_burn, false);
	Engine->SetVisible(img_succeed2, false);
	Engine->SetVisible(autoupgrade, false);

	dresult = Engine->GetChildByID(m_dVTableAddr, xorstr("dresult"));
	txt_title = Engine->GetChildByID(dresult, "txt_title");
	btn_close = Engine->GetChildByID(dresult, "btn_close");

	for (int i = 1; i <= 5; i++)
	{
		dropitems[i - 1].icon = Engine->GetChildByID(dresult, string_format(xorstr("itemicon%d"), i));
	}
	//txt_title = Engine->GetChildByID(m_dVTableAddr, "txt_title");
	//btn_close = Engine->GetChildByID(m_dVTableAddr, "btn_close");

	//for (int i = 1; i <= 5; i++)
	//{
	//	dropitems[i-1].icon = Engine->GetChildByID(m_dVTableAddr, string_format(xorstr("itemicon%d"), i));
	//}
	//Engine->UIScreenCenter(m_dVTableAddr);
}

DWORD uiBassse;
DWORD Func_Drop = 0;
bool CDropResult::ReceiveMessage(DWORD* pSender, uint32_t dwMsg)
{
	uiBassse = m_dVTableAddr;
	if (!pSender || pSender == 0 || dwMsg != UIMSG_BUTTON_CLICK)
		return false;

	//std::string msg = "";
	//if (pSender == (DWORD*)btn_close)
	//	Close();
	if (Engine->IsVisible(dresult))
	{
		if (pSender == (DWORD*)btn_close)
			Close();
	}
	else if (Engine->IsVisible(autoupgrade))
	{
		//btn_fast, fastestcount
		if (pSender == (DWORD*)btn_exit)
			CloseUp();
		else if (pSender == (DWORD*)btn_fast)
		{
			if (Engine->GetState(btn_fast) == UI_STATE_BUTTON_DOWN)
			{
				autoupisFast = 1;
			}
			else
				autoupisFast = 0;

			//printf("--------------->autoupisFast: %d \n", autoupisFast);
		}
		else if (pSender == (DWORD*)btn_start)
		{
			if (Engine->m_PlayerBase->m_iZoneID != 21)
			{
				Engine->WriteInfoMessageExt((char*)string_format(xorstr("You must go to the Moradon area.")).c_str(), 0xBA0F30);
				return true;
			}
			else if (autoupScrollID == 0)
			{
				Engine->WriteInfoMessageExt((char*)string_format(xorstr("You must select any scroll type.")).c_str(), 0xBA0F30);
				return true;
			}
			else if (autoupScrollID > 0)
			{
				//Engine->WriteInfoMessageExt((char*)string_format(xorstr("You must select any scroll type.")).c_str(), 0xBA0F30);
				//379016000 4 up5sc
				//379021000 3 bus
				//379205000 2 mid
				//379221000 1 low
				if (autoupScrollID == 1 && !Engine->HasItem(LOW_SC))
					Engine->WriteInfoMessageExt((char*)string_format(xorstr("You do not have Upgrade Scroll (Low Class item) item.")).c_str(), 0xBA0F30);
				else if (autoupScrollID == 2 && !Engine->HasItem(MID_SC))
					Engine->WriteInfoMessageExt((char*)string_format(xorstr("You do not have Upgrade Scroll (Middle Class item) item.")).c_str(), 0xBA0F30);
				else if (autoupScrollID == 3 && !Engine->HasItem(BUS_SC))
					Engine->WriteInfoMessageExt((char*)string_format(xorstr("You do not have Blessed Upgrade Scroll item.")).c_str(), 0xBA0F30);
				else if (autoupScrollID == 4 && !Engine->HasItem(UP_SC))
					Engine->WriteInfoMessageExt((char*)string_format(xorstr("You do not have Upgrade Scroll item.")).c_str(), 0xBA0F30);
				else
				{
					Engine->SetVisible(Engine->uiDropResult->yananime, true);
					Engine->SetVisible(Engine->uiDropResult->itemanime, true);

					Packet pkt(XSafe, uint8(XSafeOpCodes::RPGUARD_AUTO_UPGRADE));
					pkt << uint8(1) << uint8(autoupScrollID) << uint8(autoupisFast);
					Engine->Send(&pkt);
				}
			}


			//Packet pkt(XSafe, uint8(XSafeOpCodes::RPGUARD_AUTO_UPGRADE));
			//pkt << uint8(1) << uint8(autoupScrollID) << uint8(autoupisFast);
			//Engine->Send(&pkt);
		}
		else if (pSender == (DWORD*)btn_stop)
		{
			Engine->SetVisible(Engine->uiDropResult->yananime, false);
			Engine->SetVisible(Engine->uiDropResult->itemanime, false);

			Engine->SetVisible(img_burn, false);
			Engine->SetVisible(img_succeed2, false);


			Engine->SetTexImage(upitems.iconu, "ui\\itemicon_noimage.dxt");
			Engine->SetVisible(Engine->uiDropResult->upitemss.iconus, false);
			//Engine->SetTexImage(upitemss.iconus, "ui\\itemicon_noimage.dxt");
			upitems.nItemIDu = 0;
			upitems.tblu = nullptr;
			upitemss.nItemIDus = 0;
			upitemss.tblus = nullptr;


			autoupScrollID = 0;
			autoupisFast = 0;
			for (int i = 0; i < 4; i++)
				Engine->SetState(Engine->uiDropResult->sc[i], UI_STATE_BUTTON_NORMAL);

			Engine->SetState(btn_exit, UI_STATE_BUTTON_NORMAL);
			Engine->SetState(btn_fast, UI_STATE_BUTTON_NORMAL);
			Engine->SetState(btn_start, UI_STATE_BUTTON_NORMAL);
			Engine->m_UpgradeisStoped = true;
			Engine->m_UpgradeisStarted = false;
			Engine->WriteInfoMessageExt((char*)string_format(xorstr("Auto Upgrade System has been stopped.")).c_str(), 0xBA0F30);

			//Packet pkt(XSafe, uint8(XSafeOpCodes::RPGUARD_AUTO_UPGRADE));
			//pkt << uint8(0) << uint8(0);
			//Engine->Send(&pkt);
		}
		else {
			for (uint8 i = 0; i < 4; i++)
			{
				if ((pSender == (DWORD*)sc[i] && !Engine->m_UpgradeisStarted) || (pSender == (DWORD*)sc[i] && Engine->m_UpgradeisStoped))
				{
					for (uint8 t = 0; t < 4; t++)
					{
						if (i == t)
						{
							autoupScrollID = i + 1;
							////printf("CUIAccountRegister::ReceiveMessage SelectedNum: %d || ExchangeItemNum: %d \n", SelectedNum, ExchangeItemNum);
							//if (Engine->GetState(sc[t]) == UI_STATE_BUTTON_DOWN)
							//	Engine->SetState(sc[t], UI_STATE_BUTTON_NORMAL);
							//else
							//	Engine->SetState(sc[t], UI_STATE_BUTTON_DOWN);
							Engine->SetState(sc[t], UI_STATE_BUTTON_DOWN);
						}
						else
							Engine->SetState(sc[t], UI_STATE_BUTTON_NORMAL);

					}
				}
			}
		}
	}
	return true;
}


void __stdcall UiDropMainReceiveMessage_Hook(DWORD* pSender, uint32_t dwMsg)
{
	Engine->uiDropResult->ReceiveMessage(pSender, dwMsg);
	__asm
	{
		MOV ECX, uiBassse
		PUSH dwMsg
		PUSH pSender
		MOV EAX, Func_Drop
		CALL EAX
	}
}

void CDropResult::InitReceiveMessage()
{
	DWORD ptrMsg = Engine->GetRecvMessagePtr(m_dVTableAddr);
	Func_Drop = *(DWORD*)ptrMsg;
	*(DWORD*)ptrMsg = (DWORD)UiDropMainReceiveMessage_Hook;
}

uint32_t CDropResult::MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld)
{
	uint32_t dwRet = UI_MOUSEPROC_NONE;
	{
		bool pass = false;

		if (!Engine->IsVisible(autoupgrade)
			&& !Engine->IsVisible(dresult))
			pass = true;

		if (!pass)
		{
			if (Engine->IsVisible(dresult))
			{
				for (auto it : dropitems)
				{
					if (Engine->IsVisible(autoupgrade) || !Engine->IsVisible(dresult))
						break;

					if (it.icon != NULL && it.nItemID > 0)
					{
						if (Engine->IsIn(it.icon, ptCur.x, ptCur.y))
						{
							Engine->ShowToolTipEx(it.nItemID, ptCur.x, ptCur.y);
							break;
						}
					}
				}
			}
			else if (upitems.iconu != NULL && upitems.nItemIDu > 0 && Engine->IsVisible(autoupgrade) && !Engine->IsVisible(dresult))
			{
				if (Engine->IsIn(upitems.iconu, ptCur.x, ptCur.y))
				{
					Engine->ShowToolTipEx(upitems.nItemIDu, ptCur.x, ptCur.y);
					//break;
				}
			}

			else if (upitemss.iconus != NULL && upitemss.nItemIDus > 0 && Engine->IsVisible(autoupgrade) && !Engine->IsVisible(dresult))
			{
				if (Engine->IsIn(upitemss.iconus, ptCur.x, ptCur.y))
				{
					Engine->ShowToolTipEx(upitemss.nItemIDus, ptCur.x, ptCur.y);
					//break;
				}
			}
		}
	}

	return dwRet;
	/*uint32_t dwRet = UI_MOUSEPROC_NONE;
	bool showTooltip = false;

	for (int i = 0; i < 5; i++)
	{
		if (dropitems[i].icon != NULL) {
			if (Engine->IsIn(dropitems[i].icon, ptCur.x, ptCur.y) && dropitems[i].tbl != nullptr && dropitems[i].nItemID != 0)
			{
				Engine->ShowToolTipEx(dropitems[i].nItemID, ptCur.x, ptCur.y);
				break;
			}
		}
	}

	return dwRet;*/
}

void CDropResult::Open()
{
	Engine->SetVisible(autoupgrade, false);
	Engine->SetVisible(dresult, true);
	Engine->SetVisible(m_dVTableAddr, true);
}

void CDropResult::Close()
{
	Engine->SetVisible(dresult, false);
	Engine->SetVisible(m_dVTableAddr,false);
}

void CDropResult::OpenUp()
{
	//for (auto& it : dropitems)
	//{
	//	it.icon = NULL;
	//	it.nItemID = NULL;
	//	it.tbl = nullptr;
	//}
	//for (int i = 0; i < 5; i++) {
	//	dropitems[i].icon = NULL;
	//	dropitems[i].nItemID = 0;
	//	dropitems[i].tbl = nullptr;
	//}
	//upitems.iconu = NULL;
	if (Engine->m_PlayerBase->m_iZoneID != 21)
	{
		Engine->WriteInfoMessageExt((char*)string_format(xorstr("You must go to the Moradon area.")).c_str(), 0xBA0F30);
		return;
	}

	// anvil ile oto up ayný anda açýlmasýn
	if (Engine->uiAnvil && Engine->IsVisible(Engine->uiAnvil->m_dVTableAddr))
	{
		Engine->WriteInfoMessageExt((char*)string_format(xorstr("You cannot use this system while regular upgrading.")).c_str(), 0xBA0F30);
		return;
	}
	// anvil ile oto up ayný anda açýlmasýn end

	Engine->m_UpgradeisStoped = false;

	Engine->SetTexImage(upitems.iconu, "ui\\itemicon_noimage.dxt");
	Engine->SetVisible(Engine->uiDropResult->upitemss.iconus, false);
	//Engine->SetTexImage(upitemss.iconus, "ui\\itemicon_noimage.dxt");
	upitemss.nItemIDus = 0;
	upitemss.tblus = nullptr;
	upitems.nItemIDu = 0;
	upitems.tblu = nullptr;

	autoupissucces = 0;

	Engine->SetVisible(Engine->uiDropResult->yananime, false);
	Engine->SetVisible(Engine->uiDropResult->itemanime, false);
	Engine->SetVisible(img_burn, false);
	Engine->SetVisible(img_succeed2, false);

	autoupisFast = 0;
	Engine->SetState(btn_fast, UI_STATE_BUTTON_NORMAL);

	Engine->uiDropResult->autoupCount = 0;

	Engine->uiDropResult->autoupFailCount = 0;
	Engine->uiDropResult->autoupcoins = 0;
	Engine->uiDropResult->autoupSuccessCount = 0;


	std::string cashx = Engine->StringHelper->NumberFormat(Engine->uiDropResult->autoupfastCount);
	std::string strkc = xorstr("Click for Fast Upgrade, but you lost [%sx] as much money.");
	if (Engine->uiDropResult->autoupfastCount == 0)
		Engine->SetString(Engine->uiDropResult->fastestcount, xorstr("Click for Fast Upgrade."));
	else
		Engine->SetString(Engine->uiDropResult->fastestcount, string_format(strkc, cashx.c_str()));

	Engine->SetString(Engine->uiDropResult->totalup, Engine->StringHelper->NumberFormat(Engine->uiDropResult->autoupCount).c_str());
	Engine->SetString(Engine->uiDropResult->totalfail, Engine->StringHelper->NumberFormat(Engine->uiDropResult->autoupFailCount).c_str());
	Engine->SetString(Engine->uiDropResult->totalcoast, Engine->StringHelper->NumberFormat(Engine->uiDropResult->autoupcoins).c_str());
	Engine->SetString(Engine->uiDropResult->totalsuccess, Engine->StringHelper->NumberFormat(Engine->uiDropResult->autoupSuccessCount).c_str());

	for (int i = 0; i < 4; i++)
		Engine->SetState(sc[i], UI_STATE_BUTTON_NORMAL);


	Engine->SetState(btn_stop, UI_STATE_BUTTON_DISABLE);
	Engine->SetState(btn_start, UI_STATE_BUTTON_NORMAL);

	Engine->SetString(totalcoast, xorstr("0"));
	Engine->SetString(totalfail, xorstr("0"));
	Engine->SetString(totalup, xorstr("0"));
	Engine->SetString(totalsuccess, xorstr("0"));


	Engine->SetVisible(dresult, false);
	Engine->SetVisible(autoupgrade, true);
	Engine->SetVisible(m_dVTableAddr, true);
}

void CDropResult::CloseUp()
{
	Engine->m_UpgradeisStoped = true;
	Engine->SetVisible(autoupgrade, false);
	//Engine->SetVisible(dresult, false);
	Engine->SetVisible(m_dVTableAddr, false);

	Engine->SetVisible(Engine->uiDropResult->yananime, false);
	Engine->SetVisible(Engine->uiDropResult->itemanime, false);

	autoupissucces = 0;
	autoupisFast = 0;
	Engine->SetState(btn_fast, UI_STATE_BUTTON_NORMAL);

	Engine->SetTexImage(upitems.iconu, "ui\\itemicon_noimage.dxt");
	Engine->SetVisible(Engine->uiDropResult->upitemss.iconus, false);
	//Engine->SetTexImage(upitemss.iconus, "ui\\itemicon_noimage.dxt");
	upitems.nItemIDu = 0;
	upitems.tblu = nullptr;
	upitemss.nItemIDus = 0;
	upitemss.tblus = nullptr;

	Engine->SetVisible(img_burn, false);
	Engine->SetVisible(img_succeed2, false);

	Engine->SetString(totalcoast, xorstr("0"));
	Engine->SetString(totalfail, xorstr("0"));
	Engine->SetString(totalup, xorstr("0"));
	Engine->SetString(totalsuccess, xorstr("0"));
	//Engine->SetString(totalsuccess, string_format(xorstr("%d"), p.mydead_count));
	for (int i = 0; i < 4; i++)
		Engine->SetState(sc[i], UI_STATE_BUTTON_NORMAL);

	//upitems.iconu = NULL;
}

void CDropResult::SetTitle(std::string text)
{
	Engine->SetString(txt_title,text);
}

void CDropResult::AddItem(uint32 itemID)
{
	TABLE_ITEM_BASIC* tbl = Engine->tblMgr->getItemData(itemID);
	if (tbl == nullptr)
		return;

	auto pItemExt = Engine->tblMgr->getExtData(tbl->extNum, itemID);

	std::string szIconFN = "";

	if (TABLE_ITEM_EXT* ext = Engine->tblMgr->getExtData(tbl->extNum, itemID))
	{
		DWORD dwIconID = tbl->dxtID;
		if (ext->dxtID > 0)
			dwIconID = ext->dxtID;

		szIconFN = string_format(xorstr("UI\\ItemIcon_%.1d_%.4d_%.2d_%.1d.dxt"), dwIconID / 10000000u, dwIconID / 1000u % 10000, dwIconID / 10u % 100, dwIconID % 10);
	}
	else {
		std::vector<char> buffer(256, NULL);

		sprintf(&buffer[0], "ui\\itemicon_%.1d_%.4d_%.2d_%.1d.dxt",
			(tbl->dxtID / 10000000),
			(tbl->dxtID / 1000) % 10000,
			(tbl->dxtID / 10) % 100,
			tbl->dxtID % 10);

		szIconFN = &buffer[0];
	}

	for (int i = 0; i < 5; i++)
	{
		if (dropitems[i].nItemID == 0)
		{
			dropitems[i].nItemID = itemID;
			dropitems[i].tbl = tbl;
			Engine->SetTexImage(dropitems[i].icon,szIconFN);
			dropitems[i].fileName = szIconFN.c_str();
			return;
		}
	}

	for (int i = 4; i >= 1; i--)
	{
		dropitems[i].nItemID = dropitems[i - 1].nItemID;
		Engine->SetTexImage(dropitems[i].icon,dropitems[i - 1].fileName);
		dropitems[i].fileName = dropitems[i - 1].fileName;
		dropitems[i].tbl = dropitems[i - 1].tbl;
	}

	dropitems[0].nItemID = itemID;
	dropitems[0].tbl = tbl;
	Engine->SetTexImage(dropitems[0].icon,szIconFN);
	dropitems[0].fileName=szIconFN;
}

//void CDropResult::OpenDropResult()
//{
//
//}