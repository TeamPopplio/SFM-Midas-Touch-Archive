#include "stdafx.h"
#include "..\cvars.hpp"
#include "aim.hpp"
#include "generic.hpp"
#include "ent_utils.hpp"
#include "math.hpp"
#include "playerio.hpp"

#undef min
#undef max

ConVar _y_midas_anglesetspeed(
    "_y_midas_anglesetspeed",
    "360",
    FCVAR_TAS_RESET,
    "Determines how fast the view angle can move per tick while doing _y_midas_setyaw/_y_midas_setpitch.\n");
ConVar _y_midas_pitchspeed("_y_midas_pitchspeed", "0", FCVAR_TAS_RESET);
ConVar _y_midas_yawspeed("_y_midas_yawspeed", "0", FCVAR_TAS_RESET);
ConVar tas_anglespeed("tas_anglespeed",
                      "5",
                      FCVAR_CHEAT,
                      "Determines the speed of angle changes when using tas_aim or when TAS strafing\n");

AimFeature midas_aim;

void AimFeature::HandleAiming(float* va, bool& yawChanged)
{
	// Use tas_aim stuff for tas_strafe_version >= 4
	if (tas_strafe_version.GetInt() >= 4)
	{
		midas_aim.viewState.UpdateView(va[PITCH], va[YAW]);
	}

	double pitchSpeed = atof(_y_midas_pitchspeed.GetString()), yawSpeed = atof(_y_midas_yawspeed.GetString());

	if (pitchSpeed != 0.0f)
		va[PITCH] += pitchSpeed;
	if (setPitch.set)
	{
		setPitch.set = DoAngleChange(va[PITCH], setPitch.angle);
	}

	if (yawSpeed != 0.0f)
	{
		va[YAW] += yawSpeed;
	}
	if (setYaw.set)
	{
		yawChanged = true;
		setYaw.set = DoAngleChange(va[YAW], setYaw.angle);
	}
}

bool AimFeature::DoAngleChange(float& angle, float target)
{
	float normalizedDiff = utils::NormalizeDeg(target - angle);
	if (std::abs(normalizedDiff) > _y_midas_anglesetspeed.GetFloat())
	{
		angle += std::copysign(_y_midas_anglesetspeed.GetFloat(), normalizedDiff);
		return true;
	}
	else
	{
		angle = target;
		return false;
	}
}

void AimFeature::SetPitch(float pitch)
{
	setPitch.angle = pitch;
	setPitch.set = true;
}
void AimFeature::SetYaw(float yaw)
{
	setYaw.angle = yaw;
	setYaw.set = true;
}
void AimFeature::ResetPitchYawCommands()
{
	setYaw.set = false;
	setPitch.set = false;
}

void AimFeature::SetJump()
{
	viewState.SetJump();
}

CON_COMMAND(tas_aim_reset, "Resets tas_aim state")
{
	midas_aim.viewState.set = false;
	midas_aim.viewState.ticksLeft = 0;
	midas_aim.viewState.timedChange = false;
	midas_aim.viewState.jumpedLastTick = false;
}

CON_COMMAND(tas_aim, "Aims at a point.")
{
	if (args.ArgC() < 3)
	{
		Msg("Usage: tas_aim <pitch> <yaw> [ticks] [cone]\nWeapon cones(in degrees):\n\t- AR2: 3\n\t- Pistol & SMG: 5\n");
		return;
	}

	float pitch = clamp(std::atof(args.Arg(1)), -89, 89);
	float yaw = utils::NormalizeDeg(std::atof(args.Arg(2)));
	int frames = -1;
	int cone = -1;

	if (args.ArgC() >= 4)
		frames = std::atoi(args.Arg(3));

	if (args.ArgC() >= 5)
		cone = std::atoi(args.Arg(4));

	QAngle angle(pitch, yaw, 0);
	QAngle aimAngle = angle;

	if (cone >= 0)
	{
		if (!utils::playerEntityAvailable())
		{
			Warning(
			    "Trying to apply nospread while map not loaded in! Wait until map is loaded before issuing tas_aim with spread cone set.\n");
			return;
		}

		Vector vecSpread;
		if (!aim::GetCone(cone, vecSpread))
		{
			Warning("Couldn't find cone: %s\n", args.Arg(4));
			return;
		}

		// Even the first approximation seems to be relatively accurate and it seems to converge after 2nd iteration
		for (int i = 0; i < 2; ++i)
			aim::GetAimAngleIterative(angle, aimAngle, frames, vecSpread);

		QAngle punchAngle, punchAnglevel;

		if (utils::GetPunchAngleInformation(punchAngle, punchAnglevel))
		{
			QAngle futurePunchAngle = aim::DecayPunchAngle(punchAngle, punchAnglevel, frames);
			aimAngle -= futurePunchAngle;
			aimAngle[PITCH] = clamp(aimAngle[PITCH], -89, 89);
		}
	}

	midas_aim.viewState.set = true;
	midas_aim.viewState.target = aimAngle;

	if (frames == -1)
	{
		midas_aim.viewState.timedChange = false;
	}
	else
	{
		midas_aim.viewState.timedChange = true;
		midas_aim.viewState.ticksLeft = std::max(1, frames);
	}
}

CON_COMMAND(_y_midas_setpitch, "Sets the pitch. Usage: _y_midas_setpitch <pitch>")
{
	if (args.ArgC() != 2)
	{
		Msg("Usage: _y_midas_setpitch <pitch>\n");
		return;
	}

	midas_aim.SetPitch(atof(args.Arg(1)));
}

CON_COMMAND(_y_midas_setyaw, "Sets the yaw. Usage: _y_midas_setyaw <yaw>")
{
	if (args.ArgC() != 2)
	{
		Msg("Usage: _y_midas_setyaw <yaw>\n");
		return;
	}

	midas_aim.SetYaw(atof(args.Arg(1)));
}

CON_COMMAND(_y_midas_resetpitchyaw, "Resets pitch/yaw commands.")
{
	midas_aim.ResetPitchYawCommands();
}

CON_COMMAND(_y_midas_setangles, "Sets the angles. Usage: _y_midas_setangles <pitch> <yaw>")
{
	if (args.ArgC() != 3)
	{
		Msg("Usage: _y_midas_setangles <pitch> <yaw>\n");
		return;
	}

	midas_aim.SetPitch(atof(args.Arg(1)));
	midas_aim.SetYaw(atof(args.Arg(2)));
}

CON_COMMAND(_y_midas_setangle,
            "Sets the yaw/pitch angle required to look at the given position from player's current position.")
{
	Vector target;

	if (args.ArgC() > 3)
	{
		target.x = atof(args.Arg(1));
		target.y = atof(args.Arg(2));
		target.z = atof(args.Arg(3));

		Vector player_origin = midas_playerio.GetPlayerEyePos();
		Vector diff = (target - player_origin);
		QAngle angles;
		VectorAngles(diff, angles);
		midas_aim.SetPitch(angles[PITCH]);
		midas_aim.SetYaw(angles[YAW]);
	}
}

void AimFeature::LoadFeature()
{
	if (midas_generic.ORIG_AdjustAngles && midas_playerio.ORIG_CreateMove)
	{
		InitCommand(tas_aim_reset);
		InitCommand(tas_aim);
		InitCommand(_y_midas_setyaw);
		InitCommand(_y_midas_setpitch);
		InitCommand(_y_midas_resetpitchyaw);
		InitCommand(_y_midas_setangles);
		InitCommand(_y_midas_setangle);

		InitConcommandBase(_y_midas_anglesetspeed);
		InitConcommandBase(_y_midas_pitchspeed);
		InitConcommandBase(_y_midas_yawspeed);
		InitConcommandBase(tas_anglespeed);
	}
}