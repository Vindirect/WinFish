#pragma once

#include <SexyAppFramework/Common.h>
#include "DataSync.h"

namespace Sexy
{
	class UserProfile;
	class DataSync;

	struct HighScoreEntry
	{
		SexyString mUserName;
		int mScore;

		void Sync(DataSync* theSync)
		{
			theSync->SyncString(mUserName);
			theSync->SyncLong(mScore);
		}
	};

	typedef std::list<HighScoreEntry> HighScoreList;

	class HighScoreMgr
	{
	public:
		HighScoreList			mTimeTrialScores[4];
		HighScoreList			mChallengeScores[4];
		HighScoreList			mAdventureScores[21]; // One per level

	public:
		HighScoreMgr();
		virtual ~HighScoreMgr();

		bool					ReadHighScoresData();
		void					SaveHighScoresData();
		void					SyncData(DataSync* theSync);
		void					SyncHighScoreList(DataSync* theSync, HighScoreList& theList);

		void					ClearAllScoreLists();
		void					MakeDefaultHighScores();

		SexyString				GetPerLevelUserName(int theTank, int theLevel);
		HighScoreList*			GetPerLevelScoresList(int theTank, int theLevel);

		void					RecordAdventureHighScore(int theTank, int theLevel, UserProfile* theUser, int theScore);
		void					AddScoreToList(HighScoreList* theVector, SexyString& theUserName, int theScore, int theMaxEntries, bool sortDescending, bool forceReload);
		void					RecordTimeTrialHighScore(int theTank, UserProfile* theUser, int theScore);
		void					RecordChallengeHighScore(int theTank, UserProfile* theUser, int theScore);

		HighScoreList::iterator FindInsertionPos(HighScoreList* theList, int theScore, bool sortDescending);
	};
}

