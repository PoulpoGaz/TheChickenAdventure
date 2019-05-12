#ifndef __trex__
#define __trex__

#include "defines.h"

void renderGame();
void updateGame();
void generate();
void initCase(uint8_t x, uint8_t y, uint8_t value);
void initChicken(uint8_t x, uint8_t y);
void collide(int dx, int dy);
void startGame();
void endGame();
void newGame();
void check_victory();

#endif