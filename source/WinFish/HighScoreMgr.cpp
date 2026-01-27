#include "HighScoreMgr.h"
#include "ProfileMgr.h"
#include "WinFishAppCommon.h"
#include "WinFishApp.h"
#include "DataSync.h"

using namespace Sexy;

Sexy::HighScoreMgr::HighScoreMgr()
{

}

Sexy::HighScoreMgr::~HighScoreMgr()
{
}

bool Sexy::HighScoreMgr::ReadHighScoresData()
{
    ClearAllScoreLists();
    Buffer aBuf;
    SexyString aPath = GetAppDataFolder() + "userdata/highscores.dat";
    if (gSexyApp->ReadBufferFromFile(aPath, &aBuf, false))
    {
        DataReader aDR;
        aDR.OpenMemory(aBuf.GetDataPtr(), aBuf.GetDataLen(), false);
        DataSync aDS(aDR);
        SyncData(&aDS);
    }
    else
        MakeDefaultHighScores();
    return false;
}

void Sexy::HighScoreMgr::SaveHighScoresData()
{
    DataWriter aDW;
    aDW.OpenMemory(32);
    DataSync aDS(aDW);
    SyncData(&aDS);
    SexyString aDirPath = GetAppDataFolder() + "userdata";
    MkDir(aDirPath);

    aDirPath = GetAppDataFolder() + "userdata/highscores.dat";
    gSexyApp->WriteBytesToFile(aDirPath, aDW.mMemoryHandle, aDW.mMemoryPosition);
}

void Sexy::HighScoreMgr::SyncData(DataSync* theSync)
{
    int aVer = 2;
    theSync->SyncLong(aVer);
    theSync->mVersion = aVer;
    if (theSync->mReader != nullptr)
    {
        ClearAllScoreLists();
        if (aVer != 2)
        {
            MakeDefaultHighScores();
            return;
        }
    }

    for (int i = 0; i < 4; i++)
    {
        SyncHighScoreList(theSync, mTimeTrialScores[i]);
        SyncHighScoreList(theSync, mChallengeScores[i]);
    }

    for (int i = 0; i < 21; i++)
        SyncHighScoreList(theSync, mAdventureScores[i]);
}

void Sexy::HighScoreMgr::SyncHighScoreList(DataSync* theSync, HighScoreList& theList)
{
    if (theSync->mReader == nullptr)
    {
        theSync->mWriter->WriteShort(theList.size());
        HighScoreList::iterator it = theList.begin();
        while (it != theList.end())
        {
            it->Sync(theSync);
            it++;
        }
    }
    else
    {
        theList.clear();
        short aSize = theSync->mReader->ReadShort();
        for (int i = 0; i < aSize; i++)
        {
            HighScoreEntry anEntry;
            anEntry.Sync(theSync);

            theList.push_back(anEntry);
        }
    }
}

void Sexy::HighScoreMgr::ClearAllScoreLists()
{
    for (int i = 0; i < 4; i++)
    {
        mTimeTrialScores[i].clear();
        mChallengeScores[i].clear();
    }
    for (int i = 0; i < 21; i++)
    {
        mAdventureScores[i].clear();
    }
}

void Sexy::HighScoreMgr::MakeDefaultHighScores()
{
    ClearAllScoreLists();

    static const HighScoreEntry gTimeTrialDefaults[] = {
        {"Amy", 1000}, {"Ben", 900}, {"Brenna", 800}, {"Brian", 700}, {"Chad", 600},
        {"Damon", 1000}, {"Dave", 900}, {"Don", 800}, {"Eric", 700}, {"Greg", 600},
        {"Hans", 1000}, {"Jason", 900}, {"Jeff", 800}, {"John", 700}, {"Josh", 600},
        {"Juho", 1000}, {"Kathy", 900}, {"Katrina", 800}, {"Mark", 700}, {"Nick", 600}
    };

    static const HighScoreEntry gChalAdvDefaults[] = {
        {"Shawn", 3300}, {"Sukhbir", 3360}, {"Tysen", 3420}, {"Walter", 3480}, {"jb63", 3540},
        {"MrsDbolt", 3300}, {"Lexi", 3360}, {"GrukX", 3420}, {"cheryl", 3480}, {"Rambobear", 3540},
        {"Kingdom", 3300}, {"Inkmei", 3360}, {"wolfpackmama", 3420}, {"hottentots", 3480}, {"Dyne", 3540},
        {"monkeyboy", 3300}, {"lilith", 3360}, {"HRACH", 3420}, {"Lovedog", 3480}, {"Syrinx", 3540}
    };

    const HighScoreEntry* aCurData = nullptr;
    HighScoreList* aCurList = nullptr;

    for (int i = 0; i < 2; i++)
    {
        if (i == 0) 
        {
            aCurData = gTimeTrialDefaults;
            aCurList = mTimeTrialScores;
        }
        else 
        {
            aCurData = gChalAdvDefaults;
            aCurList = mChallengeScores;
        }

        int aDataIndex = 0;

        for (int tankIdx = 0; tankIdx < 4; tankIdx++)
        {
            for (int i = 0; i < 5; i++)
            {
                HighScoreEntry newEntry;
                newEntry.mUserName = aCurData[aDataIndex].mUserName;
                newEntry.mScore = aCurData[aDataIndex].mScore;

                aCurList[tankIdx].push_back(newEntry);

                aDataIndex++;
            }
        }
    }

    static const HighScoreEntry gAdventureDefaults[] = {
        {"HomerJay", 3300}, {"Zelda", 3360}, {"lala", 3420}, {"DixieWriter", 3480}, {"samspade", 3540},
        {"baasi1", 3300}, {"DaveDude", 3360}, {"Bubgrl", 3420}, {"Illyria", 3480}, {"hermitcrab", 3540},
        {"kane", 3300}, {"ClaireBear", 3360}, {"wuggies", 3420}, {"jebksb1977", 3480}, {"firedog", 3540},
        {"WoodRat", 3300}, {"Meatboy", 3360}, {"Splam11", 3420}, {"WendigoWolf", 3480}, {"bugsymcd24", 3540}
    };

    for (int i = 0; i < 20; i++)
    {
        HighScoreEntry newEntry;
        newEntry.mUserName = gAdventureDefaults[i].mUserName;
        newEntry.mScore = gAdventureDefaults[i].mScore;

        mAdventureScores[i].push_back(newEntry);
    }
}

SexyString Sexy::HighScoreMgr::GetPerLevelUserName(int theTank, int theLevel)
{
    static SexyString sEmptyString;
    HighScoreList* aList = GetPerLevelScoresList(theTank, theLevel);
    if (aList == nullptr || aList->empty())
        return sEmptyString;

    return aList->front().mUserName;
}

HighScoreList* Sexy::HighScoreMgr::GetPerLevelScoresList(int theTank, int theLevel)
{
    static HighScoreList sEmptyList;
    int anIdx = GetLevelIndex(theTank, theLevel);
    if(anIdx < 0)
        return &sEmptyList;
    return &mAdventureScores[anIdx];
}

void Sexy::HighScoreMgr::RecordAdventureHighScore(int theTank, int theLevel, UserProfile* theUser, int theScore)
{
    int anIdx = GetLevelIndex(theTank, theLevel);
    if (anIdx < 0)
        return;

    int theMaxEntries = 1;
    bool sortDescending = false;
    if (theTank == 5)
    {
        sortDescending = true;
        theMaxEntries = 3;
    }
    theUser->WriteAdventureScore(theTank, theLevel, theScore);
    AddScoreToList(&mAdventureScores[anIdx], theUser->mUserName, theScore, theMaxEntries, sortDescending, true);
}

void Sexy::HighScoreMgr::AddScoreToList(HighScoreList* theList, SexyString& theUserName, int theScore, int theMaxEntries, bool sortDescending, bool forceReload)
{
    while (theList->size() > theMaxEntries)
        theList->pop_back();

    HighScoreList::iterator aPos = FindInsertionPos(theList, theScore, sortDescending);

    if (aPos != theList->end() || theList->size() < theMaxEntries) {

        if (forceReload) 
        {
            ReadHighScoresData();
            AddScoreToList(theList, theUserName, theScore, theMaxEntries, sortDescending, false);
        }
        else 
        {
            HighScoreEntry newEntry;
            newEntry.mUserName = theUserName;
            newEntry.mScore = theScore;

            theList->insert(aPos, newEntry);

            while (theList->size() > theMaxEntries) 
                theList->pop_back();

            SaveHighScoresData();
        }
    }
}

void Sexy::HighScoreMgr::RecordTimeTrialHighScore(int theTank, UserProfile* theUser, int theScore)
{
    int anIdx = theTank - 1;
    if (anIdx < 4)
    {
        theUser->WriteTimeTrialScore(theTank, theScore);
        AddScoreToList(&mTimeTrialScores[anIdx], theUser->mUserName, theScore, 5, 1, true);
    }
}

void Sexy::HighScoreMgr::RecordChallengeHighScore(int theTank, UserProfile* theUser, int theScore)
{
    int anIdx = theTank - 1;
    if (anIdx < 4)
    {
        theUser->WriteChallengeScore(theTank, theScore);
        AddScoreToList(&mChallengeScores[anIdx], theUser->mUserName, theScore, 5, false, true);
    }
}

HighScoreList::iterator Sexy::HighScoreMgr::FindInsertionPos(HighScoreList* theList, int theScore, bool sortDescending)
{
    HighScoreList::iterator it = theList->begin();

    while (it != theList->end()) 
    {
        int currentScore = it->mScore; 

        if (!sortDescending) 
        {
            if (theScore < currentScore)
                return it;
        }
        else 
        {
            if (currentScore < theScore)
                return it;
        }

        it++;
    }

    return it;
}
