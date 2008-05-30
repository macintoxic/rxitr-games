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
#define SAVE_SIGNATURE "JezzballDS_1.1.0"


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

// Options {
typedef enum
  {
     OPTION_SOUNDFX
    ,OPTION_SPEED
    ,OPTION_BALLS
    ,OPTION_STYLUSMODE
    ,OPTIONS_COUNT
  } E_OPTIONS;

typedef enum
  {
     OPTION_SOUNDFX_YES
    ,OPTION_SOUNDFX_NO
    ,OPTIONS_SOUNDFX_COUNT
  } E_OPTIONS_SOUNDFX;

typedef enum
  {
     OPTION_SPEED_SLOW
    ,OPTION_SPEED_MEDIUM
    ,OPTION_SPEED_FAST
    ,OPTIONS_SPEED_COUNT
  } E_OPTIONS_SPEED;

typedef enum
  {
     OPTION_BALLS_CLASSIC
    ,OPTION_BALLS_SMILEY
    ,OPTIONS_BALLS_COUNT
  } E_OPTIONS_BALLS;

typedef enum
  {
     OPTION_STYLUSMODE_CLICK
    ,OPTION_STYLUSMODE_STRETCH
    ,OPTIONS_STYLUSMODE_COUNT
  } E_OPTIONS_STYLUSMODE;
// } Options

typedef struct
  {
    u32 score;
    u8 level;
    char name[MAX_NAME_LEN+1];
  } score;

typedef struct
  {
    u8 options[OPTIONS_COUNT];
    score highscores[NB_HIGHSCORES];
  } save;

typedef struct
  {
    s16 x, y;
    s32 vx, vy;
    s8 xdirection, ydirection;
    u8 angle, stretched;
  } sprite;

typedef struct
  {
    s16 x1, y1, x2, y2;
    s32 vx, vy;
    s8 xdirection, ydirection;
  } wall;


#endif
