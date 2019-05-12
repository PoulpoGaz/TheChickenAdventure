#include "game.h"

void renderGame() {
	uint8_t x, y;
    for(y = 0; y < 15; y++) {
        for(x = 0; x < 20; x++) {
            gfx_Sprite(level[y][x].sprite,x*16,y*16);
        }
    }
    gfx_TransparentSprite(chicken.sprite, chicken.x * 16, chicken.y * 16);

    gfx_PrintStringXY("Level:",0,0);
    gfx_SetTextXY(45,0);
    gfx_PrintUInt(curentLevel+1,2);
    gfx_PrintStringXY("Mouvements",80, 0);
    gfx_SetTextXY(170,0);
    gfx_PrintUInt(chicken.moves, 5);
    gfx_PrintStringXY("Push:",220,0);
    gfx_SetTextXY(260,0);
    gfx_PrintUInt(chicken.pushs, 5),
    print_string_centered("[2nd]-recommencer [Del]-retour", 230);
    gfx_SwapDraw();
}

void updateGame() {
    int dx = 0;
    int dy = 0;
    if(key(key_up)) {
        chicken.sprite = chicken_up;
        dy = -1;
        collide(dx,dy);
    }
    if(key(key_down)) {
        chicken.sprite = chicken_down;
        dy = 1;
        collide(dx,dy);
    }
    if(key(key_left)) {
        chicken.sprite = chicken_left;
        dx = -1;
        collide(dx,dy);
    }
    if(key(key_right)) {
        chicken.sprite = chicken_right;
        dx = 1;
        collide(dx,dy);
    }
    if(key(key_Del)) {
        gameState = MENU;
    }
    if(key(key_2nd)) {
        generate();
    }

    check_victory();
}

void collide(int dx, int dy) {
    uint8_t x = chicken.x + dx;
    uint8_t y = chicken.y + dy;
    uint8_t x2 = x + dx;
    uint8_t y2 = y + dy;

    if(level[y][x].value == 1 || level[y][x].value == 2) {
        chicken.x = x;
        chicken.y = y;
        chicken.moves++;
    } else if(level[y][x].value == 3) {
        if(level[y2][x2].value == 1 || level[y2][x2].value == 2) {
            chicken.x = x;
            chicken.y = y;
            chicken.moves++;
            chicken.pushs++;

            if(level[y2][x2].value == 1) {
                initCase(x, y, 1);
                initCase(x2, y2, 3);
            } else if(level[y2][x2].value == 2) {
                initCase(x, y, 1);
                initCase(x2, y2,4);
                nbCaisseOk++;
            }
        }
    } else if(level[y][x].value == 4) {
        if(level[y2][x2].value == 1 || level[y2][x2].value == 2) {
            chicken.x = x;
            chicken.y = y;
            chicken.moves++;
            chicken.pushs++;

            if(level[y2][x2].value == 1) {
                nbCaisseOk--;
                initCase(x, y, 2);
                initCase(x2, y2, 3);
            } else if(level[y2][x2].value == 2) {
                initCase(x, y, 2);
                initCase(x2, y2, 4);
            }
        }
    }
}

void check_victory() {
    if(nbCaisseOk == nbObj) {
        if(curentLevel + 1 < 50) {
            if(Moves[curentLevel] < chicken.moves) {
                Moves[curentLevel] = chicken.moves;
            }
            if(Pushs[curentLevel] < chicken.pushs) {
                Pushs[curentLevel] = chicken.pushs;
            }
            curentLevel++;
            if(curentLevel>maxLvl) {
                maxLvl=curentLevel;
            }
            FadeOut();
            generate();
            renderGame();
            FadeIn();
        } else if(curentLevel+1==50) {
            endGame();
        }
    }
}

void startGame() {
    generate();
    gameState = GAME;
}

void newGame() {
    uint8_t a,b;
    curentLevel=0;
    maxLvl=0;
    for(a=0;a<50;a++) {
        Moves[a]=0;
        Pushs[a]=0;
    } 
    introd();
    startGame();
}

void endGame() {
    end();
    while(!os_GetCSC());
    if(curentLevel!=50) {
        curentLevel++;
        maxLvl=curentLevel;
    }
    gameState = MENU;
}

//INIT METHOD

void generate() {
    uint8_t x, y, tileValue;
    nbObj=0;
    nbCaisseOk=0;
    for(y = 0; y < 15; y++) {
        for(x = 0; x < 20; x++) {
            tileValue = levels[curentLevel][y][x];
            
            if(tileValue == 5) {
                initCase(x, y, 1);
                initChicken(x, y);
            } else {
                initCase(x, y, tileValue);
                
                if(tileValue==2) {
                    nbObj++;
                }
            }
        }
    }
}

void initChicken(uint8_t x, uint8_t y) {
    chicken.moves = 0;
    chicken.pushs = 0;
    chicken.x = x;
    chicken.y = y;
    chicken.sprite = chicken_down;
}

void initCase(uint8_t x, uint8_t y, uint8_t value) {
    if(value == 0) {
        level[y][x].sprite = wall;
    } else if(value == 1) {
        level[y][x].sprite = grass;
    } else if(value == 2) {
        level[y][x].sprite = objectif;
    } else if(value == 3) {
        level[y][x].sprite = caisse;
    } else if(value == 4) {
        level[y][x].sprite = caisse_ok;
    }
    level[y][x].value = value;
}