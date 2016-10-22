#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "Highscore.h"

const int64_t SCREEN_WIDTH = 128;
const int64_t SCREEN_HEIGHT = 64;

const int16_t ROCKET_INFO_PANEL_WIDTH = 15;
const int16_t GAME_PANEL_WIDTH = 101;
const int16_t STAGE_INFO_PANEL_WIDTH = 10;

const int16_t ROCKET_INFO_PANEL_X = 0;
const int16_t GAME_PANEL_X = ROCKET_INFO_PANEL_WIDTH + 1;
const int16_t STAGE_INFO_PANEL_X = ROCKET_INFO_PANEL_WIDTH + GAME_PANEL_WIDTH + 1;

Highscore highscore;
int16_t score;
bool music;

#endif
