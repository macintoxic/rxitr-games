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

#ifndef JEZZBALL_H
#define JEZZBALL_H

// save
#define MAX_NAME_LEN 15
#define NB_HIGHSCORES 10
#define SAVE_SIGNATURE "JezzballDS"


#define BG_INFO_TYPE 0
#define BG_INFO_WIDTH 1
#define BG_INFO_HEIGHT 2

#define MAX_NB_BALLS 32

#define SPRITE_WBD 32
#define SPRITE_BLUE 33
#define SPRITE_RED 34

#define TILE_DEFAULT background_Map[1020]
#define TILE_CLEARED background_Map[1021]
#define TILE_BLUE background_Map[1022]
#define TILE_RED background_Map[1023]

#define MENU_PLAY 0
#define MENU_OPTIONS 1
#define MENU_ABOUT 2

typedef struct
  {
    u32 score;
    u8 level;
    char name[MAX_NAME_LEN+1];
  } score;

typedef struct
  {
    u8 options[10];
    score highscores[NB_HIGHSCORES];
  } save;

typedef struct
  {
    s16 x, y;
    s32 vx, vy;
    s8 xdirection, ydirection;
    u8 angle;
  } sprite;

typedef struct
  {
    s16 x1, y1, x2, y2;
    s32 vx, vy;
    s8 xdirection, ydirection;
  } wall;


#endif
