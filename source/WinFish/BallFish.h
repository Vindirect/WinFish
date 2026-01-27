#pragma once
#include "Fish.h"
namespace Sexy
{
	class BallFish : public Fish
	{
	public:
		BallFish();
		BallFish(int theX, int theY, bool velocityRight);
		virtual ~BallFish();

		virtual void			CountRequiredFood(int* theFoodReqPtr);			//[70]
		virtual int				GetShellPrice();								//[72]
		virtual void			OnFoodAte(GameObject* obj);						//[77]
		virtual void			UpdateStoreAnimation();							//[78]
		virtual void			DrawStoreAnimation(Graphics* g, int justification);//[79]
		virtual void			DropCoin();										//[81]
		virtual bool			Hungry();										//[82]
		virtual void			DrawFish(Graphics* g, bool mirror);				//[83]
		virtual bool			HungryBehavior();								//[84]
		virtual GameObject*		FindNearestFood();								//[85]
		virtual void			CollideWithFood();								//[86]
		virtual void			RemoveFromGame(bool removeShadow);				//[87]
		virtual void			FishUpdateAnimation();							//[89]

		void					Init();
		int						GetFoodType();
		bool					CanBallFishEatFood(GameObject* theFood, int theFoodTypePreffered);
	};
}

