#pragma once

#include "MoneyDialog.h"
#include "WinFishApp.h"
#include "SexyAppFramework/EditListener.h"
#include "SexyAppFramework/EditWidget.h"

namespace Sexy
{
	class NewUserDialog : public MoneyDialog, public ButtonListener, public EditListener
	{
	public:
		WinFishApp*				mApp2;
		EditWidget*				mEditWidget;

	public:
		NewUserDialog(WinFishApp* theApp, bool theRename);
		virtual ~NewUserDialog();

		virtual void			Draw(Graphics* g);

		virtual void			AddedToManager(WidgetManager* theWidgetManager);
		virtual void			RemovedFromManager(WidgetManager* theWidgetManager);

		virtual int				GetPreferredHeight(int theWidth);

		virtual void			Resize(int theX, int theY, int theWidth, int theHeight);

		virtual void			EditWidgetText(int theId, const SexyString& theString);

		virtual bool			AllowChar(int theId, SexyChar theChar);

		SexyString				CleanString();

	};
}

