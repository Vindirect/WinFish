#pragma once
#include "GameObject.h"
namespace Sexy
{
	class Grubber : public GameObject
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
		int						mGrubberAnimIndex;
		int						mEatingAnimTimer;
		int						mCoinDropTimer;
		int						mCoinDropThreshold;

	public:
		Grubber();
		Grubber(int theX);
		Grubber(int theX, int theY);
		virtual ~Grubber();

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
		void					CollideWithFood();
		GameObject*				FindNearestFood();
		void					Remove(bool removeShadow);
		void					CoinDrop();
		void					UpdateAnimations();
		void					DrawGrubber(Graphics* g);
		int						GetRowToDraw(bool hungry);
	};
}

