#pragma once
#include "GameObject.h"
#include "FishTypePet.h"

namespace Sexy
{
	class FishTypePet;
	class BoxingGlove : public GameObject
	{
	public:
		FishTypePet* mWalter;
		int m0x15c;
		bool m0x160;
		int m0x164;

	public:
		BoxingGlove(FishTypePet* theWalter, bool right);

		virtual void			Update();
		virtual void			Draw(Graphics* g);

		bool					HandleFishHit(GameObject* theObj, int x, int y, int dir, int unk);
	};
}

