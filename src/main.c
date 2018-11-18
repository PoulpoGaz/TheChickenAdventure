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
#include "sprites_gfx.h"
#include "levels.h"

void render();
void update();
void check_victory();
void generate();
void menu();
void draw_menu(uint8_t yPos);
void game();
void newGame();
void draw_background(bool boolean);
void endGame();
void print_string_centered(const char *str, uint16_t y);
void nopwait(uint16_t n);
void colide(int dx, int dy);
void draw_Stats(uint8_t page);
void FadeIn();
void FadeOut();

gfx_sprite_t *chicken = chicken_down;
uint8_t y=0,x=0;
uint8_t nbObj=0,nbCaisseOk=0,push=0,moves=0;
uint8_t nbLvl=0;
uint8_t maxLvl=0;
uint8_t level[50][15][20];
uint24_t Moves[50];
uint24_t Pushs[50];
bool language=false;
uint8_t* brightness = (uint8_t*)0xF60024;
uint8_t minBrightness  = 229;  
uint8_t maxBrightness  = 49;   
uint8_t normalB;

void main(void) {
    ti_var_t file;
    srand(rtc_Time());

    ti_CloseAll();
    file = ti_Open("CHKNSV", "r+");
    if(file) {
        ti_Read(&maxLvl, sizeof(uint8_t), sizeof(maxLvl)/sizeof(uint8_t), file);
        ti_Read(&Moves, sizeof(uint24_t), sizeof(Moves)/sizeof(uint24_t), file);
        ti_Read(&Pushs, sizeof(uint24_t), sizeof(Moves)/sizeof(uint24_t), file);
        ti_Read(&language, sizeof(bool), sizeof(language)/sizeof(bool), file);
    }
    nbLvl=maxLvl;
    normalB = *brightness;

    gfx_Begin(gfx_8bpp);
    gfx_SetPalette(sprites_gfx_pal, sizeof sprites_gfx_pal, 0);
    gfx_SetTextFGColor(0x00);
    gfx_SetDrawBuffer();

    menu();
    ti_CloseAll();
    file = ti_Open("CHKNSV", "w");
    if(file){
        ti_Write(&maxLvl, sizeof(uint8_t), sizeof(maxLvl)/sizeof(uint8_t), file);
        ti_Write(&Moves, sizeof(uint24_t), sizeof(Moves)/sizeof(uint24_t), file);
        ti_Write(&Pushs, sizeof(uint24_t), sizeof(Moves)/sizeof(uint24_t), file);
        ti_Write(&language, sizeof(bool), sizeof(language)/sizeof(bool), file);
    }
    ti_SetArchiveStatus(true, file);
    gfx_End();
    prgm_CleanUp();
    *brightness = normalB;
}

void generate() {
    uint8_t tileX, tileY;
    nbObj=0;
    nbCaisseOk=0;
    moves=0;
    push=0;
    memcpy(level,levels,15000);
    for(tileY=0; tileY<15; tileY++) {
        for(tileX=0; tileX<20; tileX++) {
            if(level[nbLvl][tileY][tileX]==5) {
                x=tileX;
                y=tileY;
                level[nbLvl][tileY][tileX]=1;
            } else if(level[nbLvl][tileY][tileX]==2) nbObj++;
        }
    }
}

void render() {
    uint8_t tileX, tileY;
    for(tileY=0; tileY<15; tileY++) {
        for(tileX=0; tileX<20; tileX++) {
            if(level[nbLvl][tileY][tileX]==0) gfx_Sprite(mur,tileX*16,tileY*16);
            else if(level[nbLvl][tileY][tileX]==1) gfx_Sprite(grass,tileX*16,tileY*16);
            else if(level[nbLvl][tileY][tileX]==2) gfx_Sprite(objectif,tileX*16,tileY*16);
            else if(level[nbLvl][tileY][tileX]==3) gfx_Sprite(caisse,tileX*16,tileY*16);
            else if(level[nbLvl][tileY][tileX]==4) gfx_Sprite(caisse_ok,tileX*16,tileY*16);
            if(tileX==x&&tileY==y)gfx_TransparentSprite(chicken,x*16,y*16);

        }
    }
    gfx_PrintStringXY("Level:",0,0);
    gfx_SetTextXY(45,0);
    gfx_PrintUInt(nbLvl+1,2);
    gfx_PrintStringXY("Mouvements",80, 0);
    gfx_SetTextXY(170,0);
    gfx_PrintUInt(moves, 5);
    gfx_PrintStringXY("Push:",220,0);
    gfx_SetTextXY(260,0);
    gfx_PrintUInt(push, 5),
    print_string_centered("[2nd]-recommencer [Del]-retour", 230);
    gfx_SwapDraw();
}

void update() {
    int dx=0;
    int dy=0;
    if(kb_ScanGroup(kb_group_7) == kb_Up) {
        chicken=chicken_up;
        dy--;
        colide(dx,dy);
    } else if(kb_ScanGroup(kb_group_7) == kb_Down) {
        chicken=chicken_down;
        dy++;
        colide(dx,dy);
    } else if(kb_ScanGroup(kb_group_7) == kb_Left) {
        chicken=chicken_left;
        dx--;
        colide(dx,dy);
    } else if(kb_ScanGroup(kb_group_7) == kb_Right) {
        chicken=chicken_right;
        dx++;
        colide(dx,dy);
    }
}

void colide(int dx, int dy) {
    switch(level[nbLvl][y+dy][x+dx]) {
    case 1:
        y+=dy;
        x+=dx;
        moves++;
        break;
    case 2:
        y+=dy;
        x+=dx;
        moves++;
        break;
    case 3:
        if(level[nbLvl][y+2*dy][x+2*dx]==1) {
            level[nbLvl][y+dy][x+dx]=1;
            level[nbLvl][y+2*dy][x+2*dx]=3;
            y+=dy;
            x+=dx;
            moves++;
            push++;
        } else if(level[nbLvl][y+2*dy][x+2*dx]==2) {
            level[nbLvl][y+dy][x+dx]=1;
            level[nbLvl][y+2*dy][x+2*dx]=4;
            y+=dy;
            x+=dx;
            moves++;
            push++;
            nbCaisseOk++;
        }
        break;
    case 4:
        if(level[nbLvl][y+2*dy][x+2*dx]==1) {
            level[nbLvl][y+dy][x+dx]=2;
            level[nbLvl][y+2*dy][x+2*dx]=3;
            y+=dy;
            x+=dx;
            moves++;
            push++;
            nbCaisseOk--;
        } else if(level[nbLvl][y+2*dy][x+2*dx]==2) {
            level[nbLvl][y+dy][x+dx]=2;
            level[nbLvl][y+2*dy][x+2*dx]=4;
            y+=dy;
            x+=dx;
            moves++;
            push++;
        }
        break;
    }
}

void check_victory() {
    if(nbCaisseOk==nbObj) {
        if(nbLvl+1<50) {
            Moves[nbLvl]=moves;
            Pushs[nbLvl]=push;
            nbLvl++;
            if(nbLvl>maxLvl) maxLvl=nbLvl;
            FadeOut();
            generate();
            render();
            FadeIn();
        } else if(nbLvl+1==50) {
            endGame();
        }
    }
}

void draw_background(bool boolean) {
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
    if(boolean) {
        gfx_ScaledTransparentSprite_NoClip(chicken_down,140,102,2,2);
        print_string_centered("THE CHICKEN ADVENTURE I", 50);
    }
}

void draw_menu(uint8_t yPos) {
    draw_background(true);
    gfx_SetColor(0x00); gfx_FillRectangle(95,yPos+2,8,2);
    print_string_centered("Continuer",140);
    gfx_PrintStringXY("<      >", 210,140);
    gfx_SetTextXY((gfx_lcdWidth/2)+gfx_GetStringWidth("Continuer")-10,140);
    gfx_PrintUInt(nbLvl+1, 2);
    print_string_centered("Nouvelle partie",160);
    print_string_centered("Stats",180);
    print_string_centered("Quitter",200);
    gfx_SwapDraw();
}

void menu() {
    uint8_t yPos=140;
    draw_menu(yPos);
    while (kb_ScanGroup(kb_group_6) != kb_Clear) {
        switch(kb_ScanGroup(kb_group_7)) {
        case kb_Down:
            if(yPos==140) yPos=160;
            else if(yPos==160) yPos=180;
            else if(yPos==180) yPos=200;
            else if(yPos==200) yPos=140;
            draw_menu(yPos);
            break;
        case kb_Up:
            if(yPos==140) yPos=200;
            else if(yPos==160) yPos=140;
            else if(yPos==180) yPos=160;
            else if(yPos==200) yPos=180;
            draw_menu(yPos);
            break;
        }
        if(yPos==140) {
            if(kb_ScanGroup(kb_group_7)==kb_Left) {
                if(nbLvl>0) nbLvl--;
                draw_menu(yPos);
            } else if(kb_ScanGroup(kb_group_7)==kb_Right) {
                if(nbLvl<maxLvl) nbLvl++;
                draw_menu(yPos);        
            } else if(kb_ScanGroup(kb_group_6)==kb_Enter) {
                game();
                draw_menu(yPos);
            }
        } else if(yPos==160&&kb_ScanGroup(kb_group_6)==kb_Enter) {
            newGame();
            yPos=140;
        } else if(yPos==180&&kb_ScanGroup(kb_group_6)==kb_Enter) {
            draw_Stats(0);
            yPos=140;
            draw_menu(yPos);
        } else if(yPos==200&&kb_ScanGroup(kb_group_6)==kb_Enter) {break;}
    }
}

void draw_Stats(uint8_t page) {
    uint8_t i, f=page*15, g=0;
    draw_background(false);
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
    while(kb_ScanGroup(kb_group_1)!=kb_Del) {
        if(kb_ScanGroup(kb_group_7)==kb_Left) {
            if(page!=0) draw_Stats(--page);
            else draw_Stats(3);
        } else if(kb_ScanGroup(kb_group_7)==kb_Right) {
            if(page!=3) draw_Stats(++page);
            else draw_Stats(0);
        }
    }
}

void game() {
    generate();
    render();
    while(kb_ScanGroup(kb_group_1) != kb_Del) {
        nopwait(30000);
        update();
        render();
        check_victory();
        if(nbLvl==50) break;
        if(kb_ScanGroup(kb_group_1)==kb_2nd) {
            generate();
            render();
        }
    }
    draw_menu(140);
}

void newGame() {
    ti_var_t file;
    uint8_t a,b;
    nbLvl=0;
    ti_CloseAll();
    file = ti_Open("CHKNSV", "w");
    if(file)ti_Write(&nbLvl, sizeof(uint24_t), sizeof(nbLvl)/sizeof(uint24_t), file);
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
    game();
}

void endGame() {
    draw_background(true);
    print_string_centered("Apres plusieurs mois de rangement intense,",120);
    print_string_centered("Super Poulet partit, effraye par ce rangement!",140);
    print_string_centered("Les poulets furent convertis en humains...",160);
    print_string_centered("Merci d'avoir joue!",200);
    print_string_centered("A suivre...",230);
    gfx_SwapDraw();
    while(!os_GetCSC());
    if(nbLvl!=50) {
        nbLvl++;
        maxLvl=nbLvl;
    }
}

void print_string_centered(const char *str, uint16_t y) {
    gfx_PrintStringXY(str, (gfx_lcdWidth-gfx_GetStringWidth(str)) / 2, y);
}

void nopwait(uint16_t n) {
    uint16_t j;
    for (j=0; j<n; j++) asm("nop"); // small delay
}

//Code by Xavier Andréani, Baptiste Bideau, John Breaker, Péter Asztalos
//with 1 modification

void FadeIn() {
    uint8_t i=0;
    for (i=*brightness; i>=normalB/*Here replace userBrightness by normalB*/; i--)
    {
        *brightness = i;
        nopwait(2000);
    }
}

void FadeOut() {
    uint8_t i=0;
    for (i=*brightness; i<=minBrightness; i++)
    {
        *brightness = i;
        nopwait(2000);
    }
}