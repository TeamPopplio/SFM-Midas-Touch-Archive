#include "stdafx.h"

#include "tier1\tier1.h"
#include "tier2\tier2.h"
#include "tier3\tier3.h"
#include "vgui_utils.hpp"

namespace vgui
{
#ifndef OE
	IClientMode* GetClientMode()
	{
		return nullptr;
	}
	IScheme* GetScheme()
	{
		return scheme()->GetIScheme(scheme()->GetDefaultScheme());
	}
#endif
} // namespace vgui