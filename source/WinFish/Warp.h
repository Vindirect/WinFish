#pragma once
#include "GameObject.h"
namespace Sexy
{
	class Warp : public GameObject
	{
	public:
		int						mExistenceCounter;

	public:
		Warp();
		Warp(int theX, int theY);

		virtual void			Update();
		virtual void			Draw(Graphics* g);

		virtual void			Sync(DataSync* theSync);						//[80]
	};
}

