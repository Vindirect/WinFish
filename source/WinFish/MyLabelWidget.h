#pragma once

#include "SexyAppFramework/Widget.h"

namespace Sexy
{
	class MyLabelWidget : public Widget
	{
	public:
		SexyString mLabel;
		Color mLabelColor;
		int mAlignment;
		Font* mLabelFont;

	public:
		MyLabelWidget();
		virtual ~MyLabelWidget();


		virtual void			Draw(Graphics* g);
		virtual void			SetLabel(SexyString theLabel);
	};
}

