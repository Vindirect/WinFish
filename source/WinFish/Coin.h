#pragma once
#include "GameObject.h"
namespace Sexy
{
	class OtherTypePet;
	class Coin : public GameObject
	{
	public:
		double mXD;
		double mYD;
		bool m0x168;
		int mAnimationTimer;
		int m0x170;
		int mAnimationFrame;
		int mDisappearTimer;
		int m0x17c;
		int m0x180;
		int m0x184;
		double mVY;
		OtherTypePet* m0x190;
		int mCoinType;
		bool m0x198;
		int m0x19c;
		int m0x1a0;
		SexyString m0x1a4;
		int mComboCount;

		enum CoinTypes
		{
			COIN_UNKNOWN,
			COIN_SILVER_C,
			COIN_GOLD_C,
			COIN_STAR,
			COIN_DIAMOND,
			COIN_PEARL = 6,
			SHELL_SILVER = 8,
			SHELL_GOLD,
			SHELL_STAR,
			//...
			COIN_NIKOPEARL = 15,
			COIN_NOTE,
			COIN_SHRAPNEL_BOMB
		};
	public:
		Coin();
		Coin(int theX, int theY, int theType, OtherTypePet* unk, double theVel);

		virtual void			Update();
		virtual void			Draw(Graphics* g);

		virtual void			MouseDown(int x, int y, int theClickCount);

		virtual void			Remove();										//[75]

		virtual void			Sync(DataSync* theSync);						//[80]

		bool					IsShell();
		int						GetValue();
		void					ReceiveMoney();
		void					PetCollected();
	};
}

