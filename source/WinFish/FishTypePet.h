#pragma once

#include "Fish.h"
#include "BoxingGlove.h"

namespace Sexy
{
	class BoxingGlove;
	class Alien;

	class FishTypePet : public Fish
	{
	public:
		bool m0x230;
		int mFishTypePetType;
		int m0x238;
		int m0x23c;
		int m0x240;
		bool m0x244;
		bool m0x245;
		int m0x248;
		BoxingGlove* m0x24c;
		bool m0x250;
		double m0x258;
		int m0x260;
		int m0x264;

	public:
		FishTypePet();
		FishTypePet(int theX, int theY, int thePetType, bool flag);
		virtual ~FishTypePet();

		virtual void			Update();
		virtual void			MouseDown(int x, int y, int theClickCount);

		virtual void			CountRequiredFood(int* theFoodReqPtr);			//[70]
		virtual void			PrestoMorph(int thePetId);						//[73]
		virtual void			Remove();										//[75]
		virtual void			Sync(DataSync* theSync);						//[80]

		virtual void			DropCoin();										//[81]
		virtual bool			Hungry();										//[82]
		virtual void			DrawFish(Graphics* g, bool mirror);				//[83]
		virtual bool			HungryBehavior();								//[84]
		virtual GameObject*		FindNearestFood();								//[85]
		virtual void			CollideWithFood();								//[86]
		virtual void			FishUpdateAnimation();							//[89]


		void					WadsworthAndMerylFunc01();
		bool					HandleMouseDown(int x, int y, int theClickCount);
		bool					WadsworthTimerResetCheck();
		bool					WadsworthFishesCheck();
		void					StanleyFunction();
		void					DamageAlien(Alien* theAlien);
		void					DrawItchy(Graphics* g, bool mirror);
		void					DrawPrego(Graphics* g, bool mirror);
		void					DrawZorf(Graphics* g, bool mirror);
		void					DrawMeryl(Graphics* g, bool mirror);
		void					DrawWadsworth(Graphics* g, bool mirror);
		void					DrawShrapnel(Graphics* g, bool mirror);
		void					DrawGumbo(Graphics* g, bool mirror);
		void					DrawAmp(Graphics* g, bool mirror);
		void					DrawGash(Graphics* g, bool mirror);
		void					DrawAngie(Graphics* g, bool mirror);
		void					DrawStanley(Graphics* g, bool mirror);
		void					DrawWalter(Graphics* g, bool mirror);
		void					DrawCommonPet(Graphics* g, Image* theImage, bool mirror);
	};
}

