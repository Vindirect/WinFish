#pragma once
#include "GameObject.h"
#include "Coin.h"
namespace Sexy
{
	class Penta : public GameObject
	{
	public:
		double					mXD;
		double					mYD;
		double					mVX;
		double					mVY;
		double					mSpeedMod;
		int						mBoughtTimer;
		double					mTargetVX;
		int						mSpeedState;
		int						mEatFoodDelayTimer;
		int						mSpeedChangeTimer;
		int						mMovementAnimTimer;
		int						mPentaAnimIndex;
		int						mCoinDropTimer;
		int						m0x1a8;
			
	public:
		Penta();
		Penta(int theX);
		Penta(int theX, int theY);
		virtual ~Penta();

		virtual void			Update();
		virtual void			Draw(Graphics* g);
		virtual void			MouseDown(int x, int y, int theClickCount);

		virtual void			CountRequiredFood(int* theFoodReqPtr);			//[70]
		virtual int				SpecialReturnValue();							//[71]
		virtual void			Remove();										//[75]
		virtual void			SetPosition(double newX, double newY);			//[76]
		virtual void			OnFoodAte(GameObject* obj);						//[77]
		virtual void			UpdateStoreAnimation();							//[78]
		virtual void			DrawStoreAnimation(Graphics* g, int justification);//[79]
		virtual void			Sync(DataSync* theSync);						//[80]

		void					Init();
		bool					HungryLogic();
		bool					HungryBehavior();
		void					Die(bool flag = false);
		void					DrawPenta(Graphics* g);
		void					Remove(bool removeShadow);
		bool					DropCoin();
		GameObject*				FindNearestFood();
		bool					IsCoinValid(Coin* aCoin);
		void					CollideWithFood();
		void					UpdateAnimations();
	};
}

