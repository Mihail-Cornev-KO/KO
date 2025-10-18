#include "UISlavePriest.h"
#include "N3BASE/N3UIScrollBar.h"

UISlavePriest::UISlavePriest()
{
	Heal = false;
	Buff = false;
	Ac = false;
	sayi = 0; // heal yüzde
}

UISlavePriest::~UISlavePriest() {}

bool UISlavePriest::Load(HANDLE hFile)
{
	if (CN3UIBase::Load(hFile) == false) return false;



	std::string find = xorstr("slave_priest");
	slave_priest = (CN3UIBase*)GetChildByID(find);
	find = xorstr("btn_close");
	btn_close = (CN3UIButton*)slave_priest->GetChildByID(find);
	find = xorstr("str_priest_name");
	str_priest_name = (CN3UIString*)GetChildByID(find);

	//recovery base
	find = xorstr("recovery");
	recovery = (CN3UIBase*)slave_priest->GetChildByID(find);


	//recovery elements
	find = xorstr("RecoveryHealing_str");
	RecoveryHealing_str = (CN3UIString*)recovery->GetChildByID(find);

	find = xorstr("btn_hp_recovery_off");
	btn_hp_recovery_off = (CN3UIButton*)recovery->GetChildByID(find);
	find = xorstr("btn_hp_recovery_on");
	btn_hp_recovery_on = (CN3UIButton*)recovery->GetChildByID(find);

	find = xorstr("btn_ac_recovery_off");
	btn_ac_recovery_off = (CN3UIButton*)recovery->GetChildByID(find);
	find = xorstr("btn_ac_recovery_on");
	btn_ac_recovery_on = (CN3UIButton*)recovery->GetChildByID(find);

	find = xorstr("btn_healing_recovery_off");
	btn_healing_recovery_off = (CN3UIButton*)recovery->GetChildByID(find);
	find = xorstr("btn_healing_recovery_on");
	btn_healing_recovery_on = (CN3UIButton*)recovery->GetChildByID(find);

	find = xorstr("scr_healing_recovery_rate");
	list_scroll = (CN3UIScrollBar*)recovery->GetChildByID(find);
	
	if (list_scroll != NULL)
		list_scroll->SetRange(0, 100);
	
	string yuzde = to_string(sayi) + "%";

	RecoveryHealing_str->SetString(string_format(xorstr("%s"), yuzde.c_str()));

	string SlaveName = Engine->m_PlayerBase->m_strCharacterName + "'s Priest";
	str_priest_name->SetString(string_format(xorstr("%s"), SlaveName.c_str()));
	

	SetPos(Engine->m_UiMgr->GetScreenCenter(this).x, Engine->m_UiMgr->GetScreenCenter(this).y);

	return true;
}

bool UISlavePriest::ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg)
{
	if (dwMsg == UIMSG_BUTTON_CLICK)
	{
		if (pSender == btn_close)
		{
			Close();
		}
		else if (pSender == btn_hp_recovery_off)
		{
			if (Buff == true)
				Buff = false;
			btn_hp_recovery_off->SetState(UI_STATE_BUTTON_DOWN);
			btn_hp_recovery_on->SetState(UI_STATE_BUTTON_NORMAL);
		}
		else if (pSender == btn_hp_recovery_on)
		{//
			if (Buff == false)
				Buff = true;
			btn_hp_recovery_off->SetState(UI_STATE_BUTTON_NORMAL);
			btn_hp_recovery_on->SetState(UI_STATE_BUTTON_DOWN);
		}
		else if (pSender == btn_ac_recovery_off)
		{
			if (Ac == true)
				Ac = false;
			btn_ac_recovery_off->SetState(UI_STATE_BUTTON_DOWN);
			btn_ac_recovery_on->SetState(UI_STATE_BUTTON_NORMAL);
		}
		else if (pSender == btn_ac_recovery_on)
		{
			if (Ac == false)
				Ac = true;
			btn_ac_recovery_off->SetState(UI_STATE_BUTTON_NORMAL);
			btn_ac_recovery_on->SetState(UI_STATE_BUTTON_DOWN);
		}
		else if (pSender == btn_healing_recovery_off)
		{
			if (Heal == true)
				Heal = false;
			btn_healing_recovery_off->SetState(UI_STATE_BUTTON_DOWN);
			btn_healing_recovery_on->SetState(UI_STATE_BUTTON_NORMAL);
		}
		else if (pSender == btn_healing_recovery_on)
		{
			if (Heal == false)
				Heal = true;
			btn_healing_recovery_off->SetState(UI_STATE_BUTTON_NORMAL);
			btn_healing_recovery_on->SetState(UI_STATE_BUTTON_DOWN);
		}
	}
	return true;
}


void UISlavePriest::change(uint8 job) {
	Packet pkt(XSafe, uint8(PL_JOBCHANGE));
	pkt << uint8(job);
	Engine->Send(&pkt);
}

uint32_t UISlavePriest::MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld)
{
	uint32_t dwRet = UI_MOUSEPROC_NONE;
	dwRet |= CN3UIBase::MouseProc(dwFlags, ptCur, ptOld);
	return dwRet;
}

bool UISlavePriest::OnKeyPress(int iKey)
{
	if (!IsVisible())
		return CN3UIBase::OnKeyPress(iKey);

	return true;
}

void UISlavePriest::Open()
{
	SetVisible(true);
}

void UISlavePriest::Close()
{
	SetVisible(false);
}


void UISlavePriest::Tick()
{
	POINT currentCursorPos = Engine->m_UiMgr->localInput->MouseGetPos();

	if (Engine->m_UiMgr->m_FocusedUI != this) {
		Engine->disableCameraZoom = false;
		return;
	}

	if (IsIn(currentCursorPos.x, currentCursorPos.y))
	{
		Engine->disableCameraZoom = true;

		int delta = Engine->m_UiMgr->localInput->GetMouseWheelDelta();


		if (delta > 0) // wheel to up
		{
			if (list_scroll->GetCurrentPos() != 0) 
			{
				list_scroll->SetCurrentPos(list_scroll->GetCurrentPos() - 1);

				sayi = list_scroll->GetCurrentPos();

				if (sayi == 100)
					sayi = 99;

				string yuzde = to_string(sayi) + "%";

				RecoveryHealing_str->SetString(string_format(xorstr("%s"), yuzde.c_str()));
				
			}
		}
		else if (delta < 0) // whell to down
		{
			if (list_scroll->GetCurrentPos() != list_scroll->GetMaxPos())
			{
				list_scroll->SetCurrentPos(list_scroll->GetCurrentPos() + 1);


				sayi = list_scroll->GetCurrentPos();

				if (sayi == 100)
					sayi = 99;

				string yuzde = to_string(sayi) + "%";

				RecoveryHealing_str->SetString(string_format(xorstr("%s"), yuzde.c_str()));

			}
		}

		sayi = list_scroll->GetCurrentPos();

		if (sayi == 100)
			sayi = 99;

		string yuzde = to_string(sayi) + "%";

		RecoveryHealing_str->SetString(string_format(xorstr("%s"), yuzde.c_str()));

	}
	else Engine->disableCameraZoom = false;
	Engine->m_UiMgr->localInput->SetMouseWheelDelta(0);
}