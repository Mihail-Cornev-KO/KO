#include "UISlaveAccept.h"


#define ITEM_GOLD					900000000	// Coins
#define ITEM_EXP					900001000
#define ITEM_COUNT					900002000
#define ITEM_KC						980000000

CSlaveAccept::CSlaveAccept()
{
	m_bIsCheck = false;
}

CSlaveAccept::~CSlaveAccept()
{

}



bool CSlaveAccept::Load(HANDLE hFile)
{
	if (CN3UIBase::Load(hFile) == false)
		return false;

	std::string find = xorstr("btn_slave");
	btn_slave = (CN3UIButton*)GetChildByID(find);

	find = xorstr("btn_ok");
	btn_ok = (CN3UIButton*)GetChildByID(find);

	find = xorstr("btn_cancel");
	btn_cancel = (CN3UIButton*)GetChildByID(find);

	SetVisible(true);

	SetPos(Engine->m_UiMgr->GetScreenCenter(this).x, Engine->m_UiMgr->GetScreenCenter(this).y);


	SetRegion(GetRegion());

	return true;
}



bool CSlaveAccept::ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg)
{
	if (dwMsg == UIMSG_BUTTON_CLICK)
	{

		if (pSender == btn_slave)
		{
			m_bIsCheck = !m_bIsCheck;
			if (m_bIsCheck) {
				btn_slave->SetState(UI_STATE_BUTTON_DOWN);
			}
			else if (!m_bIsCheck) {
				btn_slave->SetState(UI_STATE_BUTTON_NORMAL);
			}
		}

		if (pSender == btn_cancel) {
			SetVisible(false);
			Packet Merc(XSafe, uint8(XSafeOpCodes::PL_SLAVE_MERC));
			Merc << uint8(6) << uint8(0);
			Engine->Send(&Merc);
			if (m_bIsCheck)
				btn_slave->SetState(UI_STATE_BUTTON_NORMAL);
			m_bIsCheck = false;

		}
		if (pSender == btn_ok) {

			if (m_bIsCheck) {
				SetVisible(false);

				Packet Merc(XSafe, uint8(XSafeOpCodes::PL_SLAVE_MERC));
				Merc << uint8(6) << uint8(1);
				Engine->Send(&Merc);

				m_bIsCheck = false;
				btn_slave->SetState(UI_STATE_BUTTON_NORMAL);
			}
			else if (!m_bIsCheck) {
				SetVisible(false);
				string title, message;
				title = "Error";
				message = "Please do not forget tick the Check box to slave.";
				Engine->m_UiMgr->ShowMessageBox(title, message, Ok);
			}
		}

	}
	return true;
}

uint32_t CSlaveAccept::MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld)
{
	uint32_t dwRet = UI_MOUSEPROC_NONE;
	dwRet |= CN3UIBase::MouseProc(dwFlags, ptCur, ptOld);
	return dwRet;
}

bool CSlaveAccept::OnKeyPress(int iKey)
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



void CSlaveAccept::Open()
{
	SetVisible(true);
}

void CSlaveAccept::Close()
{
	SetVisible(false);
}

void CSlaveAccept::Tick()
{


}

