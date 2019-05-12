#include "fadeHandler.h"

uint8_t levelLight = 0;

void FadeIn() {
    uint8_t i=0;
    for (i = levelLight; i > 40; i--) {
        boot_WaitShort();
        lcd_BacklightLevel = i;
    }
}

void FadeOut() {
	uint8_t i=0;
    for (i = levelLight; i < 200; i++) {
        boot_WaitShort();
        lcd_BacklightLevel = i;
    }
}

void initFade() {
	levelLight = lcd_BacklightLevel;
}

void exitFade() {
	lcd_BacklightLevel = levelLight;
}

void nopwait(uint16_t a) {
    uint16_t j;
    for (j=0; j<a; j++) asm("nop");
}