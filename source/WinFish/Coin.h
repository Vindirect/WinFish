#ifndef __COIN_H__
#define __COIN_H__

#include "GameObject.h"

namespace Sexy
{
	class OtherTypePet;

	enum CoinTypes
	{
		COIN_UNKNOWN,
		COIN_SILVER_C,
		COIN_GOLD_C,
		COIN_STAR,
		COIN_DIAMOND,
		COIN_UNK_DIAMOND,
		COIN_PEARL,
		COIN_TREASURE,
		BOUNCED_SILVER,
		BOUNCED_GOLD,
		BOUNCED_STAR,
		BOUNCED_DIAMOND,
		BOUNCED_UNK_DIAMOND,
		BOUNCED_PEARL,
		BOUNCED_TREASURE,
		COIN_NIKOPEARL,
		COIN_NOTE,
		COIN_SHRAPNEL_BOMB,
		COIN_UNUSED_PEANUT = 18
	};

	class Coin : public GameObject
	{
	public:
		double			mXD;
		double			mYD;
		bool			m0x168;
		int				mAnimationTimer;
		int				m0x170;
		int				mAnimationFrame;
		int				mDisappearTimer;
		int				m0x17c;
		int				m0x180;
		int				m0x184;
		double			mVY;
		OtherTypePet*	m0x190;
		int				mCoinType;
		bool			m0x198;
		int				m0x19c;
		int				m0x1a0;
		SexyString		m0x1a4;
		int				mComboCount;

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

#endif