#include "stdafx.h"
#include "..\feature.hpp"

typedef void(__fastcall* _GetModelViewerOpened)(void* thisptr, int edx);

// Feature description
class SFMTest : public FeatureWrapper<SFMTest>
{
public:
	static void __fastcall HOOKED_GetModelViewerOpened(void* thisptr, int edx);

protected:
	_GetModelViewerOpened ORIG_GetModelViewerOpened = nullptr;

	virtual bool ShouldLoadFeature() override
	{
		return true;
	}

	virtual void InitHooks() override
	{
		HOOK_FUNCTION(ifm, GetModelViewerOpened);
	}

	virtual void LoadFeature() override
	{

	}

	virtual void UnloadFeature() override
	{

	}
};

static SFMTest sfm_test;

void __fastcall SFMTest::HOOKED_GetModelViewerOpened(void* thisptr, int edx)
{
	sfm_test.ORIG_GetModelViewerOpened(thisptr, edx);
	Msg("Hello from Midas Touch!");
	return;
}
