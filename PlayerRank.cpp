#include "stdafx.h"
#include "PlayerRank.h"
HookPlayerRank::HookPlayerRank()
{


	vector<int>offsets;
	offsets.push_back(0x3F0);
	offsets.push_back(0);

	el_Base = Engine->rdword(KO_DLG, offsets);

	m_btnReview = 0;
	m_btnCancel = 0;
	nHumanBaseTop = 0;
	nKarusBaseTop = 0;
	base_reward = 0;
	for (int i = 0; i < 10; i++) {
		nHumanBase[i] = 0;
		nKarusBase[i] = 0;
		nHumanBaseEffeckt[i] = 0;
		nKarusBaseEffeckt[i] = 0;
		image_reward[i] = 0;
		reward_count[i] = 0;
		RankRewardItemList[i] = 0;
	}




	ParseUIElements();

}
HookPlayerRank::~HookPlayerRank()
{
}

void HookPlayerRank::ParseUIElements()
{


	std::string find = xorstr("el_symbol");
	Engine->GetChildByID(el_Base, find, nHumanBaseTop);

	for (int i = 0; i < 10; i++) {
		find = string_format(xorstr("rank%d"), i);
		Engine->GetChildByID(nHumanBaseTop, find, nHumanBase[i]);
		Engine->SetVisible(nHumanBase[i], false);

	}
	find = xorstr("ka_symbol");
	Engine->GetChildByID(el_Base, find, nKarusBaseTop);
	for (int i = 0; i < 10; i++) {
		find = string_format(xorstr("rank%d"), i);
		Engine->GetChildByID(nKarusBaseTop, find, nKarusBase[i]);
		Engine->SetVisible(nKarusBase[i], false);

	}

	find = xorstr("reward");
	Engine->GetChildByID(el_Base, find, base_reward);
	for (int i = 0; i < 10; i++) {
		find = string_format(xorstr("reward%d"), i);
		Engine->GetChildByID(base_reward, find, image_reward[i]);
		Engine->SetVisible(image_reward[i], true);

		find = string_format(xorstr("rewardcount%d"), i);
		Engine->GetChildByID(base_reward, find, reward_count[i]);
		Engine->SetVisible(reward_count[i], true);
	}
}

bool HookPlayerRank::ResetBase()
{


	for (int i = 0; i < 10; i++) {
		Engine->SetVisible(nKarusBase[i], false);
		Engine->SetVisible(nHumanBase[i], false);
		Engine->SetVisible(nHumanBaseEffeckt[i], false);
		Engine->SetVisible(nKarusBaseEffeckt[i], false);

	}

	return false;
}

uint32_t HookPlayerRank::MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld)
{
	uint32_t dwRet = UI_MOUSEPROC_NONE;
	if (Engine->IsVisible(base_reward))
	{
		bool showTooltip = false;

		for (int i = 0; i < 10; i++)
		{
			if (image_reward[i] != NULL) {
				if (Engine->IsIn(image_reward[i], ptCur.x, ptCur.y))
				{
					Engine->ShowToolTipEx(RankRewardItemList[i], ptCur.x, ptCur.y);
					showTooltip = true;
					break;
				}
			}
		}
	}
	return dwRet;
}