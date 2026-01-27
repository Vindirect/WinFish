#pragma once
#include "GameObject.h"
namespace Sexy
{
	class Larva : public GameObject
	{
	public:
		double mXD;
		double mYD;
		double mVY;
		int mAnimFrame;
		bool mPickedUp;
		bool m0x175;
		
	public:
		Larva();
		Larva(int theX, int theY);

		virtual void			Update();
		virtual void			Draw(Graphics* g);
		virtual void			MouseDown(int x, int y, int theClickCount);

		virtual void			Remove();										//[75]
		virtual void			Sync(DataSync* theSync);						//[80]

		int						GetValue();
	};
}

