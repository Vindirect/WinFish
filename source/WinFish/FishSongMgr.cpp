#include "FishSongMgr.h"
#include "WinFishApp.h"
#include "SexyAppFramework/Common.h"
#include <SexyAppFramework/SoundManager.h>
#include <SexyAppFramework/SoundInstance.h>

using namespace Sexy;

FishSongMgr::FishSongMgr()
{
	mSongList.clear();
	mTone = -1;
	mToneHi = -1;
	mToneSuperHi = -1;
	mToneLo = -1;
	mCounter = 0;
	mLastUpdateTime = 0;
}

FishSongMgr::~FishSongMgr()
{
}

void FishSongMgr::Update()
{
    DWORD aCurrentTime = GetTickCount();
    if (aCurrentTime == mLastUpdateTime)
        return;
    mLastUpdateTime = aCurrentTime;

    int aFinishedSongSoundID = -1;

    FishSongList::iterator anIterator = mSongList.begin();

    while (anIterator != mSongList.end())
    {
        FishSong* aCurrentSong = *anIterator;

        bool isStillPlaying = aCurrentSong->Update(aCurrentTime, true);

        if (!isStillPlaying)
        {
            aFinishedSongSoundID = aCurrentSong->mApplause;

            anIterator = mSongList.erase(anIterator);
        }
        else
        {
            ++anIterator;
        }
    }

    if (aFinishedSongSoundID != -1)
    {
        FishSong aSong;
        NoteData aNote;
        aNote.mPitch = -10000;
        aNote.mVolume = 1.0;
        aNote.mDuration = 400;
        aSong.mNoteDataVector.push_back(aNote);

        aNote = NoteData();
        aNote.mPitch = 0;
        aNote.mVolume = 1.0;
        aNote.mDuration = 100;
        aSong.mNoteDataVector.push_back(aNote);
        AddSong(&aSong);
    }
}

void Sexy::FishSongMgr::AddSong(FishSong* theSong)
{
    mSongList.push_back(theSong);
    if (theSong->mTone == -1)
        theSong->mTone = mTone;
    if (theSong->mToneHi == -1)
        theSong->mToneHi = mToneHi;
    if (theSong->mToneSuperHi == -1)
        theSong->mToneSuperHi = mToneSuperHi;
    if (theSong->mToneLo == -1)
        theSong->mToneLo = mToneLo;
    theSong->Init(1.0f, 0);
}

void Sexy::FishSongMgr::StopFishSong(int theSongId)
{
    for (FishSongList::iterator it = mSongList.begin(); it != mSongList.end();)
    {
        FishSong* aCurSong = *it;

        if (aCurSong->mSongId == theSongId)
            mSongList.erase(it);
        else
            ++it;
    }
}

bool Sexy::FishSongMgr::IsSongInList(int theSongId)
{
    for (FishSongList::iterator it = mSongList.begin(); it != mSongList.end(); ++it)
    {
        FishSong* aCurSong = *it;

        if (aCurSong->mSongId == theSongId)
            return true;
    }

    return false;
}

void Sexy::FishSongMgr::PausedGameDelaySong()
{
    DWORD aCurTick = GetTickCount();

    for (FishSongList::iterator it = mSongList.begin(); it != mSongList.end(); ++it)
    {
        FishSong* aCurSong = *it;
        aCurSong->mStartTime = aCurSong->mStartTime + (aCurTick - mLastUpdateTime);
    }
}

FishSong* Sexy::FishSongMgr::PlayFishSong(FishSongData* theSongData, int theSongId)
{
    FishSong* aLongestSong = nullptr;
    int aLongestDur = 0;

    for (int i = 0; i < 3; i++)
    {
        if (theSongData->mNoteVectors[i].empty())
            continue;

        FishSong* aFishSong = new FishSong();

        mSongList.push_back(aFishSong);

        aFishSong->mSongId = theSongId;

        aFishSong->mNoteDataVector = theSongData->mNoteVectors[i];

        aFishSong->mTone = mTone;
        aFishSong->mToneHi = mToneHi;
        aFishSong->mToneSuperHi = mToneSuperHi;
        aFishSong->mToneLo = mToneLo;
        aFishSong->Init(theSongData->mSpeed, theSongData->mGlobalPitchShift);

        if (aLongestSong == nullptr || aFishSong->mTotalDurationInSamples > aLongestDur)
        {
            aLongestSong = aFishSong;
            aLongestDur = aFishSong->mTotalDurationInSamples;
        }
    }

    return aLongestSong;
}

int Sexy::FishSongMgr::Unk01(int theSongId)
{
    FishSongList::iterator anIterator = mSongList.begin();

    int aVal = 0;
    while (anIterator != mSongList.end())
    {
        FishSong* aCurrentSong = *anIterator;

        if (aCurrentSong->mSongId == theSongId && aVal < mLastUpdateTime - aCurrentSong->mStartTime)
            aVal = mLastUpdateTime - aCurrentSong->mStartTime;

        anIterator++;
    }
    return aVal;
}

void Sexy::FishSongMgr::ClearSongs()
{
    mSongList.clear();
}

Sexy::FishSong::FishSong()
{
    mCurrentNoteIndex = 0;
    mStartTime = 0;
    mTotalDurationInSamples = 0;
    mTone = -1;
    mToneHi = -1;
    mToneSuperHi = -1;
    mToneLo = -1;
    mApplause = -1;
    mSongId = -1;
}

Sexy::FishSong::~FishSong()
{
}

void Sexy::FishSong::Init(float theSpeed, int thePitchShift)
{
    float aTotalDuration = 0.0;

    for (int i = 0; i < mNoteDataVector.size(); i++)
    {
        NoteData& aNote = mNoteDataVector[i];
        aNote.mPitch += thePitchShift;
        float aThisDur = aNote.mDuration;
        if (aNote.mDuration < 0)
            aThisDur += 4.2949673e+09;

        aNote.mDuration = (int)round(aTotalDuration);
        aThisDur *= theSpeed;
        aTotalDuration += aThisDur;
    }
    mTotalDurationInSamples = (int)round(aTotalDuration);
    mStartTime = GetTickCount();
}

bool Sexy::FishSong::Update(DWORD theCurrentMusicTick, bool theAllowSound)
{
    unsigned long aTick = theCurrentMusicTick - mStartTime;
    
    if (mCurrentNoteIndex < mNoteDataVector.size())
    {
        while (mCurrentNoteIndex < mNoteDataVector.size())
        {
            NoteData& aNote = mNoteDataVector[mCurrentNoteIndex];

            if (aNote.mDuration > aTick) break;

            if (aNote.mPitch > -1000.0)
            {
                int aSoundId = mTone;
                int aPitch = 0;
                if (aNote.mPitch > 24.0)
                {
                    aPitch = -24;
                    aSoundId = mToneSuperHi;
                }
                else if (aNote.mPitch >= 12.0)
                {
                    aPitch = -12;
                    aSoundId = mToneHi;
                }
                else if (aNote.mPitch <= -12.0)
                {
                    aPitch = 12;
                    aSoundId = mToneLo;
                }

                SoundInstance* anInst = ((WinFishApp*)gSexyApp)->mSoundManager->GetSoundInstance(aSoundId);
                if (theAllowSound && anInst)
                {
                    anInst->SetVolume(aNote.mVolume);
                    anInst->AdjustPitch(aPitch + aNote.mPitch);
                    anInst->Play(false, true);
                }
            }

            mCurrentNoteIndex++;
        }
    }

    if (mNoteDataVector.size() > 0)
        return mCurrentNoteIndex < mNoteDataVector.size();
    return mCurrentNoteIndex < 0;
}

FishSongData::FishSongData()
{
    Init();
}

Sexy::FishSongData::~FishSongData()
{
}

void Sexy::FishSongData::Init()
{
    mSpeed = 3;
    mGlobalPitchShift = 0;
    for (int i = 0; i < 3; i++)
    {
        mNoteVectors[i].clear();

        mLineVolumes[i] = 1.0f;
        mLinePitchShifts[i] = 0;
    }
}

bool Sexy::FishSongData::Parse(SexyString& theFilePath)
{
    Init();
    gFishSongParseError = "";
    mSkip = false;
    FILE* aFile = fopen(theFilePath.c_str(), "r");
    if (aFile == nullptr)
    {
        gFishSongParseError = StrFormat("File not found: %s", theFilePath.c_str());
        return false;
    }

    mCurrentLine = 0;

    int aCurLineNum = 0;
    char aLineBuffer[8192];

    while (fgets(aLineBuffer, sizeof(aLineBuffer), aFile) && gFishSongParseError.empty())
    {
        aCurLineNum++;

        if (strlen(aLineBuffer) == 0 || aLineBuffer[0] == '#')
            continue;

        char* newlineptr = strchr(aLineBuffer, '\n');
        if (newlineptr != nullptr)
            *newlineptr = '\0';

        if (strlen(aLineBuffer) == 0)
            continue;

        if (aLineBuffer[0] == '*')
            ParseCommand(aLineBuffer);
        else if(!mSkip)
        {
            SexyChar* token = strtok(aLineBuffer, ",");
            while (token != nullptr)
            {
                SexyString aNoteDataStr = _S(Trim(token));
                if (strlen(aNoteDataStr.c_str()) <= 0) 
                    break;

                NoteData aNewNote;
                aNewNote.Parse(aNoteDataStr.c_str());

                mNoteVectors[mCurrentLine].push_back(aNewNote);

                NoteData& aNoteInVector = mNoteVectors[mCurrentLine].back();

                double aPitch = mGlobalPitchShift + mLinePitchShifts[mCurrentLine];
                aNoteInVector.mPitch += aPitch;

                float aDur = aNoteInVector.mDuration;
                if (aNoteInVector.mDuration < 0)
                    aDur += 4.2949673e+09;

                aNoteInVector.mDuration = round(mSpeed * aDur);
                aNoteInVector.mVolume = mLineVolumes[mCurrentLine];

                if (!gFishSongParseError.empty())
                    break;

                token = strtok(NULL, ",");
            }
        }
    }

    fclose(aFile);

    if (gFishSongParseError.empty())
    {
        mGlobalPitchShift = 0;
        mSpeed = 1;
        return true;
    }
    SexyString anError = StrFormat("%s: %s line %d", gFishSongParseError.c_str(), theFilePath.c_str(), aCurLineNum);
    gFishSongParseError = anError;
    gSexyApp->Popup(gFishSongParseError);
    return false;
}

void Sexy::FishSongData::ParseCommand(const SexyChar* theCommand)
{
    char aCommandKey[512];
    char aCommandValue[512];

    if (sscanf(theCommand + 1, "%s%s", aCommandKey, aCommandValue) == 2)
    {
        if (stricmp(aCommandKey, "skip") == 0)
            mSkip = stricmp(aCommandValue, "true") == 0;
        else if (!mSkip)
        {
            if (stricmp(aCommandKey, "line") == 0)
            {
                int aCurLineVal = atoi(aCommandValue) - 1;
                if (aCurLineVal < 3 && aCurLineVal >= 0)
                    mCurrentLine = aCurLineVal;
                else
                    gFishSongParseError = "Invalid line";
            }
            else if (stricmp(aCommandKey, "rest") == 0)
            {
                int aRestval = atoi(aCommandValue) - 1;
                if (aRestval < 3 && aRestval >= 0)
                {
                    int aCurLineDur = GetLineDuration(mCurrentLine);
                    int aRestLineDur = GetLineDuration(aRestval);

                    if (aCurLineDur < aRestLineDur)
                    {
                        NoteData aNewNote;
                        aNewNote.mDuration = aRestLineDur - aCurLineDur;
                        aNewNote.mPitch = -10000;
                        aNewNote.mVolume = 1.0;
                        mNoteVectors[mCurrentLine].push_back(aNewNote);
                    }
                }
            }
            else if (stricmp(aCommandKey, "attrib") == 0)
            {
                const char* anEq = strchr(theCommand, '=');

                if (anEq)
                    mProperties[Trim(aCommandValue)] = Trim(anEq + 1);
                else
                    mProperties[Trim(aCommandValue)] = "";
            }
            else if (stricmp(aCommandKey, "speed") == 0)
            {
                mSpeed = atof(aCommandValue);
            }
            else if (stricmp(aCommandKey, "volume") == 0)
            {
                mLineVolumes[mCurrentLine] = atof(aCommandValue);
            }
            else if (stricmp(aCommandKey, "shift") == 0)
            {
                mGlobalPitchShift = atol(aCommandValue);
            }
            else if (stricmp(aCommandKey, "localshift") == 0)
            {
                mLinePitchShifts[mCurrentLine] = atol(aCommandValue);
            }
            else
                gFishSongParseError = "Unrecognized command";
        }
    }
    else
    {
        if (stricmp(aCommandKey, "on") == 0)
        {
            mSkip = false;
        }
        else if (stricmp(aCommandKey, "off") == 0)
        {
            mSkip = true;
        }
        else
            gFishSongParseError = "Unrecognized command";
    }
}

bool Sexy::FishSongData::GetProperty(const SexyString& key, SexyString* outValue)
{
    auto& propertiesMap = mProperties;

    auto it = propertiesMap.find(key);

    if (it == propertiesMap.end())
        return false;

    if (outValue != nullptr)
        *outValue = it->second;

    return true;
}

int Sexy::FishSongData::GetLineDuration(int theLine)
{
    int aDur = 0;
    for (int i = 0; i < mNoteVectors[theLine].size(); i++)
        aDur += mNoteVectors[theLine][i].mDuration;

    return aDur;
}

void Sexy::NoteData::Parse(const SexyChar* theNoteInfo)
{
    mPitch = 0;
    mVolume = 1.0;
    mDuration = 0;

    char aStr1[16];
    char aDurStr[256];
    char aStr3[256];

    int aVal = sscanf(theNoteInfo, "%s%s%s", aStr1, aDurStr, aStr3);
    if (aVal != 2)
    {
        gFishSongParseError = "Invalid note";
        if (aVal == 3)
            gFishSongParseError = "Invalid note (missing comma ?)";
        return;
    }

    int aPitch = 0;
    switch (tolower(aStr1[0]))
    {
    case 'a':
        aPitch = 9;
        break;
    case 'b':
        aPitch = 11;
        break;
    case 'c':
        aPitch = 0;
        break;
    case 'd':
        aPitch = 2;
        break;
    case 'e':
        aPitch = 4;
        break;
    case 'f':
        aPitch = 5;
        break;
    case 'g':
        aPitch = 7;
        break;
    case 'r':
        aPitch = -10000;
        break;
    default:
        gFishSongParseError = "Invalid note";
        return;
    }

    int aPitchMod = 4;
    if (aStr1[1] != '\0')
    {
        bool skipSomething = true;
        if (!isdigit(aStr1[1]))
        {
            if (aStr1[1] == '#')
                aPitch++;
            else
            {
                if (aStr1[1] != 'b') return;
                aPitch--;
            }
            skipSomething = isdigit(aStr1[2]);
            aStr1[1] = aStr1[2];
        }

        if (skipSomething)
            aPitchMod = aStr1[1] - '0';
    }

    if (!isdigit(aDurStr[0]))
    {
        const char* pos = aDurStr;
        while (*pos != '\0')
        {
            int aBaseDur;
            switch (tolower(*pos))
            {
            case 'w':
                aBaseDur = 192;
                break;
            case 'h':
                aBaseDur = 96;
                break;
            case 'q':
                aBaseDur = 48;
                break;
            case 'e':
                aBaseDur = 24;
                break;
            case 's':
                aBaseDur = 12;
                break;
            case 't':
                aBaseDur = 6;
                break;
            case 'z':
                aBaseDur = 3;
                break;
            default:
                gFishSongParseError = "Invalid duration";
                return;
            }
            pos++;

            if (tolower(*pos) == 't')
            {
                aBaseDur = (aBaseDur * 2) / 3;
                pos++;
            }

            int aCurNoteDur = aBaseDur;

            while (tolower(*pos) == 'd')
            {
                pos++;
                aBaseDur /= 2;
                aCurNoteDur += aBaseDur;
            }

            mDuration += aCurNoteDur;

            if (*pos == '+')
                pos++;
            else if (*pos != '\0')
            {
                gFishSongParseError = "Invalid note (missing comma?)";
                return;
            }
            else
                break;
        }
    }
    else
    {
        mDuration = atol(aDurStr);
    }

    mPitch = aPitch + (aPitchMod * 3 - 12) * 4;
}
