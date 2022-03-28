#pragma once

#include "convar.hpp"

#define FCVAR_TAS_RESET (1 << 31)

void Cvar_RegisterMidasCvars();
void Cvar_UnregisterMidasCvars();
void Cvar_InitConCommandBase(ConCommandBase& concommand, void* owner);

extern ConVar y_midas_pause;
extern ConVar y_midas_motion_blur_fix;
extern ConVar y_midas_autojump;
extern ConVar y_midas_additional_jumpboost;
extern ConVar y_midas_focus_nosleep;
extern ConVar y_midas_stucksave;
extern ConVar y_midas_piwsave;
extern ConVar y_midas_pause_demo_on_tick;
extern ConVar y_midas_on_slide_pause_for;
extern ConVar y_midas_prevent_vag_crash;
extern ConVar y_midas_disable_tone_map_reset;
extern ConVar y_midas_draw_seams;
extern ConVar y_midas_draw_portal_env;
extern ConVar y_midas_draw_portal_env_type;
extern ConVar y_midas_draw_portal_env_ents;
extern ConVar y_midas_draw_portal_env_remote;
extern ConVar y_midas_draw_portal_env_wireframe;
extern ConVar y_midas_vag_search_portal;
extern ConVar y_midas_vag_trace;
extern ConVar y_midas_vag_target;
extern ConVar y_midas_vag_trace_portal;

extern ConVar tas_strafe;
extern ConVar tas_strafe_type;
extern ConVar tas_strafe_dir;
extern ConVar tas_strafe_yaw;
extern ConVar tas_strafe_buttons;
extern ConVar tas_strafe_afh;
extern ConVar tas_strafe_afh_length;

extern ConVar tas_strafe_vectorial;
extern ConVar tas_strafe_vectorial_increment;
extern ConVar tas_strafe_vectorial_offset;
extern ConVar tas_strafe_vectorial_snap;
extern ConVar tas_strafe_allow_jump_override;

extern ConVar tas_force_airaccelerate;
extern ConVar tas_force_wishspeed_cap;
extern ConVar tas_reset_surface_friction;

extern ConVar tas_force_onground;

extern ConVar tas_anglespeed;
extern ConVar tas_pause;
extern ConVar tas_log;
extern ConVar tas_strafe_lgagst;
extern ConVar tas_strafe_lgagst_max;
extern ConVar tas_strafe_lgagst_min;
extern ConVar tas_strafe_lgagst_minspeed;
extern ConVar tas_strafe_lgagst_fullmaxspeed;
extern ConVar tas_strafe_jumptype;
extern ConVar tas_strafe_capped_limit;
extern ConVar tas_strafe_hull_is_line;
extern ConVar tas_strafe_use_tracing;
extern ConVar tas_strafe_autojb;
extern ConVar tas_strafe_version;

extern ConVar tas_script_printvars;
extern ConVar tas_script_savestates;
extern ConVar tas_script_onsuccess;

extern ConVar _y_midas_autojump_ensure_legit;
extern ConVar _y_midas_afterframes_reset_on_server_activate;
extern ConVar _y_midas_anglesetspeed;
extern ConVar _y_midas_pitchspeed;
extern ConVar _y_midas_yawspeed;
extern ConVar _y_midas_force_fov;

extern ConVar y_midas_hud_velocity;
extern ConVar y_midas_hud_flags;
extern ConVar y_midas_hud_moveflags;
extern ConVar y_midas_hud_movecollideflags;
extern ConVar y_midas_hud_collisionflags;
extern ConVar y_midas_hud_accel;
extern ConVar y_midas_hud_script_length;
extern ConVar y_midas_hud_portal_bubble;
extern ConVar y_midas_hud_decimals;
extern ConVar y_midas_hud_vars;
extern ConVar y_midas_hud_ag_sg_tester;
extern ConVar y_midas_hud_ent_info;
extern ConVar y_midas_hud_left;
extern ConVar y_midas_hud_oob;
extern ConVar y_midas_hud_isg;

extern ConVar y_midas_ihud;
extern ConVar y_midas_ihud_grid_size;
extern ConVar y_midas_ihud_grid_padding;
extern ConVar y_midas_ihud_x;
extern ConVar y_midas_ihud_y;

extern ConVar _y_midas_overlay;
extern ConVar _y_midas_overlay_type;
extern ConVar _y_midas_overlay_portal;
extern ConVar _y_midas_overlay_width;
extern ConVar _y_midas_overlay_fov;
extern ConVar _y_midas_overlay_swap;

#ifdef OE
extern ConVar y_midas_gamedir;
#endif

#if defined(SSDK2007) || defined(SSDK2013)
extern ConCommand y_midas_set_isg;
#endif

extern ConVar* _viewmodel_fov;
extern ConVar* _sv_accelerate;
extern ConVar* _sv_airaccelerate;
extern ConVar* _sv_friction;
extern ConVar* _sv_maxspeed;
extern ConVar* _sv_stopspeed;
extern ConVar* _sv_stepsize;
extern ConVar* _sv_gravity;
extern ConVar* _sv_maxvelocity;
extern ConVar* _sv_bounce;
