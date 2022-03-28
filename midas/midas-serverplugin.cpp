#include "stdafx.h"
#include "..\stdafx.hpp"

#include <chrono>
#include <functional>
#include <sstream>
#include <time.h>

#include <SPTLib\Hooks.hpp>
#include "midas-serverplugin.hpp"
#include "..\sptlib-wrapper.hpp"
#include "ent_utils.hpp"
#include "math.hpp"
#include "string_utils.hpp"
#include "game_detection.hpp"
#include "custom_interfaces.hpp"
#include "cvars.hpp"
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
#include "interfaces.hpp"
#include "signals.hpp"

#if SSDK2007
#include "mathlib\vmatrix.h"
#endif

#include "SPTLib\sptlib.hpp"
#include "tier0\memdbgoff.h" // YaLTeR - switch off the memory debugging.
using namespace std::literals;

namespace interfaces
{
	std::unique_ptr<EngineClientWrapper> engine;
	IVEngineServer* engine_server = nullptr;
	IMatSystemSurface* surface = nullptr;
	vgui::ISchemeManager* scheme = nullptr;
	IVDebugOverlay* debugOverlay = nullptr;
	IMaterialSystem* materialSystem = nullptr;
	ICvar* g_pCVar = nullptr;
	void* gm = nullptr;
	IClientEntityList* entList;
	IVModelInfo* modelInfo;
	IBaseClientDLL* clientInterface;
} // namespace interfaces

ConVar* _viewmodel_fov = nullptr;
ConVar* _sv_accelerate = nullptr;
ConVar* _sv_airaccelerate = nullptr;
ConVar* _sv_friction = nullptr;
ConVar* _sv_maxspeed = nullptr;
ConVar* _sv_stopspeed = nullptr;
ConVar* _sv_stepsize = nullptr;
ConVar* _sv_gravity = nullptr;
ConVar* _sv_maxvelocity = nullptr;
ConVar* _sv_bounce = nullptr;

// useful helper func
inline bool FStrEq(const char* sz1, const char* sz2)
{
	return (Q_stricmp(sz1, sz2) == 0);
}

void CallServerCommand(const char* cmd)
{
	if (interfaces::engine)
		interfaces::engine->ClientCmd(cmd);
}

void GetViewAngles(float viewangles[3])
{
	if (interfaces::engine)
	{
		QAngle va;
		interfaces::engine->GetViewAngles(va);

		viewangles[0] = va.x;
		viewangles[1] = va.y;
		viewangles[2] = va.z;
	}
}

void SetViewAngles(const float viewangles[3])
{
	if (interfaces::engine)
	{
		QAngle va(viewangles[0], viewangles[1], viewangles[2]);
		interfaces::engine->SetViewAngles(va);
	}
}

void DefaultFOVChangeCallback(ConVar* var, char const* pOldString)
{
	if (FStrEq(var->GetString(), "75") && FStrEq(pOldString, "90"))
	{
		//Msg("Attempted to change default_fov from 90 to 75. Preventing.\n");
		var->SetValue("90");
	}
}

//
// The plugin is a static singleton that is exported as an interface
//
#ifdef OE
CMidasTouch g_MidasTouch;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CMidasTouch,
                                  IServerPluginCallbacks,
                                  INTERFACEVERSION_ISERVERPLUGINCALLBACKS_VERSION_1,
                                  g_MidasTouch);
#else
CMidasTouch g_MidasTouch;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CMidasTouch,
                                  IServerPluginCallbacks,
                                  INTERFACEVERSION_ISERVERPLUGINCALLBACKS,
                                  g_MidasTouch);
#endif

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

	ConnectTier1Libraries(&interfaceFactory, 1);
	ConnectTier3Libraries(&interfaceFactory, 1);

	interfaces::gm = gameServerFactory(INTERFACENAME_GAMEMOVEMENT, NULL);
	interfaces::g_pCVar = g_pCVar;
	interfaces::engine_server = (IVEngineServer*)interfaceFactory(INTERFACEVERSION_VENGINESERVER, NULL);
	interfaces::debugOverlay = (IVDebugOverlay*)interfaceFactory(VDEBUG_OVERLAY_INTERFACE_VERSION, NULL);
	interfaces::materialSystem = (IMaterialSystem*)interfaceFactory(MATERIAL_SYSTEM_INTERFACE_VERSION, NULL);

	auto clientFactory = Sys_GetFactory("client");
	interfaces::entList = (IClientEntityList*)clientFactory(VCLIENTENTITYLIST_INTERFACE_VERSION, NULL);
	interfaces::modelInfo = (IVModelInfo*)interfaceFactory(VMODELINFO_SERVER_INTERFACE_VERSION, NULL);
	interfaces::clientInterface = (IBaseClientDLL*)clientFactory(CLIENT_DLL_INTERFACE_VERSION, NULL);

	if (interfaces::gm)
	{
		DevMsg("Midas: Found IGameMovement at %p.\n", interfaces::gm);
	}
	else
	{
		DevWarning("Midas: Could not find IGameMovement.\n");
		DevWarning("Midas: ProcessMovement logging with tas_log is unavailable.\n");
	}

	if (g_pCVar)
#if defined(OE)
	{
		_viewmodel_fov = g_pCVar->FindVar("viewmodel_fov");
	}
#else
	{
#define GETCVAR(x) _##x = g_pCVar->FindVar(#x);

		GETCVAR(sv_airaccelerate);
		GETCVAR(sv_accelerate);
		GETCVAR(sv_friction);
		GETCVAR(sv_maxspeed);
		GETCVAR(sv_stopspeed);
		GETCVAR(sv_stepsize);
		GETCVAR(sv_gravity);
		GETCVAR(sv_maxvelocity);
		GETCVAR(sv_bounce);
	}
#endif

	// source filmmaker's engine client version
	auto ptr = interfaceFactory("VEngineClient013", NULL);

	if (ptr)
	{
#ifdef OE
		if (utils::DoesGameLookLikeDMoMM())
			interfaces::engine = std::make_unique<IVEngineClientWrapper<IVEngineClientDMoMM>>(
			    reinterpret_cast<IVEngineClientDMoMM*>(ptr));
#endif

		// Check if we assigned it in the ifdef above.
		if (!interfaces::engine)
			interfaces::engine = std::make_unique<IVEngineClientWrapper<IVEngineClient>>(
			    reinterpret_cast<IVEngineClient*>(ptr));
	}

	if (!interfaces::engine_server)
	{
		DevWarning("Midas: Failed to get the IVEngineServer interface.\n");
	}

	if (!interfaces::debugOverlay)
	{
		DevWarning("Midas: Failed to get the debug overlay interface.\n");
		Warning("Seam visualization has no effect.\n");
	}

	if (!interfaces::materialSystem)
		DevWarning("Midas: Failed to get the material system interface.\n");

	if (!interfaces::entList)
		DevWarning("Unable to retrieve entitylist interface.\n");

	if (!interfaces::modelInfo)
		DevWarning("Unable to retrieve the model info interface.\n");

	if (!interfaces::clientInterface)
		DevWarning("Unable to retrieve the client DLL interface.\n");

	EngineConCmd = CallServerCommand;
	EngineGetViewAngles = GetViewAngles;
	EngineSetViewAngles = SetViewAngles;

	_EngineMsg = Msg;
	_EngineDevMsg = DevMsg;
	_EngineWarning = Warning;
	_EngineDevWarning = DevWarning;

	TickSignal.Works = true;
	Feature::LoadFeatures();
	Cvar_RegisterMidasCvars();

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

	Cvar_UnregisterMidasCvars();
	DisconnectTier1Libraries();
	DisconnectTier3Libraries();
	Feature::UnloadFeatures();
	Hooks::Free();
	pluginLoaded = false;
}

const char* CMidasTouch::GetPluginDescription(void)
{
	return "Midas Touch v" MIDAS_VERSION ", KiwifruitDev - Fork of SourcePauseTool by Ivan \"YaLTeR\" Molodetskikh";
}

void CMidasTouch::GameFrame(bool simulating)
{
	if (simulating)
		TickSignal();
}
