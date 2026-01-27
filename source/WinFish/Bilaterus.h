#pragma once

#include "Alien.h"
#include "BilaterusBone.h"

namespace Sexy
{
	class BilaterusHead;
	class BilaterusBone;
	typedef std::vector<BilaterusBone*> BoneList;

	class Bilaterus : public GameObject
	{
	public:
		BilaterusHead* mActiveHead;
		BilaterusHead* mPassiveHead;
		BoneList* mBoneList;
		bool m0x164;
		int m0x168;
		int m0x16c;
		int m0x170;

	public:
		Bilaterus();
		Bilaterus(int theX, int theY);
		virtual ~Bilaterus();

		virtual void			MarkDirty();
		virtual void			RemovedFromManager(WidgetManager* theWidgetManager);
		virtual void			Update();
		virtual void			OrderInManagerChanged();

		virtual void			CountRequiredFood(int* theFoodReqPtr);			//[70]
		virtual void			VFT74();										//[74]
		virtual void			Remove();										//[75]
		virtual void			Sync(DataSync* theSync);						//[80]

		void					ChangeHead();
		bool					Shoot(int theX, int theY);
		void					SpawnWarp();
		void					Remove(bool flag);
	};
}



