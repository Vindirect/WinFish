#include "ProfileMgr.h"
#include "WinFishApp.h"
#include "WinFishAppCommon.h"
#include "SexyAppFramework/Common.h"

using namespace Sexy;

const char* gTextAdventure      = "adv";
const char* gTextTimeTrial      = "tim";
const char* gTextSandbox        = "snd";
const char* gTextChallenge      = "sur";
const char* gTextVirtualTank    = "sim";


ProfileMgr::ProfileMgr()
{
	mProfilesMap = new UserProfilesMap();
    mCurrentUserId = 0;
    m0x10 = 1;
    m0x14 = 1;
}

ProfileMgr::~ProfileMgr()
{
    delete mProfilesMap;
}

void Sexy::ProfileMgr::ReadUsersDat()
{
    Buffer aBuf;

    SexyString aPath = GetAppDataFolder() + "userdata/users.dat";
    if (gSexyApp->ReadBufferFromFile(aPath, &aBuf, false))
    {
        DataReader aDR;
        aDR.OpenMemory(aBuf.GetDataPtr(), aBuf.GetDataLen(), false);
        DataSync aDS(aDR);
        SyncUsersDat(aDS);
    }
}

UserProfile* Sexy::ProfileMgr::GetUserProfile(const SexyString& theProfileName)
{
    if (mProfilesMap->empty())
        return NULL;

    UserProfilesMap::iterator anIterator = mProfilesMap->find(theProfileName);

    if (anIterator != mProfilesMap->end())
    {
        anIterator->second.LoadFromMemory();
        anIterator->second.m0x40 = m0x14;
        m0x14++;
        return &anIterator->second;
    }

    return nullptr;
}

UserProfile* Sexy::ProfileMgr::GetFirstUserProfile()
{
    if (mProfilesMap->empty())
        return nullptr;

    UserProfilesMap::iterator anIterator = mProfilesMap->begin();

    UserProfile* aFirstProfile = &anIterator->second;

    aFirstProfile->LoadFromMemory();
    aFirstProfile->m0x40 = m0x14;
    m0x14++;

    return aFirstProfile;
}

void Sexy::ProfileMgr::SyncUsersDat(DataSync& theDataSync)
{
    DataWriter* aDW = theDataSync.mWriter;
    DataReader* aDR = theDataSync.mReader;
    int aDatVersion = gUsersDatVersion;
    theDataSync.SyncLong(aDatVersion);
    theDataSync.mVersion = aDatVersion;
    if (aDatVersion != gUsersDatVersion)
        return;

    if (aDW)
    {
        aDW->WriteShort(mProfilesMap->size());
        for (UserProfilesMap::iterator it = mProfilesMap->begin(); it != mProfilesMap->end(); ++it)
        {
            UserProfile* aProf = &it->second;
            aProf->SyncUsersDat(theDataSync);
        }
    }
    else if (aDR)
    {
        mProfilesMap->clear();
        int aProfileMapSize = aDR->ReadShort();
        if (aProfileMapSize <= 0)
            return;
        int aUnk01 = 0;
        int aUnk02 = 0;
        for (int i = aProfileMapSize; i > 0; i--)
        {
            UserProfile aProf = UserProfile();
            aProf.SyncUsersDat(theDataSync);
            if (aUnk01 < aProf.m0x40)
                aUnk01 = aProf.m0x40;
            if (aUnk02 < aProf.mId)
                aUnk02 = aProf.mId;
            InsertProfile(&aProf);
        }
        m0x10 = aUnk02 + 1;
        m0x14 = aUnk01 + 1;
    }
}

void Sexy::ProfileMgr::SaveUsersDat()
{
    DataWriter aDW;
    aDW.OpenMemory(32);
    DataSync aDS(aDW);
    SyncUsersDat(aDS);

    SexyString aPath = GetAppDataFolder() + "userdata";
    MkDir(aPath);

    aPath = GetAppDataFolder() + "userdata/users.dat";
    gSexyApp->WriteBytesToFile(aPath, aDW.mMemoryHandle, aDW.mMemoryPosition);
}

UserProfile* Sexy::ProfileMgr::InsertProfile(UserProfile* theProfile)
{
    UserProfilesMap::iterator it = mProfilesMap->lower_bound(theProfile->mUserName);

    if (it != mProfilesMap->end() && _stricmp(it->first.c_str(), theProfile->mUserName.c_str()) == 0)
        return nullptr;

    UserProfilesMap::iterator aNewIt = mProfilesMap->insert(it, std::make_pair(theProfile->mUserName, *theProfile));
    return &aNewIt->second;
}

UserProfile* Sexy::ProfileMgr::MakeNewUser(SexyString* theUserName)
{
    UserProfile* aProf = new UserProfile();
    aProf->mUserName = *theUserName;
    if (!(aProf = InsertProfile(aProf)))
        return nullptr;

    aProf->m0x40 = m0x14;
    aProf->mId = m0x10;

    m0x14++;
    m0x10++;

    CapMaxUserCount();

    return aProf;
}

bool Sexy::ProfileMgr::RenameUser(SexyString& theOldUserName, SexyString& theNewUserName)
{
    UserProfilesMap::iterator existingNewIt = mProfilesMap->lower_bound(theNewUserName);
    if (existingNewIt != mProfilesMap->end() && _stricmp(existingNewIt->first.c_str(), theNewUserName.c_str()) == 0)
        return false;

    UserProfilesMap::iterator oldIt = mProfilesMap->lower_bound(theOldUserName);
    if (oldIt == mProfilesMap->end() || _stricmp(oldIt->first.c_str(), theOldUserName.c_str()) != 0)
        return false;

    UserProfile anOldProf = oldIt->second;

    anOldProf.mUserName = theNewUserName;

    mProfilesMap->erase(oldIt);
    InsertProfile(&anOldProf);

    return true;
}

bool Sexy::ProfileMgr::DeleteUser(SexyString& theUserName)
{
    UserProfilesMap::iterator existingNewIt = mProfilesMap->lower_bound(theUserName);
    if (existingNewIt != mProfilesMap->end() && _stricmp(existingNewIt->first.c_str(), theUserName.c_str()) == 0)
    {
        existingNewIt->second.DeleteUserAppData();
        mProfilesMap->erase(existingNewIt);
        return true;
    }

    return false;
}

void Sexy::ProfileMgr::CapMaxUserCount()
{
    while (mProfilesMap->size() > 200)
    {
        UserProfilesMap::iterator oldest_it = mProfilesMap->begin();

        for (UserProfilesMap::iterator it = std::next(mProfilesMap->begin()); it != mProfilesMap->end(); ++it)
            if (it->second.m0x40 < oldest_it->second.m0x40) // m0x70?
                oldest_it = it;

        oldest_it->second.DeleteUserAppData();
        mProfilesMap->erase(oldest_it);
    }
}

void Sexy::UserProfile::DeleteUserAppData()
{
    SexyString aUserDatPath = GetAppDataFolder();
    aUserDatPath += StrFormat("userdata/user%d.dat", mId);

    gSexyAppBase->EraseFile(aUserDatPath);

    SexyString aScrDatPath = GetAppDataFolder();
    aScrDatPath += StrFormat("userdata/scr%d.dat", mId);

    gSexyAppBase->EraseFile(aScrDatPath);

    for (int i = 0; i <= 5; ++i)
    {
        SexyString aSaveGamePath;
        aSaveGamePath = GetSaveGameFilePath(i, mId);
        gSexyAppBase->EraseFile(aSaveGamePath);
    }
}

void Sexy::UserProfile::SaveScreenSaver(int unk)
{
    if (unk <= 0)
        return;

    SexyString aFilePath = GetAppDataFolder() + StrFormat("userdata/scr%d.dat", mId);
    int aVal = ReadScreenSaverData(false);
    DataWriter aDW;
    aDW.OpenMemory(32);
    aDW.WriteLong(m0x88);
    for (int i = 0; i < 50; i++)
        aDW.WriteLong(rand() % 20000);

    aDW.WriteLong(aVal + unk);

    for (int i = 0; i < 50; i++)
        aDW.WriteLong(rand() % 20000);

    SexyString aUserDataPath = GetAppDataFolder() + "userdata";
    MkDir(aUserDataPath);

    ((WinFishApp*)gSexyApp)->WriteBytesToFile(aFilePath, aDW.mMemoryHandle, aDW.mMemoryPosition);
    ((WinFishApp*)gSexyApp)->ClearUpdateBacklog(false);
}

void Sexy::UserProfile::Unk01()
{
    int aVal = ReadScreenSaverData(true);
    
    if (aVal > 100000)
        aVal = 100000;
    else if (aVal <= 0)
        return;

    AddShells(aVal);
    m0x88 = Rand();
    Save();
}

void Sexy::UserProfile::WriteAdventureScore(int theTank, int theLevel, int theScore)
{
    int anIdx = GetLevelIndex(theTank, theLevel);
    if (anIdx < 0)
        return;

    if (theTank == 5 && theLevel == 1)
    {
        if (mAdventureScores[anIdx] < theScore)
            mAdventureScores[anIdx] = theScore;
    }
    else
    {
        if (mAdventureScores[anIdx] > theScore || mAdventureScores[anIdx] == -1)
            mAdventureScores[anIdx] = theScore;
    }
}

void Sexy::UserProfile::WriteTimeTrialScore(int theTank, int theScore)
{
    int anIdx = theTank - 1;
    if (anIdx < 4 && mTimeTrialScores[anIdx] < theScore)
        mTimeTrialScores[anIdx] = theScore;
}

void Sexy::UserProfile::WriteChallengeScore(int theTank, int theScore)
{
    int anIdx = theTank - 1;
    if (anIdx < 4 && (mChallengeScores[anIdx] > theScore || mChallengeScores[anIdx] == -1))
        mChallengeScores[anIdx] = theScore;
}

int Sexy::UserProfile::ReadScreenSaverData(bool doErase)
{
    SexyString aFilePath = GetAppDataFolder() + StrFormat("userdata/scr%d.dat", mId);
    Buffer aBuf;
    bool success = gSexyApp->ReadBufferFromFile(aFilePath, &aBuf, false);
    if (!success)
        return 0;

    if (doErase)
        gSexyApp->EraseFile(aFilePath);

    DataReader aReader;
    aReader.OpenMemory(aBuf.GetDataPtr(), aBuf.GetDataLen(), false);
    DataSync aSync(aReader);

    int aVal = aReader.ReadLong();
    if (aVal != m0x88)
        return 0;

    for (int i = 0; i < 50; i++)
        aReader.ReadLong();

    int resultValue = aReader.ReadLong();

    for (int i = 0; i < 50; i++)
        aReader.ReadLong();

    return resultValue;
}

void Sexy::UserProfile::UpdateStoreData(long long theNewTime)
{
    if (theNewTime != m0xa0)
    {
        m0xa0 = theNewTime;
        memset(mStoreScreenBought, false, 8);
    }
}

int Sexy::UserProfile::GetAdventureScore(int theTank, int theLevel)
{
    int anIdx = GetLevelIndex(theTank, theLevel);
    if (anIdx < 0)
        return -1;
    return mAdventureScores[anIdx];
}

int Sexy::UserProfile::GetTimeTrialScore(int theTank)
{
    if (theTank < 5)
        return mTimeTrialScores[theTank-1];
    return -1;
}

int Sexy::UserProfile::GetChallengeScore(int theTank)
{
    if (theTank < 5)
        return mChallengeScores[theTank-1];
    return -1;
}


Sexy::UserProfile::UserProfile()
{
    Init();
}

void Sexy::UserProfile::LoadFromMemory()
{
    Buffer aBuf;

    SexyString aPath = GetAppDataFolder() + StrFormat("userdata/user%d.dat", mId);
    if (gSexyApp->ReadBufferFromFile(aPath, &aBuf, false))
    {
        DataReader aDR;
        aDR.OpenMemory(aBuf.GetDataPtr(), aBuf.GetDataLen(), false);
        DataSync aDS(aDR);
        SyncData(aDS);
        SetCheatFlag(6, false);
    }
}

void Sexy::UserProfile::Init()
{
    mTank = 1;
    mLevel = 1;
    mNumOfUnlockedPets = 0;
    m0x4c = 0;
    mCyraxNum = 0;
    mFinishedGameCount = 0;
    m0xb4 = 3;
    mCheatCodeFlags = 0;
    SetCheatFlag(1, true);
    m0x8c = 0;
    mBubbulatorBought = 0;
    mAlienAttractorBought = 0;
    m0x98 = Rand();
    m0x88 = Rand();
    m0x78[0] = 0;
    m0x78[1] = 0;
    m0x78[2] = 0;
    m0x78[3] = 0;
    m0x7c = 0;
    m0x80 = 0;
    for (int i = 0; i < GameObject::PET_END; i++)
    {
        mUnlockedPets[i] = false;
        m0x5a[i] = false;
    }
    for (int i = 0; i < 6; i++)
        mUnlockedBackgrounds[i] = false;
    m0xa0 = 0;
    mUnlockedBackgrounds[0] = 1;
    for (int i = 0; i < 8; i++)
        mStoreScreenBought[i] = false;
    mBonusItemId = 0;
    mShells = 200;
    for (int i = 0; i < 21; i++)
        mAdventureScores[i] = -1;
    for (int i = 0; i < 4; i++)
    {
        mTimeTrialScores[i] = -1;
        mChallengeScores[i] = -1;
    }
    mFinishedGame = false;
    m0x58 = false;
}

void Sexy::UserProfile::SetCheatFlag(char thePos, bool theFlag)
{
    unsigned int mask = 1 << thePos;
    if (theFlag)
        mCheatCodeFlags |= mask;
    else
        mCheatCodeFlags &= ~mask;
}

bool Sexy::UserProfile::ToggleCheatFlag(char theIdx)
{
    unsigned int aMask = 1 << theIdx;
    mCheatCodeFlags ^= aMask;
    return (mCheatCodeFlags & aMask) != 0;
}

void Sexy::UserProfile::SyncUsersDat(DataSync& theDataSync)
{
    theDataSync.SyncString(mUserName);
    theDataSync.SyncLong(m0x40);
    theDataSync.SyncLong(mId);
}

void Sexy::UserProfile::Save()
{
    DataWriter aDW;
    aDW.OpenMemory(32);
    DataSync aDS(aDW);
    SyncData(aDS);

    SexyString aPath = GetAppDataFolder() + "userdata";
    MkDir(aPath);

    aPath = GetAppDataFolder() + StrFormat("userdata/user%d.dat", mId);
    ((WinFishApp*)gSexyApp)->WriteBytesToFile(aPath, aDW.mMemoryHandle, aDW.mMemoryPosition);
}

void Sexy::UserProfile::SyncData(DataSync& theDataSync)
{
    DataReader* theReader = theDataSync.mReader;
    if (theReader)
        Init();

    uint aVer = 0x80000000;
    theDataSync.SyncLong(aVer);

    bool unkFlag = false;

    if (aVer < 0x7fffffff && theReader)
    {
        unkFlag = true;
        theReader->RollbackBytes(4);
    }

    theDataSync.SyncLong(mShells);
    theDataSync.SyncLong(mTank);
    theDataSync.SyncLong(mLevel);
    theDataSync.SyncBool(mFinishedGame);
    theDataSync.SyncBool(m0x58);
    theDataSync.SyncLong(mBubbulatorBought);
    theDataSync.SyncLong(mAlienAttractorBought);
    theDataSync.SyncLong(m0x8c);
    theDataSync.SyncLong(m0x4c);
    theDataSync.SyncLong(mCyraxNum);
    theDataSync.SyncLong(mFinishedGameCount);
    if(theReader)
        m0xa0 = 0;
    theDataSync.SyncLong((int&)m0xa0);
    theDataSync.SyncLong(m0x98);
    theDataSync.SyncLong(mBonusItemId);
    theDataSync.SyncLong(m0xb4);

    for (int i = 0; i < GameObject::PET_END; i++)
        theDataSync.SyncBool(mUnlockedPets[i]);
    for (int i = 0; i < 6; i++)
        theDataSync.SyncBool(mUnlockedBackgrounds[i]);
    for (int i = 0; i < 8; i++)
        theDataSync.SyncBool(mStoreScreenBought[i]);

    for (int i = 0; i < 4; i++)
    {
        if (unkFlag)
        {
            int aLoad = m0x78[i];
            theDataSync.SyncLong(aLoad);
            m0x78[i] = aLoad != 0;
        }
        else
            theDataSync.SyncBool(m0x78[i]);

        theDataSync.SyncLong(mChallengeScores[i]);
        theDataSync.SyncLong(mTimeTrialScores[i]);
    }

    if (!unkFlag)
    {
        theDataSync.SyncLong(m0x7c);
        theDataSync.SyncLong(m0x80);
        theDataSync.SyncLong(mCheatCodeFlags);
    }

    for (int i = 0; i < 21; i++)
    {
        theDataSync.SyncLong(mAdventureScores[i]);
    }

    if (aVer > 0x7fffffff)
        theDataSync.SyncLong(m0x88);

    if (theReader)
    {
        int anArrLvl = mTank * 5 - 6 + mLevel;
        if (mTank > 4)
            anArrLvl--;
        if (mFinishedGame)
            anArrLvl = 20;
        for (int i = 0; i < anArrLvl; i++)
            UnlockPet(i, true);

        mNumOfUnlockedPets = 0;
        for (int i = 0; i < GameObject::PET_END; i++)
            if (mUnlockedPets[i])
                mNumOfUnlockedPets++;
    }
}

void Sexy::UserProfile::UnlockPet(int thePetId, bool unlock)
{
    if (mUnlockedPets[thePetId] != unlock)
    {
        mUnlockedPets[thePetId] = unlock;
        if (!unlock)
            mNumOfUnlockedPets--;
        else
            mNumOfUnlockedPets++;
    }
}

void Sexy::UserProfile::AddShells(int theValue)
{
    mShells += theValue;
    if (mShells > 9999999)
        mShells = 9999999;
    else if (mShells < 0)
        mShells = 0;
}

bool Sexy::UserProfile::IsPetUnlocked(int idx)
{
    return mUnlockedPets[idx];
}

void Sexy::UserProfile::NextLevel()
{
    mLevel++;
    if (mTank == 4 && mLevel == 6)
    {
        mTank = 5;
        mLevel = 1;
    }
    if (mTank == 5 && mLevel > 1)
    {
        mCyraxNum++;
        mFinishedGame = true;
    }
    if (mLevel > 6 || (mFinishedGame && mLevel > 5))
    {
        mTank++;
        mLevel = 1;
    }
    m0x58 = true;
}



SexyString Sexy::UserProfile::GetSaveGameFilePath(int theGameMode, int theUserId)
{
    const char* aGameModeString;
    switch (theGameMode)
    {
    case WinFishApp::GAMEMODE_ADVENTURE:    aGameModeString = gTextAdventure;       break;
    case WinFishApp::GAMEMODE_TIME_TRIAL:   aGameModeString = gTextTimeTrial;       break;
    case WinFishApp::GAMEMODE_SANDBOX:      aGameModeString = gTextSandbox;         break;
    case WinFishApp::GAMEMODE_CHALLENGE:    aGameModeString = gTextChallenge;       break;
    case WinFishApp::GAMEMODE_VIRTUAL_TANK: aGameModeString = gTextVirtualTank;     break;
    default:                                aGameModeString = gTextAdventure;       break;
    }

    char aPathBuffer[256];

    sprintf(aPathBuffer, "userdata\\%s%d.dat", aGameModeString, theUserId);

    return SexyString(GetAppDataFolder() + aPathBuffer);
}
