#ifndef __fadeHandler__
#define __fadeHandler__

#include <stdint.h>
#include <tice.h>

void FadeIn();
void FadeOut();

void initFade();
void exitFade();

void nopwait(uint16_t a);

#endif