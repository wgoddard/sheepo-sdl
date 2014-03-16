#ifndef __GLOBAL_H
#define __GLOBAL_H

#include <string>
#include "hge.h"

enum screen {S_WIDTH = 1024, S_HEIGHT = 768, S_BPP = 32, S_WINDOWED = true};

const std::string NAME = "El Sheepo";
const std::string LOGFILE = "sheep.log";

const int MAX_CONTROLLERS = 4;
extern short NUMBER_PLAYERS;
extern short PLUGGED_IN;

extern bool DEBUG;
//32bit DWORD color in format alpha, red, green, blue (8 bits each)
//const DWORD COLOR = (255 << 24) + (176 << 16) + (23 << 8) + 31;
const DWORD COLOR = 0x99FFFFFF;
//const DWORD COLOR = ARGB(255, 176, 23, 31);
//const DWORD COLOR = 0xFF881015

//physics iterations
const int PHYS_ITER = 20;

//team colors
const DWORD TEAM_COLORS[] = { 0xFFEF4147, 0xFF46A3E0,  ARGB(255,255,255,255) };

struct KeyCount { int KeyA; int KeyB; int KeyC; };

#endif