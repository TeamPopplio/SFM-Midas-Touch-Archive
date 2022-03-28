#include "stdafx.h"
#include "game_detection.hpp"
#include "tracing.hpp"
#include "signals.hpp"
#include "interfaces.hpp"
#include "..\feature.hpp"
#include "..\vgui\lines.hpp"

ConVar y_midas_draw_seams("y_midas_draw_seams", "0", FCVAR_CHEAT, "Draws seamshot stuff.\n");

class GraphicsFeature : public FeatureWrapper<GraphicsFeature>
{
public:
protected:
	virtual bool ShouldLoadFeature() override;

	virtual void LoadFeature() override;
};

static GraphicsFeature midas_graphics;

bool GraphicsFeature::ShouldLoadFeature()
{
#ifdef SSDK2007
	return utils::DoesGameLookLikePortal() && interfaces::debugOverlay;
#else
	return false;
#endif
}

void GraphicsFeature::LoadFeature()
{
	if (midas_tracing.ORIG_TraceFirePortal && midas_tracing.ORIG_GetActiveWeapon && interfaces::debugOverlay
	    && AdjustAngles.Works)
	{
		InitConcommandBase(y_midas_draw_seams);
		AdjustAngles.Connect(vgui::DrawLines);
	}
}
