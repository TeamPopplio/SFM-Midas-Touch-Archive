#define MIDAS_MESSAGE_PREFIX "Midas: "
#include <SPTLib\sptlib.hpp>

extern void (*EngineConCmd)(const char* cmd);
extern void (*EngineGetViewAngles)(float viewangles[3]);
extern void (*EngineSetViewAngles)(const float viewangles[3]);
