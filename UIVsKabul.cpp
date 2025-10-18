#include "UIVsKabul.h"

CVsKabul::CVsKabul()
{
	kills = 1;
}

CVsKabul::~CVsKabul()
{

}

bool CVsKabul::Load(HANDLE hFile)
{
	if (CN3UIBase::Load(hFile) == false) return false;


	std::string find = xorstr("grp_accept");
	grp_accept = (CN3UIBase*)GetChildByID(find);
	find = xorstr("btn_ok");
	btn_ok = (CN3UIButton*)grp_accept->GetChildByID(find);
	find = xorstr("btn_cancel");
	btn_cancel = (CN3UIButton*)grp_accept->GetChildByID(find);
	find = xorstr("text_notice");
	text_notice = (CN3UIString*)grp_accept->GetChildByID(find);



	SetPos(Engine->m_UiMgr->GetScreenCenter(this).x, Engine->m_UiMgr->GetScreenCenter(this).y);

	return true;
}

bool CVsKabul::ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg)
{
	if (dwMsg == UIMSG_BUTTON_CLICK)
	{
		if (pSender == btn_ok)
		{
			Packet Update(XSafe, uint8(PL_VS_EVENT));
			Update << uint8(2);
			Engine->Send(&Update);
			Close();
		}
		if (pSender == btn_cancel)
		{
			Packet Update(XSafe, uint8(PL_VS_EVENT));
			Update << uint8(3);
			Engine->Send(&Update);
			Close();
		}

	}

	return true;
}

uint32_t CVsKabul::MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld)
{
	uint32_t dwRet = UI_MOUSEPROC_NONE;
	dwRet |= CN3UIBase::MouseProc(dwFlags, ptCur, ptOld);
	return dwRet;
}

bool CVsKabul::OnKeyPress(int iKey)
{
	if (!IsVisible())
		return CN3UIBase::OnKeyPress(iKey);

	switch (iKey)
	{
	case DIK_ESCAPE:
		Close();
		return true;
	}

	return CN3UIBase::OnKeyPress(iKey);
}

void CVsKabul::Open()
{
	SetVisible(true);
}

void CVsKabul::Close()
{
	SetVisible(false);
}

void CVsKabul::Update(Packet pkt)
{
	string isim;
	uint32 np;
	pkt >> isim >> np;

	text_notice->SetString(string_format(xorstr("%s Player offered VS. Reward : %d"), isim.c_str(), np));
}

void CVsKabul::Tick()
{



}

