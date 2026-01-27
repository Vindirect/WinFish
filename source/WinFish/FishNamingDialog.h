#pragma once
#include "MoneyDialog.h"
#include "SexyAppFramework/EditListener.h"
#include "SexyAppFramework/EditWidget.h"
namespace Sexy
{
	class FishNamingDialog : public MoneyDialog, public EditListener
	{
	public:
		WinFishApp* mApp2;
		EditWidget* mEditWidget;
		int m0x164;
		bool m0x168;

	public:
		FishNamingDialog(WinFishApp* theApp, const SexyString& theLine);
		virtual ~FishNamingDialog();

		virtual void			AddedToManager(WidgetManager* theWidgetManager);
		virtual void			RemovedFromManager(WidgetManager* theWidgetManager);
		virtual void			Draw(Graphics* g);
		virtual void			Resize(int theX, int theY, int theWidth, int theHeight);
		virtual int				GetPreferredHeight(int theWidth);

		virtual void			EditWidgetText(int theId, const SexyString& theString);
		virtual bool			AllowChar(int theId, SexyChar theChar);
	};
}

