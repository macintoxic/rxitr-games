// 
// This file is part of JezzballDS.
// 
// JezzballDS is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// JezzballDS is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with JezzballDS.  If not, see <http://www.gnu.org/licenses/>.
// 

// Includes
#include <PA9.h>       // Include for PA_Lib
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <libfat.h>

#include "bounce.h"
#include "jezzdead.h"

// Converted using PAGfxConverter
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"
#include "font/all_gfx.h"
#include "font/all_gfx.c"

#include "messages.h"
#include "jezzball.h"

void loadSaveData(void);
void saveSRAM(void);
void initBalls(void);
void moveBalls(void);
void initWbd(void);
void moveWbd(void);
void buildWall(wall * wall, s16 xstep, s16 ystep, u16 tilenum, u8 spritenum);
void fillWall(wall * wall, u16 tilenum, u8 spritenum);
u16 checkCollision(s16 x, s16 y);
void clearEmptyRooms(void);
void initLevel(u8 level);
void initMenu(void);
void destroyBalls(void);
void destroyWbd(void);
void promptHighscore(void);
void checkIfWin(void);
void updateCurrentMenu(void);

void menu(void);
void menuPlay(void);
void menuAbout(void);

void play(void);
void setPause(bool pause);
void switchPause(void);

void fadeIn(u8 screen0, u8 screen1);
void fadeOut(u8 screen0, u8 screen1);


s16 width, height;

sprite balls[MAX_NB_BALLS];

wall blueWall, redWall;
sprite wbd;

u16 speed = 256;

u16 collision[1024];

u8 level = 1;
u8 lives = 0;
u8 nbBalls = 0;

s32 levelScore = 0;
s32 totalScore = 0;
u32 bonus = 0;

u8 columns[32];
u8 rows[24];

u16 time4lvl;
s16 timeLeft;

u8 language;
u8 inPause = 0, complete = 0, gameover = 0, levelComplete = 0, inMenu = 0, currentMenuItem = 0;

u16 tilesToClear;

save saveData;


l18n messages;


// Function: main()
int main(int argc, char ** argv)
  {
    width = background_Info[BG_INFO_WIDTH]>>3;
    height = background_Info[BG_INFO_HEIGHT]>>3;
    
    PA_Init();
    PA_InitVBL();
    PA_InitSound();
    
    initLang(&messages);
    
    fatInitDefault();
    
    PA_SetBrightness(0, -31);
    PA_SetBrightness(1, -31);
    
    
    loadSaveData();

    // PA_SplashBlue();

    PA_LoadSpritePal(0, 0, (void*) sprite0_Pal);

    initMenu();
    
    fadeIn(1, 1);
    
    // Infinite loop to keep the program running
    while (1)
      {
        if (inMenu)
          {
            menu();
          }
        else
          {
            // in game
            
            if (!inPause && !gameover && !levelComplete && !inMenu)
              {
                play();
              }
    
            if (!gameover && !levelComplete && !inMenu && Pad.Newpress.Start)
              {
                switchPause();
              }
            if (PA_LidClosed())
              {
                setPause(true);
              }
    
            if (gameover && (Pad.Newpress.A || Stylus.Newpress))
              {
                totalScore += levelScore;
                promptHighscore();
                fadeOut(1,1);
                destroyBalls();
                destroyWbd();
                gameover = 0;
                totalScore = 0;
                initMenu();
                fadeIn(1,1);
              }
            else if (levelComplete && (Pad.Newpress.A || Stylus.Newpress))
              {
                levelComplete = 0;
                totalScore += levelScore;
                fadeOut(1, 0);
                destroyBalls();
                initLevel(++level);
                fadeIn(1, 0);
                PA_VBLCounterStart(0);
              }
          }
        
        PA_WaitForVBL();
      }

    return 0;
  } // End of main()

void switchPause(void)
  {
    setPause(!inPause);
  }

void setPause(bool pause)
  {
    if (pause)
      {
        inPause = 1;
        PA_VBLCounterPause(0);
        messages.pause();
      }
    else
      {
        inPause = 0;
        PA_VBLCounterUnpause(0);
        msg_none();
      }
  }

void menu(void)
  {
    if (Pad.Newpress.Up || Pad.Newpress.Down)
      {
        currentMenuItem += 3 + Pad.Newpress.Down - Pad.Newpress.Up;
        currentMenuItem = currentMenuItem%3;
        updateCurrentMenu();
      }
    if (Stylus.Newpress && Stylus.X >= 72 && Stylus.X <= 184)
      {
        if (Stylus.Newpress && Stylus.Y >= 24 && Stylus.Y <= 48)
          {
            if (currentMenuItem == MENU_PLAY)
              menuPlay();
            currentMenuItem = MENU_PLAY;
          }
        else if (Stylus.Newpress && Stylus.Y >= 72 && Stylus.Y <= 96)
          {
            currentMenuItem = 1;
          }
        else if (Stylus.Newpress && Stylus.Y >= 120 && Stylus.Y <= 144)
          {
            if (currentMenuItem == MENU_ABOUT)
              menuAbout();
            currentMenuItem = MENU_ABOUT;
          }
        updateCurrentMenu();
      }
    if (Pad.Newpress.A)
      {
        switch (currentMenuItem)
        {
        case MENU_PLAY :
          menuPlay();
          break;
        case MENU_ABOUT :
          menuAbout();
          break;
        }
      }
  }

void menuPlay(void)
  {
    inMenu = 0;
    fadeOut(1,1);
    destroyBalls();
    initLevel(level = 1);
    fadeIn(1,1);
    PA_VBLCounterStart(0);
  }

void menuAbout(void)
  {
    destroyBalls();
    messages.about();
    PA_WaitFor(Pad.Newpress.Anykey || Stylus.Newpress);
    initMenu();
  }

void play(void)
  {
    moveBalls();

    moveWbd();
    
    buildWall(&blueWall, wbd.angle%2 ? 0 : -1,
        wbd.angle%2 ? -1 : 0, TILE_BLUE, SPRITE_BLUE);
    
    buildWall(&redWall, wbd.angle%2 ? 0 : 1,
        wbd.angle%2 ? 1 : 0, TILE_RED, SPRITE_RED);
    
    timeLeft = time4lvl - (u32) (((double) ((PA_VBLCounter[0] * speed)>>8)) / 6.);
    if (timeLeft <= 0)
      {
        gameover = 1;
        timeLeft = 0;
        messages.gameover();
      }
    
    messages.score(level, lives, complete, timeLeft, (levelScore + totalScore + 5) / 10, (bonus + 5) / 10);
  }

void promptHighscore(void)
  {
    if (totalScore > 0)
      {
        u8 i;
        
        for (i=0; i<NB_HIGHSCORES; i++)
          {
            if (saveData.highscores[i].score < totalScore)
              {
                u8 j, nletter = 0, typing = 1, blink = 0, len;
                char letter;

                for (j=9; j>i; j--)
                  saveData.highscores[j] = saveData.highscores[j-1];
                saveData.highscores[i].score = totalScore;
                saveData.highscores[i].level = level;
                
                messages.newhighscore();
                PA_WaitFor(Pad.Newpress.A || Stylus.Newpress);
                
                PA_InitCustomKeyboard(0, keyboardcustom);
                PA_KeyboardIn(24, 95);
                
                for (nletter=0; nletter<MAX_NAME_LEN && PA_UserInfo.Name[nletter] != '\0'; nletter++)
                  saveData.highscores[i].name[nletter] = PA_UserInfo.Name[nletter];
                len = nletter;
                for (; nletter<MAX_NAME_LEN; nletter++)
                  saveData.highscores[i].name[nletter] = ' ';
                saveData.highscores[i].name[MAX_NAME_LEN] = '\0';
                nletter = len;
                
                PA_VBLCounterStart(0);
                
                while (typing)
                  {
                    letter = PA_CheckKeyboard();
                    if (letter > 31)
                      { // there is a new letter
                        saveData.highscores[i].name[nletter] = letter;
                        nletter++;
                      }
                    else if ((letter == PA_BACKSPACE) && nletter)
                      { // Backspace pressed
                        nletter--;
                        saveData.highscores[i].name[nletter] = ' '; // Erase the last letter
                      }
                    else if ((letter == '\n' && nletter) || Pad.Newpress.A)
                      { // Enter pressed
                        typing = 0;
                        saveSRAM();
                      }
                    if (nletter >= MAX_NAME_LEN)
                      nletter = MAX_NAME_LEN - 1;
                    
                    if ((PA_VBLCounter[0] & 15) == 0)
                      {
                        messages.highscores(blink ? i : -1, saveData.highscores);
                        blink = !blink;
                      }
                    
                    PA_WaitForVBL();
                  }
                break;
              }
          }
      }
  }

void loadSaveData(void)
  {
    int fd, ret;
    fd = open("JezzballDS.dat", O_RDONLY|O_CREAT);
    ret = read(fd, &saveData, sizeof(saveData));
    close(fd);
  }

void saveSRAM(void)
  {
    int fd, ret;
    fd = open("JezzballDS.dat", O_WRONLY|O_CREAT);
    ret = write(fd, &saveData, sizeof(saveData));
    close(fd);
  }

u16 checkCollision(s16 x, s16 y)
  {
    s8 x2, y2;
    
    x2 = x>>3;
    y2 = y>>3;
    
    if (x2 <= -1 || y2 <= -1 || x2 >= 32 || y2 >= 24)
      return TILE_CLEARED;
    else
      return collision[y2*width+x2];
  }

void moveBalls(void)
  {
    s8 i;
    u8 bouncePlayed = 0, jezzdeadPlayed = 0;

    for (i=0; i<nbBalls; i++)
      {
        u16 left = TILE_DEFAULT, top = TILE_DEFAULT, right = TILE_DEFAULT, bottom = TILE_DEFAULT, check;

        // left
        check = checkCollision((balls[i].x - 1), (balls[i].y + 4));
        if (check != TILE_DEFAULT)
          {
            left = check;
          }

        // top left
        check = checkCollision((balls[i].x + 1), (balls[i].y + 1));
        if (check != TILE_DEFAULT)
          {
            top = left = check;
          }

        // top
        check = checkCollision((balls[i].x + 4), (balls[i].y - 1));
        if (check != TILE_DEFAULT)
          {
            top = check;
          }

        // right top
        check = checkCollision((balls[i].x + 7), (balls[i].y + 1));
        if (check != TILE_DEFAULT)
          {
            right = top = check;
          }

        // right
        check = checkCollision((balls[i].x + 8), (balls[i].y + 4));
        if (check != TILE_DEFAULT)
          {
            right = check;
          }

        // bottom right
        check = checkCollision((balls[i].x + 7), (balls[i].y + 7));
        if (check != TILE_DEFAULT)
          {
            bottom = right = check;
          }

        // bottom
        check = checkCollision((balls[i].x + 4), (balls[i].y + 8));
        if (check != TILE_DEFAULT)
          {
            bottom = check;
          }

        // left bottom
        check = checkCollision((balls[i].x + 1), (balls[i].y + 7));
        if (check != TILE_DEFAULT)
          {
            left = bottom = check;
          }

        if (top == TILE_CLEARED || bottom == TILE_CLEARED || right == TILE_CLEARED || left == TILE_CLEARED)
          {
            if (!bouncePlayed)
              {
                PA_PlaySimpleSound(PA_GetFreeSoundChannel(), bounce);
                bouncePlayed = 1;
              }
          }
        
        if (top == TILE_BLUE || bottom == TILE_BLUE || right == TILE_BLUE || left == TILE_BLUE)
          {
            lives --;
            if (lives == 0)
              {
                gameover = 1;
                messages.gameover();
              }
            fillWall(&blueWall, TILE_DEFAULT, SPRITE_BLUE);
            
            if (!jezzdeadPlayed)
              {
                PA_PlaySimpleSound(PA_GetFreeSoundChannel(), jezzdead);
                jezzdeadPlayed = 1;
              }
          }
        if (top == TILE_RED || bottom == TILE_RED || right == TILE_RED || left == TILE_RED)
          {
            lives --;
            if (lives == 0)
              {
                gameover = 1;
                messages.gameover();
              }
            fillWall(&redWall, TILE_DEFAULT, SPRITE_RED);
            
            if (!jezzdeadPlayed)
              {
                PA_PlaySimpleSound(PA_GetFreeSoundChannel(), jezzdead);
                jezzdeadPlayed = 1;
              }
          }
        
        if (!(top == TILE_CLEARED && bottom == TILE_CLEARED) && !(right == TILE_CLEARED && left == TILE_CLEARED))
          {
            if (right == TILE_CLEARED)
              balls[i].xdirection = -1;
            else if (left == TILE_CLEARED)
              balls[i].xdirection = 1;

            if (bottom == TILE_CLEARED)
              balls[i].ydirection = -1;
            else if (top == TILE_CLEARED)
              balls[i].ydirection = 1;

            balls[i].vx += balls[i].xdirection * speed;
            balls[i].vy += balls[i].ydirection * speed;
            balls[i].x = balls[i].vx >> 8;
            balls[i].y = balls[i].vy >> 8;
          }
        else
          {
            // stop the ball
            
            u8 modulo;

            balls[i].xdirection = balls[i].ydirection = 0;

            modulo = balls[i].x & 7;
            if (modulo <= 4)
              {
                balls[i].x -= modulo;
                balls[i].vx = balls[i].x << 8;
              }
            else
              {
                balls[i].x = balls[i].x - modulo + 8;
                balls[i].vx = balls[i].x << 8;
              }

            modulo = balls[i].y & 7;
            if (modulo <= 4)
              {
                balls[i].y -= modulo;
                balls[i].vy = balls[i].y << 8;
              }
            else
              {
                balls[i].y = balls[i].y - modulo + 8;
                balls[i].vy = balls[i].y << 8;
              }
          }

        // balls collision
        u8 j;
        for (j=0; j<i; j++)
          { // test collisions for all balls with a smaller number...
            if (PA_Distance(balls[i].x, balls[i].y, balls[j].x, balls[j].y) < 8*8)
              {
                balls[i].xdirection = balls[i].x - balls[j].x > 0 ? 1 : -1;
                balls[i].ydirection = balls[i].y - balls[j].y > 0 ? 1 : -1;
                balls[j].xdirection = -(balls[i].xdirection);
                balls[j].ydirection = -(balls[i].ydirection);
                
                if (!bouncePlayed)
                  {
                    PA_PlaySimpleSound(PA_GetFreeSoundChannel(), bounce);
                    bouncePlayed = 1;
                  }
             }
          }
        
        PA_SetSpriteXY(0, i, balls[i].x, balls[i].y);
      }
  }

void buildWall(wall * wall, s16 xstep, s16 ystep, u16 tilenum, u8 spritenum)
  {
    s8 x, y;

    if (complete < 75 && (Pad.Newpress.A || (Stylus.Released && Stylus.Downtime < 10 && abs(Stylus.Vx) < 10 && abs(Stylus.Vy) < 10)))
      {
        x = ((wbd.x)>>3) + (xstep>0 ? 1 : 0);
        y = ((wbd.y)>>3) + (ystep>0 ? 1 : 0);

        if (x >= 0 && y >= 0 && x < 32 && y < 24 && wall->xdirection == 0 && wall->ydirection == 0 && collision[y*width+x] != TILE_CLEARED)
          {
            wall->xdirection = xstep;
            wall->ydirection = ystep;

            wall->x1 = wall->x2 = (x<<3);
            wall->y1 = wall->y2 = (y<<3);
            wall->vx = wall->x2 << 8;
            wall->vy = wall->y2 << 8;

            PA_SetMapTile(0, 1, x, y, tilenum);
            collision[y*width+x] = tilenum;
            
            PA_SetSpriteXY(0, spritenum, wall->x2, wall->y2);
          }
      }
    else if (wall->xdirection != 0 || wall->ydirection != 0)
      {
        s8 xnext, ynext;
        
        x = xnext = ((wall->x2 + 4)>>3);
        y = ynext = ((wall->y2 + 4)>>3);
        if (wall->xdirection != 0)
          xnext += wall->xdirection > 0 ? 1 : -1;
        if (wall->ydirection != 0)
          ynext += wall->ydirection > 0 ? 1 : -1;
        
        if ( (wall->xdirection > 0 && wall->x2 == 248)
            || (wall->xdirection < 0 && wall->x2 == 0)
            || (wall->ydirection > 0 && wall->y2 == 184)
            || (wall->ydirection < 0 && wall->y2 == 0)
            || (collision[ynext*width+xnext] == TILE_CLEARED && (wall->x2 & 7) == 0 && (wall->y2 & 7) == 0))
          {
            fillWall(wall, TILE_CLEARED, spritenum);
            clearEmptyRooms();
          }
        else
          {
            wall->vx += wall->xdirection * speed;
            wall->vy += wall->ydirection * speed;
            wall->x2 = wall->vx >> 8;
            wall->y2 = wall->vy >> 8;

            if ((wall->x2 & 7) == 0 && (wall->y2 & 7) == 0)
            {
              x = ((wall->x2 + 4)>>3);
              y = ((wall->y2 + 4)>>3);
              
              PA_SetMapTile(0, 1, x, y, tilenum);
              collision[y*width+x] = tilenum;
            }
          }
        
        PA_SetSpriteXY(0, spritenum, wall->x2, wall->y2);
      }
  }

void fillWall(wall * wall, u16 tilenum, u8 spritenum)
  {
    s8 x, y, start, end, step;
    
    if (wall->xdirection != 0)
      {
        start = ((wall->x1)>>3);
        step = wall->xdirection;
        end = ((wall->x2)>>3) + step;
        y = (wall->y1)>>3;
        
        for (x=start; x!=end; x+=step)
          {
            PA_SetMapTile(0, 1, x, y, tilenum == TILE_DEFAULT ? background_Map[y*width+x] : tilenum);
            collision[y*width+x] = tilenum;
          }
      }
    else if (wall->ydirection != 0)
      {
        start = ((wall->y1)>>3);
        step = wall->ydirection;
        end = ((wall->y2)>>3) + step;
        x = (wall->x1)>>3;
        
        for (y=start; y!=end; y+=step)
          {
            PA_SetMapTile(0, 1, x, y, tilenum);
            collision[y*width+x] = tilenum;
            if (tilenum == TILE_CLEARED)
              {
                columns[x]++;
                rows[y]++;
              }
          }
      }

    wall->x1 = wall->x2 = wall->y1 = wall->y2 = -8;
    wall->vx = wall->vy = -8 << 3;
    wall->xdirection = wall->ydirection = 0;
    
    PA_SetSpriteXY(0, spritenum, -8, -8);
    
    checkIfWin();
  }

void initBalls(void)
  {
    u8 i;

    for (i=0; i<nbBalls; i++)
      {
        u8 x, y;
        
        balls[i].xdirection = PA_RandMinMax(0, 1) ? -1 : 1;
        balls[i].ydirection = PA_RandMinMax(0, 1) ? -1 : 1;

        x = PA_RandMinMax(1, 30);
        y = PA_RandMinMax(1, 22);
        while (collision[y*width+x] == TILE_CLEARED)
          {
            x = PA_RandMinMax(1, 30);
            y = PA_RandMinMax(1, 22);
          }
        balls[i].x = x << 3;
        balls[i].y = y << 3;
        balls[i].vx = balls[i].x << 8;
        balls[i].vy = balls[i].y << 8;
        
        PA_CreateSprite(0, i, (void*) ball_Sprite, OBJ_SIZE_8X8, 1 , 0, balls[i].x, balls[i].y);
        PA_SetSpritePrio(0, i, 1);
        PA_StartSpriteAnim(0, i, 0, 7, 8);

        PA_SetSpriteAnim(0, i, PA_RandMinMax(0,7));
      }
  }

void initWbd(void)
  {
    wbd.x = 120;
    wbd.y = 88;
    wbd.angle = 0;

    PA_CreateSprite(0, SPRITE_BLUE, (void*) blue_Sprite, OBJ_SIZE_8X8, 1, 0, -8, -8);
    PA_SetSpritePrio(0, SPRITE_BLUE, 1);
    PA_CreateSprite(0, SPRITE_RED, (void*) red_Sprite, OBJ_SIZE_8X8, 1, 0, -8, -8);
    PA_SetSpritePrio(0, SPRITE_RED, 1);
    PA_CreateSprite(0, SPRITE_WBD, (void*) wbd_Sprite, OBJ_SIZE_16X16,1 , 0, wbd.x, wbd.y);
    PA_SetSpritePrio(0, SPRITE_WBD, 1);
    PA_StartSpriteAnim(0, SPRITE_WBD, 0, 5, 18);
    
    fillWall(&blueWall, TILE_DEFAULT, SPRITE_BLUE);
    fillWall(&redWall, TILE_DEFAULT, SPRITE_RED);
  }

void moveWbd(void)
  {
    s8 x, y;
    if (Stylus.Held)
      {
        wbd.x = Stylus.X;
        wbd.y = Stylus.Y;

        x = wbd.x>>3;
        y = wbd.y>>3;
      }
    
    if (Pad.Newpress.Right || Pad.Newpress.Left || Pad.Newpress.Down
        || Pad.Newpress.Up)
      {
        wbd.x = ((wbd.x>>3)<<3) + Pad.Newpress.Right * 8
            - Pad.Newpress.Left * 4;
        wbd.y = ((wbd.y>>3)<<3) + Pad.Newpress.Down * 8
            - Pad.Newpress.Up * 4;
      }
    else
      {
        wbd.x += Pad.Held.Right - Pad.Held.Left;
        wbd.y += Pad.Held.Down - Pad.Held.Up;
      }
    
    x = (wbd.x)>>3;
    y = (wbd.y)>>3;
    if (x < ((!wbd.angle) * -1))
      {
        x = ((!wbd.angle) * -1);
        wbd.x = x<<3;
      }
    else if (x > 31)
      {
        x = 31;
        wbd.x = x<<3;
      }
    if (y < (wbd.angle * -1))
      {
        y = (wbd.angle * -1);
        wbd.y = y<<3;
      }
    else if (y > 23)
      {
        y = 23;
        wbd.y = y<<3;
      }

    if (Pad.Newpress.L || Pad.Newpress.R)
      {
        wbd.angle++;
        wbd.angle &= 1;

        if (wbd.angle)
          PA_StartSpriteAnim(0, SPRITE_WBD, 6, 11, 18);
        else
          PA_StartSpriteAnim(0, SPRITE_WBD, 0, 5, 18);
      }
    
    PA_SetSpriteXY(0, SPRITE_WBD, (wbd.x>>3)<<3, (wbd.y>>3)<<3);

  }

void clearEmptyRooms(void)
  {
    u8 cleared[32][24];
    
    u16 tilesComplete;
    u8 change = 1;
    s8 x, y, i;

    for (y=0; y<24; y++)
      {
        rows[y] = 0;
        for (x=0; x<32; x++)
          {
            rows[x] = 0;
            cleared[x][y] = 1;
          }
      }

    for (i=0; i<nbBalls; i++)
      {
        x = (balls[i].x + 4)>>3;
        y = (balls[i].y + 4)>>3;

        cleared[x][y] = 0;
      }

    while (change)
      {
        change = 0;
        for (y=0; y<24; y++)
          for (x=0; x<32; x++)
            {
              if (!cleared[x][y])
                {
                  if (cleared[x][y+1] && (y+1) < 24 && collision[(y+1)*width+(x)] != TILE_CLEARED)
                    {
                      cleared[x][y+1] = 0;
                      change = 1;
                    }
                  if (cleared[x+1][y] && (x+1) < 32 && collision[(y)*width+(x+1)] != TILE_CLEARED)
                    {
                      cleared[x+1][y] = 0;
                      change = 1;
                    }
                }
            }
        for (y=23; y>=0; y--)
          for (x=31; x>=0; x--)
            {
              if (!cleared[x][y])
                {
                  if (cleared[x][y-1] && (y-1) >= 0 && collision[(y-1)*width+(x)] != TILE_CLEARED)
                    {
                      cleared[x][y-1] = 0;
                      change = 1;
                    }
                  if (cleared[x-1][y] && (x-1) >= 0 && collision[(y)*width+(x-1)] != TILE_CLEARED)
                    {
                      cleared[x-1][y] = 0;
                      change = 1;
                    }
                }
            }
      }

    tilesComplete = 0;
    for (y=0; y<24; y++)
      for (x=0; x<32; x++)
        if (cleared[x][y])
          {
            PA_SetMapTile(0, 1, x, y, TILE_CLEARED);
            collision[y*width+x] = TILE_CLEARED;
            if (background_Map[y*width+x] != TILE_CLEARED)
              tilesComplete++;
            columns[x]++;
            rows[y]++;
          }
        else if (collision[y*width+x] == TILE_CLEARED)
          {
            if (background_Map[y*width+x] != TILE_CLEARED)
              tilesComplete++;
            columns[x]++;
            rows[y]++;
          }

    complete = (u8) ((tilesComplete*100)/tilesToClear + .5);
    
    levelScore = tilesComplete * (30 + 5 * level);
    
    checkIfWin();
  }

void checkIfWin(void)
  {
    if (complete >= 75 && blueWall.xdirection == 0 && blueWall.ydirection == 0 && redWall.xdirection == 0 && redWall.ydirection == 0)
      {
        s8 x, y;
        
        for (y=0; y<24; y++)
          if (rows[y] == 32)
            bonus += (30 + 5 * level) * 32;
        for (x=0; x<32; x++)
          if (columns[x] == 24)
            bonus += (30 + 5 * level) * 24;
        
        bonus += (timeLeft * lives * level * speed) >> 8;
        bonus += (((complete - 79) * level * speed) >> 8) * 100;
        bonus += (((complete - 89) * level * speed) >> 8) * 200;
        
        levelScore += bonus;
        
        messages.levelcomplete();
        levelComplete = 1;
      }
  }

void destroyBalls(void)
  {
    u8 i;

    for (i=0; i<nbBalls; i++)
      {
        PA_StopSpriteAnim(0, i);
        PA_DeleteSprite(0, i);
      }
  }

void destroyWbd(void)
  {
    PA_DeleteSprite(0, SPRITE_BLUE);
    PA_DeleteSprite(0, SPRITE_RED);
    PA_StopSpriteAnim(0, SPRITE_WBD);
    PA_DeleteSprite(0, SPRITE_WBD);
  }

void initLevel(u8 level)
  {
    u8 i, j;

    nbBalls = level + 1;
    lives = nbBalls;
    complete = 0;
    time4lvl = 1300;
    for (i=0; i<level; i++)
      time4lvl += (i&1) * 100 + 200;
    timeLeft = time4lvl;
    levelScore = 0;
    levelComplete = 0;
    bonus = 0;
    
    tilesToClear = 0;
    for (i=0; i<32; i++)
      {
        for (j=0; j<24; j++)
          {
            if (background_Map[j*width+i] == TILE_CLEARED)
              {
                collision[j*width+i] = TILE_CLEARED;
              }
            else
              {
                collision[j*width+i] = TILE_DEFAULT;
                tilesToClear++;
              }
          }
      }
    initBalls();
    initWbd();

    PA_ResetBgSys();
    PA_EasyBgLoad(0, 1, background);
    PA_EasyBgScrollXY(0, 1, 0, 0);

    PA_InitCustomText(0, 0, newfont);
    PA_InitCustomText(1, 0, newfont);
    
    messages.title();
    messages.score(level, lives, complete, timeLeft, (levelScore + totalScore + 5) / 10, (bonus + 5) / 10);
  }


void fadeIn(u8 screen0, u8 screen1)
  {
    s8 i;
    
    // Back in from black...
    for (i = -31; i <= 0; i++)
      {
        if (screen0)
          PA_SetBrightness(0, i);
        if (screen1)
          PA_SetBrightness(1, i);
        PA_WaitForVBL(); // To slow down the fades, we wait a frame...
        PA_WaitForVBL(); 
      }       
  }

void fadeOut(u8 screen0, u8 screen1)
  {
    s8 i;
    
    // Fade out to black
    for (i = 0; i >= -31; i--)
      {
        if (screen0)
          PA_SetBrightness(0, i);
        if (screen1)
          PA_SetBrightness(1, i);
        PA_WaitForVBL(); // To slow down the fades, we wait a frame...
        PA_WaitForVBL(); 
      }       
  }

void initMenu(void)
  {
    inMenu = 1;
    currentMenuItem = 0;
    
    PA_ResetBgSys();

    PA_InitCustomText(0, 0, newfont);
    PA_InitCustomText(1, 0, newfont);
    
    messages.title();
    messages.highscores(-1, saveData.highscores);
    
    u8 i;

    PA_ClearBg(0, 0);
    
    messages.menu();
    
    nbBalls = 2;
    
    for (i=0; i<nbBalls; i++)
      {
        PA_CreateSprite(0, i, (void*) ball_Sprite, OBJ_SIZE_8X8, 1 , 0, -8, -8);
        PA_StartSpriteAnim(0, i, 0, 7, 8);

        PA_SetSpriteAnim(0, i, i*3);
      }
    updateCurrentMenu();
  }

void updateCurrentMenu(void)
  {
    u8 i;
    
    for (i=0; i<nbBalls; i++)
      {
        u8 x, y;
        
        x = 8 + i * 15;
        y = 4 + currentMenuItem * 6;
        
        PA_SetSpriteXY(0, i, x << 3, y << 3);
      }
  }
