#pragma once
#include "Fish.h"
namespace Sexy
{
    class Gekko : public Fish
    {
    public:
        Gekko();
        Gekko(int theX, int theY);
        Gekko(int theX, int theY, bool velocityRight);
        virtual ~Gekko();

        virtual void			CountRequiredFood(int* theFoodReqPtr);			//[70]
        virtual void			OnFoodAte(GameObject* obj);						//[77]
        virtual void			DrawStoreAnimation(Graphics* g, int justification);//[79]

        virtual void			DropCoin();										//[81]
        virtual bool			Hungry();										//[82]
        virtual void			DrawFish(Graphics* g, bool mirror);				//[83]
        virtual bool			HungryBehavior();								//[84]
        virtual GameObject*     FindNearestFood();								//[85]
        virtual void			CollideWithFood();								//[86]
        virtual void			RemoveFromGame(bool removeShadow);				//[87]

        void                    Init();
        void			        DrawGekko(Graphics* g, bool mirror);
        int                     GetDrawRow(bool isHungry);
        bool                    FoodCollided(GameObject* theObj);
    };
}

