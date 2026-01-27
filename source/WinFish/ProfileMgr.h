#pragma once

#include "SexyAppFramework/Common.h"
#include "DataSync.h"

namespace Sexy
{
	class UserProfile
	{
	public:
		bool mUnlockedPets[GameObject::PET_END];
		int mNumOfUnlockedPets;
		int mTank;
		int mLevel;
		SexyString mUserName;
		int m0x40;
		int mId;
		int mShells;
		int m0x4c;
		int mCyraxNum;
		int mFinishedGameCount;
		bool m0x58;
		bool mFinishedGame;
		bool m0x5a[GameObject::PET_END];
		bool mUnlockedBackgrounds[6];
		bool m0x78[4];
		int m0x7c;
		int m0x80;
		uint mCheatCodeFlags;
		int m0x88;
		int m0x8c;
		int mBubbulatorBought;
		int mAlienAttractorBought;
		int m0x98;

		long long m0xa0;
		bool mStoreScreenBought[8];
		int mBonusItemId;
		int m0xb4;
		int mAdventureScores[21];
		int mTimeTrialScores[4];
		int mChallengeScores[4];

	public:
		UserProfile();

		void						LoadFromMemory();
		void						Init();
		void						SetCheatFlag(char thePos, bool theFlag);
		bool						ToggleCheatFlag(char theIdx);
		void						SyncUsersDat(DataSync& theDataSync);
		void						Save();
		void						SyncData(DataSync& theDataSync);
		void						UnlockPet(int thePetId, bool unlock);
		void						AddShells(int theValue);
		bool						IsPetUnlocked(int idx);
		void						NextLevel();
		SexyString					GetSaveGameFilePath(int theGameMode, int theUserId);
		void						DeleteUserAppData();
		void						SaveScreenSaver(int unk);
		void						Unk01();
		void						WriteAdventureScore(int theTank, int theLevel, int theScore);
		void						WriteTimeTrialScore(int theTank, int theScore);
		void						WriteChallengeScore(int theTank, int theScore);
		int							ReadScreenSaverData(bool doErase);
		void						UpdateStoreData(long long theNewTime);
		int							GetAdventureScore(int theTank, int theLevel);
		int							GetTimeTrialScore(int theTank);
		int							GetChallengeScore(int theTank);
	};

	typedef std::map<SexyString, UserProfile> UserProfilesMap;

	class ProfileMgr
	{
	public:
		UserProfilesMap*			mProfilesMap; // map has m0x160 and m0x161 flags
		int							mCurrentUserId;
		int							m0x10;
		int							m0x14;

	public:
		ProfileMgr();
		~ProfileMgr();

		UserProfile*				GetUserProfile(const SexyString& theProfileName);
		UserProfile*				GetFirstUserProfile();
		void						SyncUsersDat(DataSync& theDataSync);
		void						ReadUsersDat();
		void						SaveUsersDat();
		UserProfile*				InsertProfile(UserProfile* theProfile);
		UserProfile*				MakeNewUser(SexyString* theUserName);
		bool						RenameUser(SexyString& theOldUserName, SexyString& theNewUserName);
		bool						DeleteUser(SexyString& theUserName);
		void						CapMaxUserCount();
	};
}

