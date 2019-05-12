#include "common.h"

uint8_t page = 0;
uint8_t yPos = 140;

void renderStats() {
    uint8_t i, f=page*15, g=0;
    renderBackground(false);
    for(i=0; i<15;i++) {
        g=i+f;
        if(g==50) break;
        gfx_PrintStringXY("Level: ",0,i*15);
        gfx_SetTextXY(50,i*15);
        gfx_PrintUInt(g+1, 2);
        gfx_PrintStringXY("Moves: ", 80,i*15);
        if(Moves[g]!=0) {
            gfx_SetTextXY(140,i*15);
            gfx_PrintUInt(Moves[g], 5);
        }
        gfx_PrintStringXY("Pushs: ", 190, i*15);
        if(Pushs[g]!=0) {
            gfx_SetTextXY(250, i*15);
            gfx_PrintUInt(Pushs[g],5);
        }
    }
    print_string_centered("[<>]-deplacer [Del]-retour",230);
    gfx_SwapDraw();
}

void introd() {
	uint8_t a, b;
	gfx_SetColor(0x09);
    gfx_FillRectangle(0,0,320,240);
    gfx_ScaledTransparentSprite_NoClip(intro,0,0,2,2);
    print_string_centered("En l'an 0, la lune s'ecrasa sur Terre apres",200);
    print_string_centered("s'etre fait posseder par Super Poulet.",210);
    print_string_centered("Les humains furent convertis en poulet...",220);
    gfx_SwapDraw();
    while(!os_GetCSC());
    gfx_FillRectangle(0,0,320,240);
    for(a=1; a*28<292; a++) {
        for(b=0; b*28<160; b+=2) {
            gfx_ScaledTransparentSprite_NoClip(doctor_chicken,a*28,b*28,4,4);
            gfx_ScaledTransparentSprite_NoClip(doctor_chicken,a*28-17,b*28+28,4,4);
        }
    }
    print_string_centered("En 93632 apSP, les chercheurs poulets ont",200);
    print_string_centered("trouve le moyen de revenir a leur etat",210);
    print_string_centered("naturel. Il faut qu'ils rangent des caisses...",220);
    gfx_SwapDraw();
    while(!os_GetCSC());
    gameState = GAME;
}

void end() {
	renderBackground(true);
    print_string_centered("Apres plusieurs mois de rangement intense,",120);
    print_string_centered("Super Poulet partit, effraye par ce rangement!",140);
    print_string_centered("Les poulets furent convertis en humains...",160);
    print_string_centered("Merci d'avoir joue!",200);
    print_string_centered("A suivre... peut etre... c'est pas sur...",230);
    gfx_SwapDraw();
    while(!os_GetCSC());
}

void renderBackground(bool title) {
    uint8_t a, yy,l;
    uint16_t xx;
    gfx_FillScreen(0xAF);
    gfx_SetColor(0x03); gfx_FillCircle(160,240,170);
    gfx_SetColor(0x11); gfx_FillCircle(160,240,140);
    gfx_SetColor(0x3F); gfx_FillCircle(160,240,110);
    gfx_SetColor(0x06); gfx_FillCircle(160,240,90);
    gfx_SetColor(0x32); gfx_FillCircle(160,240,70);
    gfx_SetColor(0x28); gfx_FillCircle(160,240,50);
    gfx_SetColor(0x0E);
    for(a=0; a<randInt(70,90); a++) {
        xx=randInt(0,320); yy=randInt(0,100); l=randInt(1,2);
        gfx_FillRectangle(xx,yy,l,l);
    }
    if(title) {
        gfx_ScaledTransparentSprite_NoClip(chicken_down,140,102,2,2);
        print_string_centered("THE CHICKEN ADVENTURE I", 50);
    }
}

void renderMenu() {
    renderBackground(true);
    gfx_SetColor(0x00); gfx_FillRectangle(95,yPos+2,8,2);
    print_string_centered("Continuer", 140);
    gfx_PrintStringXY("<      >", 210,140);
    gfx_SetTextXY((gfx_lcdWidth/2)+gfx_GetStringWidth("Continuer")-10,140);
    gfx_PrintUInt(curentLevel+1, 2);
    print_string_centered("Nouvelle partie",160);
    print_string_centered("Stats",180);
    print_string_centered("Quitter",200);

    gfx_PrintStringXY("V2.0", 290, 230);
    gfx_PrintStringXY("PoulpoCorp (C)", 0, 230);
    gfx_SwapDraw();
}

void updateMenu() {
	if(key(key_down)) {
		if(yPos == 200) {
			yPos = 140;
		} else {
			yPos+=20;
		}
	} else if(key(key_up)) {
		if(yPos == 140) {
			yPos = 200;
		} else {
			yPos-=20;
		}
	} else if(key(key_2nd)) {
		if(yPos == 140) {
			gameState = GAME;
			startGame();
		} else if(yPos == 160) {
			newGame();
		} else if(yPos == 180) {
			gameState = STATS;
		} else if(yPos == 200) {
			gameState = EXIT;
		}
	} else if(key(key_Del)) {
		gameState = EXIT;
	} else if(key(key_left)) {
		if(curentLevel != 0) {
			curentLevel--;
		}
	} else if(key(key_right)) {
		if(curentLevel != maxLvl) {
			curentLevel++;
		}
	}
}

void updateStats() {
	if(key(key_left)) {
		if(page == 0) {
			page = 3;
		} else {
			page--;
		}
	} else if(key(key_right)) {
		if(page == 3) {
			page = 0;
		} else {
			page++;
		}
	} else if(key(key_Del)) {
		gameState = MENU;
	}
}

void print_string_centered(const char *str, uint16_t y) {
    gfx_PrintStringXY(str, (gfx_lcdWidth-gfx_GetStringWidth(str)) / 2, y);
}