#pragma once

#include "MoneyDialog.h"
#include "WinFishApp.h"
#include "SexyAppFramework/ListListener.h"
#include "SexyAppFramework/EditListener.h"
#include "SexyAppFramework/ListWidget.h"

namespace Sexy
{
	class UserDialog : public MoneyDialog, public ButtonListener, public ListListener, public EditListener
	{
	public:
		ListWidget*				mListWidget;
		EditWidget*				mEditWidget;
		ScrollbarWidget*		mScrollbarWidget;
		DialogButton*			mRenameButton;
		DialogButton*			mDeleteButton;
		bool					mTransferShells;

	public:
		UserDialog(WinFishApp* theApp, bool transferShells);
		virtual ~UserDialog();

		virtual void			Draw(Graphics* g);

		virtual void			AddedToManager(WidgetManager* theWidgetManager);
		virtual void			RemovedFromManager(WidgetManager* theWidgetManager);

		virtual void			Resize(int theX, int theY, int theWidth, int theHeight);

		virtual int				GetPreferredHeight(int theWidth);

		virtual void			ButtonPress(int theId);
		virtual void			ButtonDepress(int theId);
		virtual void			ListClicked(int theId, int theIdx, int theClickCount);

		SexyString				GetSelectedUserName();
		int						GetShellsValue();

	};
}
