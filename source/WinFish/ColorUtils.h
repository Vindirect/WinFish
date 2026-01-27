#ifndef __COLOR_UTILS_H_
#define __COLOR_UTILS_H_

#include "SexyAppFramework/Color.h"

#pragma once

namespace Sexy
{
	class ColorUtils
	{
	public:
		static				Color FromHSL(float h, float s, float l);
	};
}

#endif