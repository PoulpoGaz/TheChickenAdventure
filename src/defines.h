#ifndef __defines__
#define __defines__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <graphx.h>
#include <fileioc.h>
#include <keypadc.h>

#include "gfx/sprites_gfx.h"
#include "levels/levels.h"

#include "game.h"
#include "fadeHandler.h"
#include "keyHandler.h"
#include "common.h"

enum state {
	MENU,
	STATS,
	GAME,
	EXIT
};

typedef struct {
	uint8_t x;
	uint8_t y;
	gfx_sprite_t *sprite;
	uint24_t pushs;
	uint24_t moves;
} chicken_t;

typedef struct {
	gfx_sprite_t *sprite;
	uint8_t value;
} case_t;

extern chicken_t chicken;
extern uint24_t Moves[50];
extern uint24_t Pushs[50];

extern enum state gameState;

extern uint8_t curentLevel;
extern uint8_t nbObj;
extern uint8_t nbCaisseOk;
extern uint8_t maxLvl;

extern case_t level[15][20];

#endif