#include "UISlaveMerchant.h"
#include "HDRReader.h"
#include "N3BASE/N3UIArea.h"

#define ITEM_GOLD					900000000	// Coins
#define ITEM_EXP					900001000
#define ITEM_COUNT					900002000
#define ITEM_KC						980000000

CSlaveMerchant::CSlaveMerchant()
{
	m_iRemainingTime = 0;
	txt_sure = 0;
	text_cash = 0;
	text_money = 0;
}

CSlaveMerchant::~CSlaveMerchant()
{
	m_iRemainingTime = 0;
}



bool CSlaveMerchant::Load(HANDLE hFile)
{
	if (CN3UIBase::Load(hFile) == false)
		return false;


	std::string find = xorstr("grp_info");
	grp_merchant = (CN3UIBase*)GetChildByID(find);
	find = xorstr("grp_inventory");
	grp_inventory = (CN3UIBase*)GetChildByID(find);

	find = xorstr("btn_close");
	btn_close = (CN3UIButton*)GetChildByID(find);
	find = xorstr("btn_cash");
	btn_kc = (CN3UIButton*)GetChildByID(find);
	find = xorstr("btn_gold");
	btn_coin = (CN3UIButton*)GetChildByID(find);
	find = xorstr("close_merchant");
	btn_cancel = (CN3UIButton*)GetChildByID(find);
	find = xorstr("btn_useslavescroll");
	btn_useslavescroll = (CN3UIButton*)GetChildByID(find);

	find = xorstr("txt_valuedelay");
	txt_sure = (CN3UIString*)GetChildByID(find);
	find = xorstr("txt_valuestate");
	txt_durum = (CN3UIString*)GetChildByID(find);
	find = xorstr("text_cashpoint");
	text_cash = (CN3UIString*)GetChildByID(find);
	find = xorstr("text_gold");
	text_money = (CN3UIString*)GetChildByID(find);


	//merchant

	for (int i = 0; i <= 11; i++)
	{
		find = string_format(xorstr("st%d"), i);
		c[i] = (CN3UIString*)grp_merchant->GetChildByID(find);
	}


	float fUVAspect = (float)45.0f / (float)64.0f;


	for (int i = 0; i <= 11; i++)
	{
		find = string_format(xorstr("%d"), i);
		CN3UIArea* area = (CN3UIArea*)grp_merchant->GetChildByID(find);

		CN3UIIcon* icon = new CN3UIIcon();
		icon->Init(this);
		icon->SetUIType(UI_TYPE_ICON);
		icon->SetStyle(UISTYLE_ICON_ITEM | UISTYLE_ICON_CERTIFICATION_NEED);
		icon->SetUVRect(0, 0, fUVAspect, fUVAspect);
		icon->SetRegion(area->GetRegion());

		ItemInfo* inf = new ItemInfo();
		inf->icon = icon;
		inf->tbl = NULL;
		inf->nItemID = 0;

		itemsMerch.push_back(inf);
	}

	//inventory

	for (int i = 0; i <= 27; i++)
	{
		find = string_format(xorstr("a%d"), i);
		d[i] = (CN3UIString*)grp_inventory->GetChildByID(find);
	}


	float fUVAspect2 = (float)45.0f / (float)64.0f;


	for (int i = 0; i <= 27; i++)
	{
		find = string_format(xorstr("%d"), i);
		CN3UIArea* area = (CN3UIArea*)grp_inventory->GetChildByID(find);

		CN3UIIcon* icon = new CN3UIIcon();
		icon->Init(this);
		icon->SetUIType(UI_TYPE_ICON);
		icon->SetStyle(UISTYLE_ICON_ITEM | UISTYLE_ICON_CERTIFICATION_NEED);
		icon->SetUVRect(0, 0, fUVAspect2, fUVAspect2);
		icon->SetRegion(area->GetRegion());

		ItemInfo* inf = new ItemInfo();
		inf->icon = icon;
		inf->tbl = NULL;
		inf->nItemID = 0;

		itemsInventory.push_back(inf);
	}


	if (TotalTime == 1 || m_iRemainingTime > 0)
		m_iRemainingTime = 0;


	SetVisible(true);

	SetPos(Engine->m_UiMgr->GetScreenCenter(this).x, Engine->m_UiMgr->GetScreenCenter(this).y);

	return true;
}


void CSlaveMerchant::Update(Packet pkt, vector<SlaveItem> drops, vector<InventoryItem> drops2)
{
	uint8 subcode;
	pkt >> subcode;
	switch (subcode)
	{
	case UPDATE_SLAVE_MERCHANT:
	{

		uint32 TotalKC, TotalCoins;
		pkt >> TotalKC >> TotalCoins;

		if (m_iRemainingTime > 3600)
			m_iRemainingTime = 3600;

		KC = TotalKC;
		COINS = TotalCoins;


		text_cash->SetString(string_format(xorstr("%s"), Engine->StringHelper->NumberFormat(KC).c_str()));
		text_money->SetString(string_format(xorstr("%s"), Engine->StringHelper->NumberFormat(COINS).c_str()));

		m_Data = drops;

		m_Data2 = drops2;

		Clear();

		int slot = 0;
		for (auto drop : m_Data)
		{
			if (drop.nItemID == 0) {
				c[slot]->SetString(string_format(xorstr(" ")));
				continue;
			}

			TABLE_ITEM_BASIC* tbl = Engine->tblMgr->getItemData(drop.nItemID);
			if (tbl != nullptr)
			{
				c[slot]->SetString(string_format(xorstr("%d"), drop.count1));


				std::vector<char> buffer(256, NULL);

				sprintf(&buffer[0], "ui\\itemicon_%.1d_%.4d_%.2d_%.1d.dxt",
					(tbl->dxtID / 10000000),
					(tbl->dxtID / 1000) % 10000,
					(tbl->dxtID / 10) % 100,
					tbl->dxtID % 10);

				std::string szIconFN = &buffer[0];

				SRC dxt = hdrReader->GetSRC(szIconFN);

				if (dxt.sizeInBytes == 0) {
					string iconID = to_string(tbl->Num);
					szIconFN = "itemicon_" + iconID.substr(0, 1) + "_" + iconID.substr(1, 4) + "_" + iconID.substr(5, 2) + "_" + iconID.substr(7, 1) + ".dxt";
				}

				itemsMerch[slot]->icon->SetTex(szIconFN);
				itemsMerch[slot]->tbl = tbl;

			}

			itemsMerch[slot]->nItemID = drop.nItemID;
			slot++;
		}

		int slot2 = 0;
		for (auto drop : m_Data2)
		{
			if (drop.nItemID == 0) {
				d[slot2]->SetString(string_format(xorstr("%d"), drop.count1));
				continue;
			}

			TABLE_ITEM_BASIC* tbl = Engine->tblMgr->getItemData(drop.nItemID);
			if (tbl != nullptr)
			{
				d[slot2]->SetString(string_format(xorstr("%d"), drop.count1));


				std::vector<char> buffer(256, NULL);

				sprintf(&buffer[0], "ui\\itemicon_%.1d_%.4d_%.2d_%.1d.dxt",
					(tbl->dxtID / 10000000),
					(tbl->dxtID / 1000) % 10000,
					(tbl->dxtID / 10) % 100,
					tbl->dxtID % 10);

				std::string szIconFN = &buffer[0];

				SRC dxt = hdrReader->GetSRC(szIconFN);

				if (dxt.sizeInBytes == 0) {
					string iconID = to_string(tbl->Num);
					szIconFN = "itemicon_" + iconID.substr(0, 1) + "_" + iconID.substr(1, 4) + "_" + iconID.substr(5, 2) + "_" + iconID.substr(7, 1) + ".dxt";
				}

				itemsInventory[slot2]->icon->SetTex(szIconFN);
				itemsInventory[slot2]->tbl = tbl;

			}

			itemsInventory[slot2]->nItemID = drop.nItemID;
			slot2++;
		}

	}
	break;
	}
}


void CSlaveMerchant::UpdateTimer(Packet pkt)
{
	uint8 subcode;

	pkt >> subcode >> TotalTime2;

	if (TotalTime2 == 1)
	{

		Engine->m_PlayerBase->m_iSlaveTime += 3600;

	}
	else if (TotalTime2 == 2)
	{

		TotalTime = 1;
	}

}

bool CSlaveMerchant::ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg)
{
	if (dwMsg == UIMSG_BUTTON_CLICK)
	{


		if (pSender == btn_close) {
			Close();
		}
		if (pSender == btn_confrim) {
			Packet SendInventory(XSafe, uint8(XSafeOpCodes::PL_SLAVE_MERC));
			SendInventory << uint8(4);
			Engine->Send(&SendInventory);
			Close();
		}

		if (pSender == btn_cancel) {//pazarı kapatma 
			Packet CloseMerc(XSafe, uint8(XSafeOpCodes::PL_SLAVE_MERC));
			CloseMerc << uint8(6) << uint8(0);
			Engine->Send(&CloseMerc);
			Close();
		}

		if (pSender == btn_useslavescroll) {//sc bas
			Packet CloseMerc(XSafe, uint8(XSafeOpCodes::PL_SLAVE_MERC));
			CloseMerc << uint8(3) << uint8(0);
			Engine->Send(&CloseMerc);

		
		}

		if (pSender == btn_kc || pSender == btn_coin) {
			if (pSender == btn_coin) {
				if (Engine->m_UiMgr->uiSlaveMerchantPrice == NULL) {
					Engine->m_UiMgr->OpenSlaveMerchantPrice();
				}
				else
				{
					if (Engine->m_UiMgr->uiSlaveMerchantPrice->IsVisible())
						Engine->m_UiMgr->uiSlaveMerchantPrice->Close();
					else
						Engine->m_UiMgr->uiSlaveMerchantPrice->Open();
				}
				Packet SlavePrice(XSafe, uint8(XSafeOpCodes::PL_SLAVE_MERC));
				SlavePrice << uint8(5) << COINS << uint8(2);
				Engine->Send(&SlavePrice);
			}
			if (pSender == btn_kc) {

				if (Engine->m_UiMgr->uiSlaveMerchantPrice == NULL) {
					Engine->m_UiMgr->OpenSlaveMerchantPrice();
				}
				else
				{
					if (Engine->m_UiMgr->uiSlaveMerchantPrice->IsVisible())
						Engine->m_UiMgr->uiSlaveMerchantPrice->Close();
					else
						Engine->m_UiMgr->uiSlaveMerchantPrice->Open();
				}
				Packet SlavePrice(XSafe, uint8(XSafeOpCodes::PL_SLAVE_MERC));
				SlavePrice << uint8(5) << KC << uint8(1);
				Engine->Send(&SlavePrice);
			}
			SetVisible(false);

		}



	}
	return true;
}

uint32_t CSlaveMerchant::MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld)
{
	uint32_t dwRet = UI_MOUSEPROC_NONE;
#if 0
	if (Engine->m_UiMgr->m_FocusedUI == this)
	{
		bool showTooltip = false;

		for (auto it : itemsMerch)
		{
			if (it->icon->IsIn(ptCur.x, ptCur.y) && it->tbl != nullptr)
			{
				Engine->m_UiMgr->m_pUITooltipDlg->DisplayTooltipsEnable(ptCur.x, ptCur.y, GetItemStruct(it->icon, it->icon->FileName(), it->tbl, it->nItemID), this, false, true, true);
				showTooltip = true;
				break;
			}
		}

		for (auto it : itemsInventory)
		{
			if (it->icon->IsIn(ptCur.x, ptCur.y) && it->tbl != nullptr)
			{
				Engine->m_UiMgr->m_pUITooltipDlg->DisplayTooltipsEnable(ptCur.x, ptCur.y, GetItemStruct(it->icon, it->icon->FileName(), it->tbl, it->nItemID), this, false, true, true);
				showTooltip = true;
				break;
			}
		}



		Engine->m_UiMgr->m_pUITooltipDlg->SetVisible(showTooltip);
	}
#endif
	dwRet |= CN3UIBase::MouseProc(dwFlags, ptCur, ptOld);
	return dwRet;
}

bool CSlaveMerchant::OnKeyPress(int iKey)
{
	if (!IsVisible())
		return CN3UIBase::OnKeyPress(iKey);

	switch (iKey)
	{
	case DIK_ESCAPE:
		Close();
		return true;
		break;
	}

	return CN3UIBase::OnKeyPress(iKey);
}



void CSlaveMerchant::Open()
{
	Packet Update(XSafe, uint8(PL_SLAVE_MERC));//salve merch bilgileri
	Update << uint8(1);
	Engine->Send(&Update);
	SetVisible(true);
}

void CSlaveMerchant::Close()
{
	SetVisible(false);
}

void CSlaveMerchant::Clear()
{
	for (int i = 0; i < itemsMerch.size(); i++)
	{
		itemsMerch[i]->icon->SetTex("");
		itemsMerch[i]->nItemID = 0;
		itemsMerch[i]->tbl = nullptr;
	}

	for (int i = 0; i < itemsInventory.size(); i++)
	{
		itemsInventory[i]->icon->SetTex("");
		itemsInventory[i]->nItemID = 0;
		itemsInventory[i]->tbl = nullptr;
	}
}


void CSlaveMerchant::Tick()
{
	m_iRemainingTime = Engine->m_PlayerBase->m_iSlaveTime;

	/*if (m_iRemainingTime <= 0 || TotalTime == 1)
	{
		m_iRemainingTime = 0;
		Packet CloseMerc(XSafeOpCodes::PL_SLAVE_MERC);
		CloseMerc << uint8(6) << uint8(0);
		Engine->Send(&CloseMerc);
		Close();
		//Engine->m_UiMgr->uiIcons->SlaveStatus(false);
	}
	*/

	if (m_iRemainingTime > 0)
	{
		if (m_iRemainingTime == 1)
		{
			Packet CloseMerc(XSafe, uint8(XSafeOpCodes::PL_SLAVE_MERC));
			CloseMerc << uint8(6) << uint8(0);
			Engine->Send(&CloseMerc);

			Packet CloseMerc2(XSafe, uint8(XSafeOpCodes::PL_SLAVE_MERC));
			CloseMerc2 << uint8(3) << uint8(1);
			Engine->Send(&CloseMerc2);
		}

		uint16_t remainingHourse = (uint16_t)ceil((m_iRemainingTime / 60) / 60);
		uint16_t remainingMinutes = (uint16_t)ceil((m_iRemainingTime / 60));
		uint16_t remainingSeconds = m_iRemainingTime - (remainingMinutes * 60);

		uint16_t dakika;

		dakika = (m_iRemainingTime - (remainingHourse * 3600)) / 60;

		std::string remainingTime;
		if (dakika < 10 && remainingSeconds < 10 && remainingHourse < 10)
			remainingTime = string_format(xorstr("0%d:0%d:0%d"), remainingHourse, dakika, remainingSeconds);

		else if (remainingHourse < 10 && dakika < 10)
			remainingTime = string_format(xorstr("0%d:0%d:%d"), remainingHourse, dakika, remainingSeconds);

		else if (remainingHourse < 10 && remainingSeconds < 10)
			remainingTime = string_format(xorstr("0%d:%d:0%d"), remainingHourse, dakika, remainingSeconds);

		else if (remainingHourse < 10)
			remainingTime = string_format(xorstr("0%d:%d:%d"), remainingHourse, dakika, remainingSeconds);

		else if (dakika < 10 && remainingSeconds < 10)
			remainingTime = string_format(xorstr("%d:0%d:0%d"), remainingHourse, dakika, remainingSeconds);

		else if (remainingSeconds < 10)
			remainingTime = string_format(xorstr("%d:%d:0%d"), remainingHourse, dakika, remainingSeconds);

		else remainingTime = string_format(xorstr("%d:%d:%d"), remainingHourse, dakika, remainingSeconds);

		txt_sure->SetString(string_format(xorstr("%s"), remainingTime.c_str()));
		txt_durum->SetString(xorstr("Active!"));
	}

}

__IconItemSkill* CSlaveMerchant::GetItemStruct(CN3UIImage* img, std::string fileSz, __TABLE_ITEM_BASIC* tbl, uint32 realID)
{
#if 0
	__IconItemSkill* newStruct = new __IconItemSkill;
	newStruct->pUIIcon = img;
	newStruct->szIconFN = fileSz;
	newStruct->index = 0;

	__TABLE_ITEM_BASIC* newItemBasic = new __TABLE_ITEM_BASIC;
	__TABLE_ITEM_BASIC* itemOrg = tbl;
	newItemBasic->Num = itemOrg->Num;
	newItemBasic->extNum = itemOrg->extNum;
	newItemBasic->strName = itemOrg->strName;
	newItemBasic->Description = itemOrg->Description;
	//newItemBasic->customStrings = itemOrg->customStrings;

	newItemBasic->Class = itemOrg->Class;

	newItemBasic->Attack = itemOrg->Attack;
	newItemBasic->Delay = itemOrg->Delay;
	newItemBasic->Range = itemOrg->Range;
	newItemBasic->Weight = itemOrg->Weight;
	newItemBasic->Duration = itemOrg->Duration;
	newItemBasic->repairPrice = itemOrg->repairPrice;
	newItemBasic->sellingPrice = itemOrg->sellingPrice;
	newItemBasic->AC = itemOrg->AC;
	newItemBasic->isCountable = itemOrg->isCountable;
	newItemBasic->ReqLevelMin = itemOrg->ReqLevelMin;

	newItemBasic->ReqStr = itemOrg->ReqStr;
	newItemBasic->ReqHp = itemOrg->ReqHp;
	newItemBasic->ReqDex = itemOrg->ReqDex;
	newItemBasic->ReqInt = itemOrg->ReqInt;
	newItemBasic->ReqMp = itemOrg->ReqMp;

	newItemBasic->SellingGroup = itemOrg->SellingGroup;

	newStruct->pItemBasic = newItemBasic;

	__TABLE_ITEM_EXT* newItemExt = Engine->tblMgr->getExtData(itemOrg->extNum, realID);

	if (newItemExt == nullptr)
	{
		newItemExt = new __TABLE_ITEM_EXT;
		newItemExt->extensionID = itemOrg->extNum;
		newItemExt->szHeader = itemOrg->strName;
		newItemExt->byMagicOrRare = itemOrg->ItemGrade;
		newItemExt->siDamage = 0;
		newItemExt->siAttackIntervalPercentage = 100;
		newItemExt->siHitRate = 0;
		newItemExt->siEvationRate = 0;
		newItemExt->siMaxDurability = 0;
		newItemExt->siDefense = 0;
		newItemExt->siDefenseRateDagger = 0;
		newItemExt->siDefenseRateSword = 0;
		newItemExt->siDefenseRateBlow = 0;
		newItemExt->siDefenseRateAxe = 0;
		newItemExt->siDefenseRateSpear = 0;
		newItemExt->siDefenseRateArrow = 0;
		newItemExt->byDamageFire = 0;
		newItemExt->byDamageIce = 0;
		newItemExt->byDamageThuner = 0;
		newItemExt->byDamagePoison = 0;
		newItemExt->byStillHP = 0;
		newItemExt->byDamageMP = 0;
		newItemExt->byStillMP = 0;
		newItemExt->siBonusStr = 0;
		newItemExt->siBonusSta = 0;
		newItemExt->siBonusHP = 0;
		newItemExt->siBonusDex = 0;
		newItemExt->siBonusMSP = 0;
		newItemExt->siBonusInt = 0;
		newItemExt->siBonusMagicAttak = 0;
		newItemExt->siRegistFire = 0;
		newItemExt->siRegistIce = 0;
		newItemExt->siRegistElec = 0;
		newItemExt->siRegistMagic = 0;
		newItemExt->siRegistPoison = 0;
		newItemExt->siRegistCurse = 0;
		newItemExt->siNeedLevel = 0;
	}

	__TABLE_SET_ITEM* setitem = Engine->tblMgr->getSetData(realID);

	if (setitem == nullptr)
	{

		setitem = new __TABLE_SET_ITEM;
		setitem->SetIndex = realID;
		setitem->SetName = "none";
		setitem->ACBonus = 0;
		setitem->HPBonus = 0;
		setitem->MPBonus = 0;
		setitem->StrengthBonus = 0;
		setitem->StaminaBonus = 0;
		setitem->DexterityBonus = 0;
		setitem->IntelBonus = 0;
		setitem->CharismaBonus = 0;
		setitem->FlameResistance = 0;
		setitem->GlacierResistance = 0;
		setitem->LightningResistance = 0;
		setitem->PoisonResistance = 0;
		setitem->MagicResistance = 0;
		setitem->CurseResistance = 0;
		setitem->XPBonusPercent = 0;
		setitem->CoinBonusPercent = 0;
		setitem->APBonusPercent = 0;
		setitem->APBonusClassType = 0;
		setitem->APBonusClassPercent = 0;
		setitem->ACBonusClassType = 0;
		setitem->ACBonusClassPercent = 0;
		setitem->MaxWeightBonus = 0;
		setitem->NPBonus = 0;







	}



	newStruct->pItemSet = setitem;

	newStruct->pItemExt = newItemExt;

	return newStruct;
#else
//return;
return nullptr;
#endif
}


