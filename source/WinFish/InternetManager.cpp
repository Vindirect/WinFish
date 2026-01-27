#include "InternetManager.h"
#include "WinFishApp.h"

using namespace Sexy;

Sexy::InternetManager::InternetManager()
{
	mUpdateTransfer = HTTPTransfer();

	m0x508 = false;
	mUpToDate = true;
	m0x50a = false;
	m0x04 = 4;
}

Sexy::InternetManager::~InternetManager()
{
	CleanAdList();
}

void Sexy::InternetManager::Update()
{
	if (!m0x50a)
		return;

	for (int i = 0;i < MAX_HTTP_TRANSFERS; i++)
	{
		HTTPTransfer* aCurTransf = &mHTTPTransfers[i];

		int aResCode = aCurTransf->GetResultCode();

		bool doNextUrl = false;

		if (aResCode == HTTPTransfer::RESULT_DONE)
		{
			SexyString aRelPath = aCurTransf->mSpecifiedRelURL;

			size_t aSlashPos = aRelPath.find('/');
			while (aSlashPos != std::string::npos)
			{
				SexyString aDirToCreate = aRelPath.substr(0, aSlashPos);
				CreateDirectoryA(aDirToCreate.c_str(), NULL);
				aSlashPos = aRelPath.find('/', aSlashPos + 1);
			}

			OutputDebugStringA(StrFormat("AdFile: %s\r\n", aRelPath.c_str()).c_str());

			FILE* aFile = fopen(aRelPath.c_str(), "wb");
			if (aFile != nullptr)
			{
				SexyString aContent = aCurTransf->GetContent();
				const char* aConstContent = aContent.c_str();
				size_t aContentLength = aCurTransf->mContentLength;

				fwrite(aConstContent, 1, aContentLength, aFile);
				fclose(aFile);
			}

			aCurTransf->Reset();

			doNextUrl = true;
		}
		else if (aResCode != HTTPTransfer::RESULT_NOT_COMPLETED)
		{
			if (aResCode != HTTPTransfer::RESULT_NOT_STARTED)
			{
				SexyString anErrorMsg = StrFormat("Error on AdFile: %s\r\n", aCurTransf->mSpecifiedRelURL.c_str());
				OutputDebugStringA(anErrorMsg.c_str());

				aCurTransf->Reset();
			}

			doNextUrl = true;
		}
		if (doNextUrl)
		{
			if (!mAdUrlList.empty())
			{
				SexyString aNextUrl = mAdUrlList.front();
				mAdUrlList.pop_front();

				aCurTransf->Get(aNextUrl);
			}
		}
	}
}

bool Sexy::InternetManager::CheckForUpdates()
{
	if (m0x508)
		return true;

	int aResCode = mUpdateTransfer.GetResultCode();

	if (aResCode != 0)
		return false;

	m0x50a = true;

	mList1.clear();
	mList2.clear();
	mAdUrlList.clear();

	SexyString aContent = mUpdateTransfer.GetContent();

	int aLineIdx = 0;
	while (!aContent.empty())
	{
		size_t aNewLinePos = aContent.find('\n');
		SexyString aCurLine;

		if (aNewLinePos == std::string::npos) {
			aCurLine = aContent;
			aContent.clear();
		}
		else 
		{
			aCurLine = aContent.substr(0, aNewLinePos);
			aContent.erase(0, aNewLinePos + 1);
		}

		while (!aCurLine.empty() && (aCurLine.back() == '\r' || aCurLine.back() == ' '))
			aCurLine.pop_back();

		if (aCurLine.empty())
			continue;

		if (aLineIdx == 0)
			mUpToDate = (atol(aCurLine.c_str()) != 0);
		else if (aLineIdx == 1)
			mUpdateURL = aCurLine;
		else
			mAdUrlList.push_back(aCurLine);

		aLineIdx++;
	}

	Unk01();
	return true;
}

bool Sexy::InternetManager::IsUpToDate()
{
	bool upToDate = true;
	if (CheckForUpdates())
		upToDate = mUpToDate;
	return upToDate;
}

void Sexy::InternetManager::StartAdLoading()
{
	CheckForUpdates();
	auto& adUrlList = mAdUrlList;
	auto it = adUrlList.begin();

	while (it != adUrlList.end())
	{
		SexyString* currentUrl = &(*it);

		SexyString localFileName;

		localFileName = *currentUrl;

		bool fileExists = gSexyAppBase->FileExists(localFileName);

		if (fileExists)
			it = adUrlList.erase(it);
		else
			++it;
	}
}

void Sexy::InternetManager::StartUpdateCheck(SexyString aReqUrl)
{
	m0x508 = false;
	mUpdateTransfer.Get(aReqUrl);
}

SexyString Sexy::InternetManager::GetUpdateURL()
{
	if (!CheckForUpdates())
		return "";
	return mUpdateURL;
}

int Sexy::InternetManager::GetUpdateResultCode()
{
	int aResCode = mUpdateTransfer.GetResultCode();
	if (aResCode == HTTPTransfer::RESULT_DONE)
		return HTTPTransfer::RESULT_NOT_COMPLETED;

	if (aResCode != HTTPTransfer::RESULT_NOT_STARTED)
	{
		if (aResCode != HTTPTransfer::RESULT_NOT_COMPLETED)
			return HTTPTransfer::RESULT_NOT_FOUND;
		return HTTPTransfer::RESULT_NOT_STARTED;
	}
	return HTTPTransfer::RESULT_DONE;
}

void Sexy::InternetManager::ReadAdList()
{
	// TODO
}

void Sexy::InternetManager::CleanAdList()
{
	// TODO
}

void Sexy::InternetManager::Unk01()
{
	// TODO
}

