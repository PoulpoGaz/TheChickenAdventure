#ifndef __common__
#define __common__

#include "defines.h"

void renderMenu();
void introd();
void end();
void renderBackground(bool title);
void renderStats();
void print_string_centered(const char *str, uint16_t y);

void updateStats();
void updateMenu();

extern uint8_t yPos;
extern uint8_t page;

#endif