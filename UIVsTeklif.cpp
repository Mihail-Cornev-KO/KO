#include "UIVsTeklif.h"

CVsTeklif::CVsTeklif()
{
	kills = 1;
}

CVsTeklif::~CVsTeklif()
{

}

bool CVsTeklif::Load(HANDLE hFile)
{
	if (CN3UIBase::Load(hFile) == false) return false;


	std::string find = xorstr("btn_close");
	btn_close = (CN3UIButton*)GetChildByID(find);
	find = xorstr("edit_cashvs");
	edit_cashvs = (CN3UIEdit*)GetChildByID(find);
	find = xorstr("edit_str2");
	edit_str2 = (CN3UIString*)edit_cashvs->GetChildByID(find);
	find = xorstr("txt_kill");
	txt_kill = (CN3UIString*)GetChildByID(find);
	find = xorstr("btn_eksi");
	btn_eksi = (CN3UIButton*)GetChildByID(find);
	find = xorstr("btn_arti");
	btn_arti = (CN3UIButton*)GetChildByID(find);
	find = xorstr("btn_1v1");
	btn_1v1 = (CN3UIButton*)GetChildByID(find);




	SetPos(Engine->m_UiMgr->GetScreenCenter(this).x, Engine->m_UiMgr->GetScreenCenter(this).y);

	return true;
}

bool CVsTeklif::ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg)
{
	if (dwMsg == UIMSG_BUTTON_CLICK)
	{
		if (pSender == btn_close)
		{
			Close();
		}
		if (pSender == btn_eksi)
		{


			kills -= 2;

			if (kills <= 1)
				kills = 1;

			txt_kill->SetString(Engine->StringHelper->NumberFormat(kills).c_str());
		}
		if (pSender == btn_arti)
		{

			kills += 2;

			if (kills >= 21)
				kills = 21;

			txt_kill->SetString(Engine->StringHelper->NumberFormat(kills).c_str());
		}
		if (pSender == btn_1v1)
		{
			if (txt_kill->m_szString != "")
			{
				string Loyality = edit_cashvs->GetString();
				string Kill = txt_kill->GetString();

				Packet Update(XSafe, uint8(PL_VS_EVENT));
				Update << uint8(1) << Loyality << Kill << Engine->VsTeklif.GetID;
				Engine->Send(&Update);
				Close();

			}
		}
	}

	return true;
}

uint32_t CVsTeklif::MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld)
{
	uint32_t dwRet = UI_MOUSEPROC_NONE;
	dwRet |= CN3UIBase::MouseProc(dwFlags, ptCur, ptOld);
	return dwRet;
}

bool CVsTeklif::OnKeyPress(int iKey)
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

void CVsTeklif::Open()
{
	SetVisible(true);
}

void CVsTeklif::Close()
{
	SetVisible(false);
}

void CVsTeklif::Update()
{

}

void CVsTeklif::Tick()
{



}

