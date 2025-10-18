#include "UISlaveMerchantPrice.h"


#define ITEM_GOLD					900000000	// Coins
#define ITEM_EXP					900001000
#define ITEM_COUNT					900002000
#define ITEM_KC						980000000

CSlaveMerchantPrice::CSlaveMerchantPrice()
{
	m_bIsCoin = false;
	edit_price = NULL;
}

CSlaveMerchantPrice::~CSlaveMerchantPrice()
{
	edit_price = NULL;
}



bool CSlaveMerchantPrice::Load(HANDLE hFile)
{
	if (CN3UIBase::Load(hFile) == false)
		return false;

	std::string find = xorstr("grp_takecoin");
	grp_takecoin = (CN3UIBase*)GetChildByID(find);





	// Group takecoin
	find = xorstr("edit_price");
	edit_price = (CN3UIEdit*)grp_takecoin->GetChildByID(find);
	find = xorstr("text_price");
	text_price = (CN3UIString*)edit_price->GetChildByID(find);


	find = xorstr("btn_cancel2");
	btn_cancel2 = (CN3UIButton*)grp_takecoin->GetChildByID(find);
	find = xorstr("btn_ok");
	btn_ok = (CN3UIButton*)grp_takecoin->GetChildByID(find);
	find = xorstr("btn_coin2");
	btn_coin2 = (CN3UIButton*)grp_takecoin->GetChildByID(find);



	SetVisible(true);

	SetPos(Engine->m_UiMgr->GetScreenCenter(this).x, Engine->m_UiMgr->GetScreenCenter(this).y);


	SetRegion(GetRegion());

	return true;
}



void CSlaveMerchantPrice::Update(Packet pkt)
{
	uint8 subcode;
	pkt >> subcode;
	switch (subcode)
	{
	case SLAVE_MERCHANT_PRICE:
	{
		uint32 TotalCoins;
		uint8 isCashOrCoin;

		pkt >> TotalCoins >> isCashOrCoin;

		COINS = TotalCoins;
		result = isCashOrCoin;


		uint8 text = 0;
		text_price->SetString(string_format(xorstr("%s"), Engine->StringHelper->NumberFormat(text).c_str()));
		
	}
	break;
	}
}


bool CSlaveMerchantPrice::ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg)
{
	if (dwMsg == UIMSG_BUTTON_CLICK)
	{

		if (pSender == btn_coin2)
		{
			m_bIsCoin = !m_bIsCoin;
			if (m_bIsCoin) {
				btn_coin2->SetState(UI_STATE_BUTTON_DOWN);
			}
			else if (!m_bIsCoin) {
				btn_coin2->SetState(UI_STATE_BUTTON_NORMAL);
			}
		}

		if (pSender == btn_cancel2) {
			SetVisible(false);
			if (m_bIsCoin)
				btn_coin2->SetState(UI_STATE_BUTTON_NORMAL);
			m_bIsCoin = false;

		}
		if (pSender == btn_ok) {
			
			if (m_bIsCoin) {
				GetCoinCash();
				SetVisible(false);
				m_bIsCoin = false;
				btn_coin2->SetState(UI_STATE_BUTTON_NORMAL);
			}
			else
			{
				Close();
				string title, message;
				title = "Error";
				message = "Please do not forget tick the Check box.";
				Engine->m_UiMgr->ShowMessageBox(title, message, Ok);
				SetVisible(false);
			}
		}

	}
	return true;
}

uint32_t CSlaveMerchantPrice::MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld)
{
	uint32_t dwRet = UI_MOUSEPROC_NONE;
	dwRet |= CN3UIBase::MouseProc(dwFlags, ptCur, ptOld);
	return dwRet;
}

bool CSlaveMerchantPrice::OnKeyPress(int iKey)
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



void CSlaveMerchantPrice::Open()
{
	SetVisible(true);
}

void CSlaveMerchantPrice::Close()
{
	SetVisible(false);
}

void CSlaveMerchantPrice::Tick()
{
	

}

void CSlaveMerchantPrice::GetCoinCash()
{
	uint32 total_coin = 0;
	uint32 total_kc = 0;
	uint8 isCashNoah = result;

	int x = atoi(text_price->GetString().c_str());

	if (x > COINS) {
		string title, message;
		title = "Error";
		message = "Please be careful.";
		Engine->m_UiMgr->ShowMessageBox(title, message, Ok);
		return;
	}

	if (isCashNoah == 1) {
		total_coin = 0;
		total_kc = COINS - x;
	}
	else if (isCashNoah == 2) {
		total_coin = COINS - x;
		total_kc = 0;
	}


	Packet SendCoins(XSafe, uint8(XSafeOpCodes::PL_SLAVE_MERC));
	SendCoins << uint8(2) << total_kc << total_coin;
	Engine->Send(&SendCoins);
}