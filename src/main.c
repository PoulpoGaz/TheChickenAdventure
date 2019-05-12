#include "defines.h"

void update();
void render();

uint8_t nbObj=0;
uint8_t nbCaisseOk=0;
uint8_t curentLevel=0;
uint8_t maxLvl=0;
enum state gameState = MENU;
uint24_t Moves[50]; 
uint24_t Pushs[50];
chicken_t chicken;
case_t level[15][20];

void main(void) {
    ti_var_t file;
    
    //Init
    initFade();
    
    //Read
    srand(rtc_Time());

    ti_CloseAll();
    file = ti_Open("CHKNSV", "r+");
    if(file) {
        ti_Read(&maxLvl, sizeof(uint8_t), sizeof(maxLvl)/sizeof(uint8_t), file);
        ti_Read(&Moves, sizeof(uint24_t), sizeof(Moves)/sizeof(uint24_t), file);
        ti_Read(&Pushs, sizeof(uint24_t), sizeof(Moves)/sizeof(uint24_t), file);
    }
    //Set curentLevel
    curentLevel=maxLvl;

    gfx_Begin(gfx_8bpp);
    gfx_SetPalette(sprites_gfx_pal, sizeof_sprites_gfx_pal, 0);
    gfx_SetTextFGColor(0x00);
    gfx_SetDrawBuffer();

    //Game loop
    while(gameState != EXIT) {
        setKey();
        update();
        render();
        setPreKey();
    }

    //Write and quit
    ti_CloseAll();
    file = ti_Open("CHKNSV", "w");
    if(file){
        ti_Write(&maxLvl, sizeof(uint8_t), sizeof(maxLvl)/sizeof(uint8_t), file);
        ti_Write(&Moves, sizeof(uint24_t), sizeof(Moves)/sizeof(uint24_t), file);
        ti_Write(&Pushs, sizeof(uint24_t), sizeof(Moves)/sizeof(uint24_t), file);
    }
    ti_SetArchiveStatus(true, file);
    gfx_End();
    prgm_CleanUp();
    exitFade();
}

void update() {
    if(gameState == MENU) {
        updateMenu();
    } else if(gameState == GAME) {
        updateGame();
    } else if(gameState == STATS) {
        updateStats();
    }
}

void render() {
    if(gameState == MENU) {
        renderMenu();
    } else if(gameState == GAME) {
        renderGame();
    } else if(gameState == STATS) {
        renderStats();
    }
}