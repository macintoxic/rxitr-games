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

#ifndef MESSAGES_H_
#define MESSAGES_H_

#include "jezzball.h"

#define LANG_JP 0
#define LANG_EN 1
#define LANG_FR 2

#define TILE_LEFT_ARROW background_Map[1022]
#define TILE_RIGHT_ARROW background_Map[1023]


typedef struct
  {
    void (*menu)(void);
    void (*options)(void);
    void (*optionsValues)(u8 * options, u8 selectedOption);
    void (*gameover)(void);
    void (*levelcomplete)(void);
    void (*score)(void);
    void (*pause)(void);
    void (*none)(void);
    void (*title)(void);
    void (*about)(void);
    void (*highscores)(u8 blink, score * highscores);
    void (*newhighscore)(void);
    void (*update_score)(u8 level, u8 lives, u8 complete, s16 timeLeft, u32 score, u32 bonus);
    void (*level_choice)(u8 level);
} l18n;

void initLang(l18n * messages);

void msg_menu_en(void);
void msg_options_en(void);
void msg_optionsValues_en(u8 * options, u8 selectedOption);
void msg_gameover_en(void);
void msg_levelcomplete_en(void);
void msg_score_en(void);
void msg_pause_en(void);
void msg_title_en(void);
void msg_about_en(void);
void msg_highscores_en(u8 blink, score * highscores);
void msg_newhighscore_en(void);
void msg_level_choice_en(u8 level);

void msg_menu_fr(void);
void msg_options_fr(void);
void msg_optionsValues_fr(u8 * options, u8 selectedOption);
void msg_gameover_fr(void);
void msg_levelcomplete_fr(void);
void msg_score_fr(void);
void msg_pause_fr(void);
void msg_title_fr(void);
void msg_about_fr(void);
void msg_highscores_fr(u8 blink, score * highscores);
void msg_newhighscore_fr(void);
void msg_level_choice_fr(u8 level);

void msg_none(void);
void msg_update_score(u8 level, u8 lives, u8 complete, s16 timeLeft, u32 score, u32 bonus);

#endif /*MESSAGES_H_*/
