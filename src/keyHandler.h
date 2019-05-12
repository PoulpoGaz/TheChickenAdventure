#ifndef __keyHandler__
#define __keyHandler__

#include <stdint.h>
#include <tice.h>

#include <keypadc.h>

void setKey();
void setPreKey();
bool key(bool keys[2]);

extern bool key_2nd[2];
extern bool key_Del[2];

extern bool key_up[2];
extern bool key_down[2];
extern bool key_left[2];
extern bool key_right[2];

#endif