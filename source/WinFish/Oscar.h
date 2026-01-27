#pragma once
#include "Fish.h"
namespace Sexy
{
	class Oscar : public Fish
	{
	public:
		Oscar();
		Oscar(int theX, int theY);
		Oscar(int theX, int theY, bool velocityRight);

		void					Init(int theX, int theY);

		virtual void			CountRequiredFood(int* theFoodReqPtr);			//[70]
		virtual void			OnFoodAte(GameObject* obj);						//[77]
		virtual void			DrawStoreAnimation(Graphics* g, int unk);		//[79]

		virtual void			DropCoin();										//[81]
		virtual bool			Hungry();										//[82]
		virtual void			DrawFish(Graphics* g, bool mirror);				//[83]
		virtual bool			HungryBehavior();								//[84]
		virtual GameObject*		FindNearestFood();								//[85]
		virtual void			CollideWithFood();								//[86]
		virtual void			RemoveFromGame(bool aRemoveShadow);				//[87]
	};
}

