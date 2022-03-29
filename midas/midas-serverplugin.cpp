#include "stdafx.h"
#include "..\stdafx.hpp"

#include <chrono>
#include <functional>
#include <sstream>
#include <time.h>

#include <SPTLib\Hooks.hpp>
#include "midas-serverplugin.hpp"
#include "math.hpp"
#include "string_utils.hpp"
#include "..\feature.hpp"
#include "vstdlib\random.h"

#include "cdll_int.h"
#include "eiface.h"
#include "engine\iserverplugin.h"
#include "icliententitylist.h"
#include "tier2\tier2.h"
#include "tier3\tier3.h"
#include "vgui\iinput.h"
#include "vgui\isystem.h"
#include "vgui\ivgui.h"
#include "SDK\hl_movedata.h"
#include "SDK\igamemovement.h"

#include "SPTLib\sptlib.hpp"
// #include "tier0\memdbgoff.h" // YaLTeR - switch off the memory debugging.
using namespace std::literals;

//
// The plugin is a static singleton that is exported as an interface
//
CMidasTouch g_MidasTouch;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CMidasTouch,
                                  IServerPluginCallbacks,
                                  INTERFACEVERSION_ISERVERPLUGINCALLBACKS,
                                  g_MidasTouch);

bool CMidasTouch::Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory)
{
	auto startTime = std::chrono::high_resolution_clock::now();

	if (pluginLoaded)
	{
		Warning("Trying to load Midas when Midas is already loaded.\n");
		// Failure to load causes immediate call to Unload, make sure we do nothing there
		skipUnload = true;
		return false;
	}
	pluginLoaded = true;

	_EngineMsg = Msg;
	_EngineDevMsg = DevMsg;
	_EngineWarning = Warning;
	_EngineDevWarning = DevWarning;

	Feature::LoadFeatures();

	auto loadTime =
	    std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - startTime)
	        .count();
	std::ostringstream out;
	out << "Midas Touch version " MIDAS_VERSION " was loaded in " << loadTime << "ms.\n";

	Msg("%s", std::string(out.str()).c_str());

	return true;
}

void CMidasTouch::Unload(void)
{
	if (skipUnload)
	{
		// Preventing double load of plugin, do nothing on unload
		skipUnload = false; // Enable unloading again
		return;
	}

	Feature::UnloadFeatures();
	Hooks::Free();
	pluginLoaded = false;
}

const char* CMidasTouch::GetPluginDescription(void)
{
	return "Midas Touch v" MIDAS_VERSION ", KiwifruitDev - Fork of SourcePauseTool by Ivan \"YaLTeR\" Molodetskikh";
}
