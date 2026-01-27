#pragma once

#include "MyLabelWidget.h"

namespace Sexy
{
	class ToolTipWidget : public MyLabelWidget
	{
	public:
		ToolTipWidget(SexyString theToolTip);
		virtual ~ToolTipWidget();

		virtual void			Update();
		virtual void			Draw(Graphics* g);
	};
}

