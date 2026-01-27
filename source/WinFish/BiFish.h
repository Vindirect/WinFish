#pragma once
#include "Fish.h"
namespace Sexy
{
	class BiFish : public Fish
	{
	public:
		int m0x230;
		int m0x234;

	public:
		BiFish();
		BiFish(int theX, int theY, bool velocityRight);
		virtual ~BiFish();

		virtual void			CountRequiredFood(int* theFoodReqPtr);			//[70]
		virtual void			OnFoodAte(GameObject* obj);						//[77]
		virtual void			UpdateStoreAnimation();							//[78]
		virtual void			DrawStoreAnimation(Graphics* g, int justification);//[79]
		virtual void			Sync(DataSync* theSync);						//[80]

		virtual void			DropCoin();										//[81]
		virtual bool			Hungry();										//[82]
		virtual void			DrawFish(Graphics* g, bool mirror);				//[83]
		virtual bool			HungryBehavior();								//[84]
		virtual GameObject*		FindNearestFood();								//[85]
		virtual void			CollideWithFood();								//[86]
		virtual void			RemoveFromGame(bool removeShadow);				//[87]
		virtual void			FishUpdateAnimation();							//[89]

		void Init();
	};
}

