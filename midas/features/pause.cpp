#include "stdafx.h"
#include "..\feature.hpp"
#include "generic.hpp"
#include "convar.hpp"
#include "signals.hpp"
#include "dbg.h"

ConVar y_midas_pause("y_midas_pause", "0", FCVAR_ARCHIVE);

extern ConVar tas_pause;

// y_midas_pause stuff
class PauseFeature : public FeatureWrapper<PauseFeature>
{
public:
protected:
	virtual void InitHooks() override;

	virtual void LoadFeature() override;

private:
	uintptr_t ORIG_SpawnPlayer = 0;
	bool* pM_bLoadgame = nullptr;
	void* pGameServer = nullptr;

	void SV_ActivateServer(bool result);
	void FinishRestore(void* thisptr, int edx);
	void SetPaused(void* thisptr, int edx, bool paused);
};

static PauseFeature midas_pause;

void PauseFeature::InitHooks()
{
	FIND_PATTERN(engine, SpawnPlayer);
}

void PauseFeature::LoadFeature()
{
	pM_bLoadgame = nullptr;
	pGameServer = nullptr;
	SV_ActivateServerSignal.Connect(this, &PauseFeature::SV_ActivateServer);
	SetPausedSignal.Connect(this, &PauseFeature::SetPaused);
	FinishRestoreSignal.Connect(this, &PauseFeature::FinishRestore);

	if (ORIG_SpawnPlayer)
	{
		int ptnNumber = GetPatternIndex((void**)&ORIG_SpawnPlayer);

		switch (ptnNumber)
		{
		case 0:
			pM_bLoadgame = (*(bool**)(ORIG_SpawnPlayer + 5));
			pGameServer = (*(void**)(ORIG_SpawnPlayer + 18));
			break;

		case 1:
			pM_bLoadgame = (*(bool**)(ORIG_SpawnPlayer + 8));
			pGameServer = (*(void**)(ORIG_SpawnPlayer + 21));
			break;

		case 2: // 4104 is the same as 5135 here.
			pM_bLoadgame = (*(bool**)(ORIG_SpawnPlayer + 5));
			pGameServer = (*(void**)(ORIG_SpawnPlayer + 18));
			break;

		case 3: // 2257546 is the same as 5339 here.
			pM_bLoadgame = (*(bool**)(ORIG_SpawnPlayer + 8));
			pGameServer = (*(void**)(ORIG_SpawnPlayer + 21));
			break;

		case 4:
			pM_bLoadgame = (*(bool**)(ORIG_SpawnPlayer + 26));
			//pGameServer = (*(void **)(pSpawnPlayer + 21)); - We get this one from SV_ActivateServer in OE.
			break;

		case 5: // 6879 is the same as 5339 here.
			pM_bLoadgame = (*(bool**)(ORIG_SpawnPlayer + 8));
			pGameServer = (*(void**)(ORIG_SpawnPlayer + 21));
			break;

		default:
			Warning("Spawnplayer did not have a matching switch-case statement!\n");
			break;
		}

		DevMsg("m_bLoadGame is situated at %p.\n", pM_bLoadgame);

#if !defined(OE)
		DevMsg("pGameServer is %p.\n", pGameServer);
#endif
	}

	// SV_ActivateServer
	if (midas_generic.ORIG_SV_ActivateServer)
	{
		int ptnNumber = GetPatternIndex((void**)&midas_generic.ORIG_SV_ActivateServer);
		switch (ptnNumber)
		{
		case 3:
			pGameServer = (*(void**)((int)midas_generic.ORIG_SV_ActivateServer + 223));
			break;
		}

#if defined(OE)
		DevMsg("pGameServer is %p.\n", pGameServer);
#endif
	}

	bool pause1_works = midas_generic.ORIG_FinishRestore;
	bool pause2_works = ORIG_SpawnPlayer && midas_generic.ORIG_SV_ActivateServer;
	bool pause_works = midas_generic.ORIG_SetPaused && (pause1_works || pause2_works);

	if (!pause_works)
	{
		Warning("y_midas_pause has no effect.\n");
	}
	else
	{
		if (!pause1_works)
		{
			Warning("y_midas_pause 2 has no effect.\n");
		}

		if (!pause2_works)
		{
			Warning("y_midas_pause 1 has no effect.\n");
		}

		InitConcommandBase(y_midas_pause);
	}
}

void PauseFeature::SV_ActivateServer(bool result)
{
	DevMsg("Engine call: SV_ActivateServer() => %s;\n", (result ? "true" : "false"));

	if (tas_pause.GetBool())
	{
		tas_pause.SetValue(0);
	}

	if (midas_generic.ORIG_SetPaused && pM_bLoadgame && pGameServer)
	{
		if ((y_midas_pause.GetInt() == 2) && *pM_bLoadgame)
		{
			midas_generic.ORIG_SetPaused((void*)pGameServer, 0, true);
			DevMsg("Pausing...\n");

			midas_generic.shouldPreventNextUnpause = true;
		}
	}
}

void PauseFeature::FinishRestore(void* thisptr, int edx)
{
	DevMsg("Engine call: FinishRestore();\n");

	if (midas_generic.ORIG_SetPaused && (y_midas_pause.GetInt() == 1))
	{
		midas_generic.ORIG_SetPaused(thisptr, 0, true);
		DevMsg("Pausing...\n");

		midas_generic.shouldPreventNextUnpause = true;
	}
}

void PauseFeature::SetPaused(void* thisptr, int edx, bool paused)
{
	if (pM_bLoadgame)
	{
		DevMsg("Engine call: SetPaused( %s ); m_bLoadgame = %s\n",
		       (paused ? "true" : "false"),
		       (*pM_bLoadgame ? "true" : "false"));
	}
	else
	{
		DevMsg("Engine call: SetPaused( %s );\n", (paused ? "true" : "false"));
	}
}
