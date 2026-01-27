#pragma once
#include "GameObject.h"
namespace Sexy
{
	class Food : public GameObject
	{
	public:
		double mXD, mYD;
		double mVX, mVY;
		int m0x178;
		int m0x17c;
		int m0x180;
		bool m0x184;
		bool mPickedUp;
		int mFoodType;
		int m0x18c;
		bool m0x190;
		int mExoticFoodType;
		int mCantEatTimer;
		
		enum ExoticFoodTypes
		{
			EXO_FOOD_SNOT = 2,
			EXO_FOOD_PIZZA = 3,
			EXO_FOOD_ICE_CREAM = 4,
			EXO_FOOD_CHICKEN = 5,
			EXO_FOOD_GUPPY = 1000 + TYPE_GUPPY,
			EXO_FOOD_OBJECTS_START = EXO_FOOD_GUPPY,
			EXO_FOOD_OSCAR = 1000 + TYPE_OSCAR,
			EXO_FOOD_ULTRA = 1000 + TYPE_ULTRA,
		};

	public:
		Food();
		Food(int theX, int theY, int unk1, bool unk2, int theExoFoodType);
		virtual ~Food();


		virtual void			Update();
		virtual void			Draw(Graphics* g);

		virtual void			MouseDown(int x, int y, int theClickCount);

		virtual void			Remove();										//[75]
		virtual void			Sync(DataSync* theSync);						//[80]

		void					PickUp();
	};
}
