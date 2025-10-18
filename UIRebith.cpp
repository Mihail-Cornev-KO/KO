#include "UIRebith.h"

CRebith::CRebith()
{
		m_RebPoint = 2;

	for (int i = 0; i < 5; i++)
		stats[i] = 0;
}

CRebith::~CRebith()
{
}

bool CRebith::Load(HANDLE hFile)
{
	if (CN3UIBase::Load(hFile) == false)
		return false;

	std::string find = xorstr("base_stat");
	base_stat = (CN3UIBase*)GetChildByID(find);

	find = xorstr("use_helps");
	use_helps = (CN3UIBase*)GetChildByID(find);
	find = xorstr("use_confirm");
	use_confirm = (CN3UIBase*)GetChildByID(find);

	find = xorstr("btn_confirm");
	btn_confirm = (CN3UIButton*)use_confirm->GetChildByID(find);
	find = xorstr("btn_canceled");
	btn_canceled = (CN3UIButton*)use_confirm->GetChildByID(find);
	
	

	find = xorstr("use_rebirth");
	use_rebirth = (CN3UIBase*)GetChildByID(find);


	find = xorstr("rebirth_confirm");
	rebirth_confirm = (CN3UIButton*)use_rebirth->GetChildByID(find);
	


	find = xorstr("btn_helps");
	btn_helps = (CN3UIButton*)GetChildByID(find);

	find = xorstr("btn_use_rebirth");
	btn_use_rebirth = (CN3UIButton*)GetChildByID(find);

	find = xorstr("btn_close");
	btn_close = (CN3UIButton*)GetChildByID(find);



	for (int i = 0; i < 5; i++)
	{
		find = string_format(xorstr("str_%d"), i);
		str[i] = (CN3UIString*)base_stat->GetChildByID(find);
		find = string_format(xorstr("btn_%d_1"), i);
		btn_up[i] = (CN3UIButton*)base_stat->GetChildByID(find);
		find = string_format(xorstr("btn_%d_0"), i);
		btn_down[i] = (CN3UIButton*)base_stat->GetChildByID(find);
	}


	find = xorstr("str_point");
	str_point = (CN3UIString*)base_stat->GetChildByID(find);

	find = xorstr("btn_save");
	btn_save = (CN3UIButton*)base_stat->GetChildByID(find);


	find = xorstr("btn_statclose");
	btn_statclose = (CN3UIButton*)base_stat->GetChildByID(find);


	str_point->SetString(string_format(xorstr("%d"), m_RebPoint));
	use_helps->SetVisible(false);
	base_stat->SetVisible(false);
	use_confirm->SetVisible(false);
	SetPos(Engine->m_UiMgr->GetScreenCenter(this).x, Engine->m_UiMgr->GetScreenCenter(this).y);

	return true;
}

void CRebith::Update(Packet pkt)
{

}

bool CRebith::ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg)
{
	if (dwMsg == UIMSG_BUTTON_CLICK)
	{
		if (pSender == btn_close)
			Close();

		if (pSender == btn_statclose)
			base_stat->SetVisible(false);

		for (int i = 0; i < 5; i++)
		{
			if (pSender == btn_up[i])
			{
				if (m_RebPoint <= 0)
				{
					m_RebPoint = 0;
					return false;
				}


				stats[i]++;
				str[i]->SetString(string_format(xorstr("%d"), stats[i]));
				m_RebPoint--;
				str_point->SetString(string_format(xorstr("%d"), m_RebPoint));
			}
		}

		for (int i = 0; i < 5; i++)
		{
			if (pSender == btn_down[i])
			{
				if (stats[i] <= 0)
				{
					stats[i] = 0;
					return false;
				}


				stats[i]--;
				str[i]->SetString(string_format(xorstr("%d"), stats[i]));


				m_RebPoint++;
				str_point->SetString(string_format(xorstr("%d"), m_RebPoint));
			}
		}
		if (pSender == btn_save)
		{

			string Str = str[0]->GetString();
			string Dex = str[1]->GetString();
			string Int = str[2]->GetString();
			string Sta = str[3]->GetString();
			string Cha = str[4]->GetString();

			uint8 RecStr = atoi(Str.c_str());
			uint8 RecSta = atoi(Sta.c_str());
			uint8 RecDex = atoi(Dex.c_str());
			uint8 RecInt = atoi(Int.c_str());
			uint8 RecCha = atoi(Cha.c_str());

			Packet ui(WIZ_CLASS_CHANGE);
			ui << uint8(7) << RecStr << RecSta << RecDex << RecInt << RecCha;
			Engine->Send(&ui);
			Close();
		}
		if (pSender == btn_helps)
			use_helps->SetVisible(true);
		if (pSender == btn_use_rebirth)
			use_helps->SetVisible(false);


		if (pSender == rebirth_confirm)
			use_confirm->SetVisible(true);

		if (pSender == btn_confirm)
		{
			use_confirm->SetVisible(false);
			base_stat->SetVisible(true);
		}
		if (pSender == btn_canceled)
		{
			use_confirm->SetVisible(false);
		}
			
		
		
		
	}

	return true;
}

uint32_t CRebith::MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld)
{
	uint32_t dwRet = UI_MOUSEPROC_NONE;
	dwRet |= CN3UIBase::MouseProc(dwFlags, ptCur, ptOld);
	return dwRet;
}

bool CRebith::OnKeyPress(int iKey)
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

void CRebith::Open()
{
	SetVisible(true);
	m_RebPoint = 2;
}
void CRebith::Close()
{
	SetVisible(false);
}