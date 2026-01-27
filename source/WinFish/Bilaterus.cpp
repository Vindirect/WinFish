#include "Bilaterus.h"
#include "BilaterusHead.h"
#include "BilaterusBone.h"
#include "Board.h"
#include "Res.h"

#include "SexyAppFramework/WidgetManager.h"

#define SPINE_FOLLOW_RESPONSE_TIME 15
#define SPINE_SPEED_CHANGE_RESPONSE 0.05

using namespace Sexy;

Sexy::Bilaterus::Bilaterus()
{
	mType = TYPE_BILATERUS;
	mActiveHead = nullptr;
	mPassiveHead = nullptr;
}

Bilaterus::Bilaterus(int theX, int theY)
{
	mType = TYPE_BILATERUS;
	m0x168 = 8;

	mActiveHead = new BilaterusHead(this, theX, theY, true);
	mApp->mWidgetManager->AddWidget(mActiveHead);
	mPassiveHead = new BilaterusHead(this, theX, theY, false);
	mApp->mWidgetManager->AddWidget(mPassiveHead);

	mBoneList = new BoneList();
	mBoneList->reserve(6);

	for (int i = 0; i < 6; i++)
	{
		BilaterusBone* aBone = new BilaterusBone(this, theX, theY, i);
		mApp->mWidgetManager->AddWidget(aBone);
		mBoneList->push_back(aBone);
	}

	m0x164 = false;
	mMouseVisible = false;
	m0x170 = 0;
	m0x16c = 15;

	if ((mApp->mSeed->Next() & 1) == 0)
		ChangeHead();
	else
		mApp->mBoard->PlaySample(SOUND_RATTLE_ID, 3, 1.0);
}

Bilaterus::~Bilaterus()
{
	for (int i = 0; i < mBoneList->size(); i++)
		delete mBoneList->at(i);
	delete mActiveHead;
	delete mPassiveHead;
	delete mBoneList;
}

void Sexy::Bilaterus::MarkDirty()
{
	GameObject::MarkDirty();
	if (mActiveHead)
		mActiveHead->MarkDirty();

	for (int i = 0; i < 6; i++)
		mBoneList->at(i)->MarkDirty();

	if (mPassiveHead)
		mPassiveHead->MarkDirty();
}

void Sexy::Bilaterus::RemovedFromManager(WidgetManager* theWidgetManager)
{
	GameObject::RemovedFromManager(theWidgetManager);
	if (mActiveHead)
		theWidgetManager->RemoveWidget(mActiveHead);

	for (int i = 0; i < 6; i++)
		theWidgetManager->RemoveWidget(mBoneList->at(i));

	if (mPassiveHead)
		theWidgetManager->RemoveWidget(mPassiveHead);
}

void Bilaterus::Update()
{
	if (mApp->mBoard == nullptr || mApp->mBoard->mPause)
		return;

	if (mApp->mGameMode == WinFishApp::GAMEMODE_VIRTUAL_TANK && mUpdateCnt > 4320)
	{
		if (mActiveHead != nullptr)
		{
			if (mPassiveHead == nullptr)
			{
				mApp->mBoard->SpawnLaserShot(mActiveHead->mX - 40 + mActiveHead->mWidth / 2, 
					mActiveHead->mY - 40 + mActiveHead->mHeight / 2);
			}
			else
			{
				mApp->mBoard->SpawnLaserShot(mActiveHead->mX - 40 + mActiveHead->mWidth / 2,
					mActiveHead->mY - 40 + mActiveHead->mHeight / 2);
				mApp->mBoard->SpawnLaserShot(mPassiveHead->mX - 40 + mPassiveHead->mWidth / 2,
					mPassiveHead->mY - 40 + mPassiveHead->mHeight / 2);
				mApp->mBoard->SpawnLaserShot(mActiveHead->mWidth / 2 + mPassiveHead->mWidth / 2 + mPassiveHead->mX + mActiveHead->mX / 2 - 40,
					mActiveHead->mHeight / 2 + mPassiveHead->mHeight / 2 + mPassiveHead->mY + mActiveHead->mY / 2 - 40);
			}
		}
		mApp->mBoard->PlaySample(SOUND_SFX_ID, 3, 1.0);
		RemoveHelper02(true);
		return;
	}

	UpdateCounters();
	if (m0x16c != 0)
		m0x16c--;
	else
	{
		m0x170++;
		if (!m0x164 && m0x170 > 999)
		{
			ChangeHead();
			m0x170 = 0;
		}

		if (mActiveHead)
			mActiveHead->UpdateHead();

		for (int i = 0; i < 6; i++)
			mBoneList->at(i)->UpdateBone();

		if (mPassiveHead)
			mPassiveHead->UpdateHead();

		if (mActiveHead && mActiveHead->m0x1d8 <= 0.0)
			mActiveHead->Die(true);
	}
}

void Sexy::Bilaterus::OrderInManagerChanged()
{
	if (mPassiveHead)
		mWidgetManager->BringToFront(mPassiveHead);

	for(int i = 5; i >= 0; i--)
		mWidgetManager->BringToFront(mBoneList->at(i));

	if (mActiveHead)
		mWidgetManager->BringToFront(mActiveHead);
}

void Sexy::Bilaterus::CountRequiredFood(int* theFoodReqPtr)
{
	theFoodReqPtr[0]++;
	theFoodReqPtr[3]++;
	theFoodReqPtr[4]++;
}

void Sexy::Bilaterus::VFT74()
{
	mApp->mBoard->Unk12();
}

void Sexy::Bilaterus::Remove()
{
	Remove(true);
}

void Sexy::Bilaterus::Sync(DataSync* theSync)
{
	GameObject::Sync(theSync);
	theSync->SyncBool(m0x164);
	theSync->SyncLong(m0x168);
	theSync->SyncLong(m0x16c);
	theSync->SyncLong(m0x170);
	DataReader* aReader = theSync->mReader;
	DataWriter* aWriter = theSync->mWriter;
	if (aReader == nullptr)
	{
		aWriter->WriteBool((mActiveHead != nullptr));
		if (mActiveHead)
			mActiveHead->Sync(theSync);
		aWriter->WriteBool((mPassiveHead != nullptr));
		if (mPassiveHead)
			mPassiveHead->Sync(theSync);

		aWriter->WriteLong(mBoneList->size());
		for (int i = 0; i < mBoneList->size(); i++)
			mBoneList->at(i)->Sync(theSync);
	}
	else
	{
		mBoneList = new BoneList();
		mBoneList->reserve(6);
		bool hasActiveHead = aReader->ReadBool();
		if (hasActiveHead)
		{
			mActiveHead = new BilaterusHead(this, mX, mY, true);
			mActiveHead->Sync(theSync);
			mApp->mWidgetManager->AddWidget(mActiveHead);
		}
		bool hasPassiveHead = aReader->ReadBool();
		if (hasPassiveHead)
		{
			mPassiveHead = new BilaterusHead(this, mX, mY, false);
			mPassiveHead->Sync(theSync);
			mApp->mWidgetManager->AddWidget(mPassiveHead);
		}

		ulong aNumOfBones = aReader->ReadLong();
		for (int i = 0; i < aNumOfBones; i++)
		{
			BilaterusBone* aBone = new BilaterusBone(this, mX, mY, i);
			mBoneList->push_back(aBone);
			aBone->Sync(theSync);
			mApp->mWidgetManager->AddWidget(aBone);
		}
	}
}

void Sexy::Bilaterus::ChangeHead()
{
	BilaterusHead* aPrevActive = mActiveHead;
	BilaterusHead* aPrevPassive = mPassiveHead;
	mActiveHead = aPrevPassive;
	mPassiveHead = aPrevActive;

	if (aPrevActive == nullptr || aPrevPassive == nullptr)
		return;

	aPrevPassive->mHeadActive = true;
	mPassiveHead->mHeadActive = false;
	mActiveHead->m0x1cc = -mActiveHead->m0x1cc;
	mPassiveHead->m0x1cc = -mPassiveHead->m0x1cc;
	mActiveHead->mVX = 0;
	mPassiveHead->mVX = 0;

	BoneList* aBoneList = new BoneList();
	aBoneList->reserve(6);

	int j = 5;
	for (int i = 0; i < 6; i++)
	{
		mBoneList->at(j)->mBoneId = i;
		aBoneList->push_back(mBoneList->at(j));
		j--;
	}

	if (mBoneList)
		delete mBoneList;
	
	mBoneList = aBoneList;
	mActiveHead->BHUnk01();
	for (int i = 0; i < 5; i++)
	{
		BilaterusBone* aBone = mBoneList->at(i);
		aBone->UpdateFollowPosAndVel();
		aBone->m0x1b8 = -aBone->m0x1b8;
		aBone->m0x170 = 0.0;
	}
	mPassiveHead->BHUnk01();
	mApp->mBoard->PlaySample(SOUND_RATTLE_ID, 3, 1.0);
}

bool Sexy::Bilaterus::Shoot(int theX, int theY)
{
	bool isShot = false;
	if (mActiveHead != nullptr)
	{
		isShot = mActiveHead->TakeHit(theX, theY);
		if (isShot)
			mApp->mBoard->SpawnShot(theX - 40, theY - 40, 2);
	}
	return isShot;
}

void Sexy::Bilaterus::SpawnWarp()
{
	Warp* aWarp = new Warp(mActiveHead->mX - 10, mActiveHead->mY - 70);
	mApp->mBoard->AddGameObject(aWarp);
	mApp->mBoard->mWidgetManager->AddWidget(aWarp);
	mApp->mBoard->mWidgetManager->BringToFront(aWarp);
}

void Sexy::Bilaterus::Remove(bool flag)
{
	if (mMisslePtr)
		mMisslePtr->Remove();

	if (mApp->mBoard->mCyraxPtr == nullptr)
	{
		if (CanDropCoin() && flag)
			mApp->mBoard->DropCoin(mX + 25, mY + 25, 4, nullptr, -1.0, 0);
	}

	if (flag)
	{
		for (int i = 0; i < 6; i++)
		{
			BilaterusBone* aBone = mBoneList->at(i);
			mApp->mBoard->SpawnMissle(aBone->mX, aBone->mY, nullptr, Missle::MISSLE_BONE);
		}
		int aType = 5;
		if (mActiveHead->m0x160 == 0)
			aType = 4;
		mApp->mBoard->SpawnMissle(mActiveHead->mX, mActiveHead->mY, nullptr, aType);
	}

	mApp->mBoard->mWidgetManager->RemoveWidget(this);
	mApp->SafeDeleteWidget(this);
	mApp->mBoard->RemoveGameObjectFromLists(this, false);
	if (mShadowPtr)
		mShadowPtr->Remove();

	mApp->mBoard->Unk12();
	if (flag)
	{
		for (int i = (mApp->mSeed->Next() & 1) + 2; i > 0; i--)
		{
			mApp->mBoard->SpawnBubble(mApp->mSeed->Next() % 40 + 10 + mX, mApp->mSeed->Next() % 40 + 10 + mY);
			mApp->mBoard->SpawnBubble(mApp->mSeed->Next() % 20 + 20 + mX, mApp->mSeed->Next() % 20 + 20 + mY);
		}

		for (int i = (mApp->mSeed->Next() % 3) + 4; i > 0; i--)
		{
			mApp->mBoard->SpawnShot(mApp->mSeed->Next() % 40 + 10 + mX, mApp->mSeed->Next() % 40 + 10 + mY, mApp->mSeed->Next() % 3 + 3);
			mApp->mBoard->SpawnShot(mApp->mSeed->Next() % 20 + 20 + mX, mApp->mSeed->Next() % 20 + 20 + mY, mApp->mSeed->Next() % 3 + 3);
		}

		mApp->mBoard->PlaySample(SOUND_EXPLODE_ID, 3, 1.0);
		mApp->mBoard->PlaySample(SOUND_EXPLOSION1_ID, 3, 1.0);
	}
}
