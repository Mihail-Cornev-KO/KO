#include "UISlavePriestSub.h"

UISlavePriestSub::UISlavePriestSub() 
{
}

UISlavePriestSub::~UISlavePriestSub() {}

bool UISlavePriestSub::Load(HANDLE hFile)
{
	if (CN3UIBase::Load(hFile) == false) return false;



	std::string find = xorstr("btn_priest_open");
	btn_priest_open = (CN3UIButton*)GetChildByID(find);
	find = xorstr("btn_priest_stop");
	btn_priest_stop = (CN3UIButton*)GetChildByID(find);
	find = xorstr("btn_priest_start");
	btn_priest_start = (CN3UIButton*)GetChildByID(find);
	

	POINT eventNoticePos;
	Engine->GetUiPos(Engine->uiTargetBar->m_dVTableAddr, eventNoticePos);
	SetPos(eventNoticePos.x + Engine->GetUiWidth(Engine->uiTargetBar->m_dVTableAddr) + 10, 5);

	return true;
}

bool UISlavePriestSub::ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg)
{
	if (dwMsg == UIMSG_BUTTON_CLICK)
	{
		if (pSender == btn_priest_open)
		{
			if (Engine->m_UiMgr->uiSlavePriest == NULL)
			{
				Engine->m_UiMgr->uiSlavePriest = new UISlavePriest();
				Engine->m_UiMgr->uiSlavePriest->Init(Engine->m_UiMgr);
				Engine->m_UiMgr->uiSlavePriest->LoadFromFile(Engine->m_BasePath + xorstr("DeaFSoft\\DAT-32.uif"), N3FORMAT_VER_1068);
				Engine->m_UiMgr->AddChild(Engine->m_UiMgr->uiSlavePriest);
			}
			else if (!Engine->m_UiMgr->uiSlavePriest->IsVisible())
					Engine->m_UiMgr->uiSlavePriest->Open();
		}
		else if (pSender == btn_priest_stop)
		{
			if (Engine->m_UiMgr->uiSlavePriest != NULL)
			{	
				btn_priest_start->SetState(UI_STATE_BUTTON_NORMAL);
				Packet pkt(XSafe, uint8(PL_SLAVEPRÝEST));
				pkt << uint8(2) << Engine->m_UiMgr->uiSlavePriest->sayi << Engine->m_UiMgr->uiSlavePriest->Buff << Engine->m_UiMgr->uiSlavePriest->Ac << Engine->m_UiMgr->uiSlavePriest->Heal;
				Engine->Send(&pkt);
			}
			else
			Engine->m_UiMgr->ShowMessageBox("Error", "Firstly,You Have To Set Priest Settings.", Ok);
			
		}
		else if (pSender == btn_priest_start)
		{
			if (Engine->m_UiMgr->uiSlavePriest != NULL)
			{
				btn_priest_stop->SetState(UI_STATE_BUTTON_NORMAL);

				Packet pkt(XSafe, uint8(PL_SLAVEPRÝEST));
				pkt << uint8(1) << Engine->m_UiMgr->uiSlavePriest->sayi << Engine->m_UiMgr->uiSlavePriest->Buff << Engine->m_UiMgr->uiSlavePriest->Ac << Engine->m_UiMgr->uiSlavePriest->Heal;
				Engine->Send(&pkt);
			}
			else
			Engine->m_UiMgr->ShowMessageBox("Error", "Firstly,You Have To Set Priest Settings.", Ok);
		}
	}
	return true;
}



uint32_t UISlavePriestSub::MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld)
{
	uint32_t dwRet = UI_MOUSEPROC_NONE;
	dwRet |= CN3UIBase::MouseProc(dwFlags, ptCur, ptOld);
	return dwRet;
}

bool UISlavePriestSub::OnKeyPress(int iKey)
{
	if (!IsVisible())
		return CN3UIBase::OnKeyPress(iKey);

	return true;
}

void UISlavePriestSub::Open()
{
	SetVisible(true);
	POINT eventNoticePos;
	Engine->GetUiPos(Engine->uiTargetBar->m_dVTableAddr, eventNoticePos);
	SetPos(eventNoticePos.x + Engine->GetUiWidth(Engine->uiTargetBar->m_dVTableAddr) + 5, 5);
}

void UISlavePriestSub::Close()
{
	SetVisible(false);
}
