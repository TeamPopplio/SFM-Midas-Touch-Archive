#include "stdafx.h"
#include "..\feature.hpp"
#include "..\utils\game_detection.hpp"
#include "shadow.hpp"

typedef int(__fastcall* _GetShadowPosition)(void* thisptr, int edx, Vector* worldPosition, QAngle* angles);

// This feature allows access to the Havok hitbox location (aka physics shadow)
class ShadowPosition : public FeatureWrapper<ShadowPosition>
{
public:
	static int __fastcall HOOKED_GetShadowPosition(void* thisptr, int edx, Vector* worldPosition, QAngle* angles);
	Vector PlayerHavokPos;

protected:
	_GetShadowPosition ORIG_GetShadowPosition = nullptr;

	virtual bool ShouldLoadFeature() override
	{
#ifdef SSDK2013
		return !utils::DoesGameLookLikePortal();
#else

		return true;
#endif
	}

	virtual void InitHooks() override
	{
		HOOK_FUNCTION(vphysics, GetShadowPosition);
		PlayerHavokPos.Init(0, 0, 0);
	}

	virtual void LoadFeature() override {}

	virtual void UnloadFeature() override {}
};

static ShadowPosition midas_position;

int __fastcall ShadowPosition::HOOKED_GetShadowPosition(void* thisptr, int edx, Vector* worldPosition, QAngle* angles)
{
	int GetShadowPos = midas_position.ORIG_GetShadowPosition(thisptr, edx, worldPosition, angles);
	midas_position.PlayerHavokPos.x = worldPosition->x;
	midas_position.PlayerHavokPos.y = worldPosition->y;
	midas_position.PlayerHavokPos.z = worldPosition->z;
	return GetShadowPos;
}

Vector GetPlayerHavokPos()
{
	return midas_position.PlayerHavokPos;
}
