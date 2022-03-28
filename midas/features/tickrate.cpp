#include "stdafx.h"
#include "..\midas-serverplugin.hpp"
#include "tickrate.hpp"
#include "convar.hpp"
#include "dbg.h"

TickrateMod midas_tickrate;

float TickrateMod::GetTickrate()
{
	if (pIntervalPerTick)
		return *pIntervalPerTick;

	return 0.0f;
}

void TickrateMod::SetTickrate(float tickrate)
{
	if (pIntervalPerTick)
		*pIntervalPerTick = tickrate;
}

bool TickrateMod::ShouldLoadFeature()
{
	return true;
}

void TickrateMod::InitHooks()
{
	FIND_PATTERN(engine, MiddleOfSV_InitGameDLL);
}

void TickrateMod::UnloadFeature() {}

CON_COMMAND(_y_midas_tickrate, "Get or set the tickrate. Usage: _y_midas_tickrate [tickrate]")
{
	switch (args.ArgC())
	{
	case 1:
		Msg("Current tickrate: %f\n", midas_tickrate.GetTickrate());
		break;

	case 2:
		midas_tickrate.SetTickrate(atof(args.Arg(1)));
		break;

	default:
		Msg("Usage: _y_midas_tickrate [tickrate]\n");
	}
}

void TickrateMod::LoadFeature()
{
	// interval_per_tick
	if (ORIG_MiddleOfSV_InitGameDLL)
	{
		int ptnNumber = GetPatternIndex((void**)&ORIG_MiddleOfSV_InitGameDLL);

		switch (ptnNumber)
		{
		case 0:
			pIntervalPerTick = *reinterpret_cast<float**>(ORIG_MiddleOfSV_InitGameDLL + 18);
			break;

		case 1:
			pIntervalPerTick = *reinterpret_cast<float**>(ORIG_MiddleOfSV_InitGameDLL + 16);
			break;
		default:
			pIntervalPerTick = nullptr;
			break;
		}

		DevMsg("Found interval_per_tick at %p.\n", pIntervalPerTick);

		if (pIntervalPerTick)
			InitCommand(_y_midas_tickrate);
	}
}
