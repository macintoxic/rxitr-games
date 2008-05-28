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

#include <PA9.h>
#include "messages.h"

#include "messages/all_gfx.c"
#include "messages/all_gfx.h"

void initLang(l18n * messages)
  {
    PA_InitText(1, 0);
    switch (PA_UserInfo.Language)
      {
    case LANG_FR:
      PA_OutputText(1, 0, 0, "chargement...");
      messages->menu = &msg_menu_fr;
      messages->options = &msg_options_fr;
      messages->optionsValues = &msg_optionsValues_fr;
      messages->gameover = &msg_gameover_fr;
      messages->levelcomplete = &msg_levelcomplete_fr;
      messages->score = &msg_score_fr;
      messages->pause = &msg_pause_fr;
      messages->none = &msg_none;
      messages->title = &msg_title_fr;
      messages->about = &msg_about_fr;
      messages->highscores = &msg_highscores_fr;
      messages->newhighscore = &msg_newhighscore_fr;
      break;
    default:
      PA_OutputText(1, 0, 0, "loading...");
      messages->menu = &msg_menu_en;
      messages->options = &msg_options_en;
      messages->optionsValues = &msg_optionsValues_en;
      messages->gameover = &msg_gameover_en;
      messages->levelcomplete = &msg_levelcomplete_en;
      messages->score = &msg_score_en;
      messages->pause = &msg_pause_en;
      messages->none = &msg_none;
      messages->title = &msg_title_en;
      messages->about = &msg_about_en;
      messages->highscores = &msg_highscores_en;
      messages->newhighscore = &msg_newhighscore_en;
      break;
      }
  }

void msg_none(void)
  {
    PA_HideBg(0, 0);
  }

void msg_gameover_en(void)
  {
    PA_ClearBg(0, 0);
    PA_EasyBgLoad(0, 0, msg_en_gameover);
    PA_ShowBg(0, 0);
  }

void msg_menu_en(void)
  {
    PA_ClearBg(0, 0);
    PA_EasyBgLoad(0, 0, msg_en_menu);
    PA_ShowBg(0, 0);
  }

void msg_menu_fr(void)
  {
    PA_ClearBg(0, 0);
    PA_EasyBgLoad(0, 0, msg_fr_menu);
    PA_ShowBg(0, 0);
  }

void msg_options_en(void)
  {
    PA_ClearBg(0, 0);
    PA_EasyBgLoad(0, 1, msg_en_options);
    PA_ShowBg(0, 0);
  }

void msg_options_fr(void)
  {
    PA_ClearBg(0, 0);
    PA_EasyBgLoad(0, 1, msg_en_options);
    PA_ShowBg(0, 0);
  }

void msg_optionsValues_en(u8 * options, u8 selectedOption)
  {
    PA_OutputText(0, 2, 5, "Sound:             %s        %s", selectedOption == OPTION_SOUNDFX ? "<" : " ", selectedOption == OPTION_SOUNDFX ? ">" : " ");
    switch (options[OPTION_SOUNDFX])
      {
    case OPTIONVALUE_SOUNDFX_YES:
      PA_OutputText(0, 23, 5, "%s", "Yes");
      break;
    case OPTIONVALUE_SOUNDFX_NO:
      PA_OutputText(0, 23, 5, "%s", "No");
      break;
      }
    
    PA_OutputText(0, 2, 7, "Speed:             %s        %s", selectedOption == OPTION_SPEED   ? "<" : " ", selectedOption == OPTION_SPEED   ? ">" : " ");
    switch (options[OPTION_SPEED])
      {
    case OPTIONVALUE_SPEED_SLOW:
      PA_OutputText(0, 23, 7, "%s", "Slow");
      break;
    case OPTIONVALUE_SPEED_MEDIUM:
      PA_OutputText(0, 23, 7, "%s", "Medium");
      break;
    case OPTIONVALUE_SPEED_FAST:
      PA_OutputText(0, 23, 7, "%s", "Fast");
      break;
      }
  }

void msg_optionsValues_fr(u8 * options, u8 selectedOption)
  {
    PA_OutputText(0, 2, 5, "Son:               %s        %s", selectedOption == OPTION_SOUNDFX ? "<" : " ", selectedOption == OPTION_SOUNDFX ? ">" : " ");
    switch (options[OPTION_SOUNDFX])
      {
    case OPTIONVALUE_SOUNDFX_YES:
      PA_OutputText(0, 23, 5, "%s", "Oui");
      break;
    case OPTIONVALUE_SOUNDFX_NO:
      PA_OutputText(0, 23, 5, "%s", "Non");
      break;
      }
    
    PA_OutputText(0, 2, 7, "Vitesse:           %s        %s", selectedOption == OPTION_SPEED   ? "<" : " ", selectedOption == OPTION_SPEED   ? ">" : " ");
    switch (options[OPTION_SPEED])
      {
    case OPTIONVALUE_SPEED_SLOW:
      PA_OutputText(0, 23, 7, "%s", "Lent");
      break;
    case OPTIONVALUE_SPEED_MEDIUM:
      PA_OutputText(0, 23, 7, "%s", "Moyen");
      break;
    case OPTIONVALUE_SPEED_FAST:
      PA_OutputText(0, 23, 7, "%s", "Rapide");
      break;
      }
  }

void msg_gameover_fr(void)
  {
    PA_ClearBg(0, 0);
    PA_EasyBgLoad(0, 0, msg_fr_gameover);
    PA_ShowBg(0, 0);
  }

void msg_levelcomplete_en(void)
  {
    PA_ClearBg(0, 0);
    PA_EasyBgLoad(0, 0, msg_en_levelcomplete);
    PA_ShowBg(0, 0);
  }

void msg_levelcomplete_fr(void)
  {
    PA_ClearBg(0, 0);
    PA_EasyBgLoad(0, 0, msg_fr_levelcomplete);
    PA_ShowBg(0, 0);
  }

void msg_pause_en(void)
  {
    PA_ClearBg(0, 0);
    PA_EasyBgLoad(0, 0, msg_en_pause);
    PA_ShowBg(0, 0);
  }

void msg_pause_fr(void)
  {
    PA_ClearBg(0, 0);
    PA_EasyBgLoad(0, 0, msg_fr_pause);
    PA_ShowBg(0, 0);
  }

void msg_newhighscore_en(void)
  {
    PA_ClearBg(0, 0);
    PA_EasyBgLoad(0, 0, msg_en_newhighscore);
    PA_ShowBg(0, 0);
  }

void msg_newhighscore_fr(void)
  {
    PA_ClearBg(0, 0);
    PA_EasyBgLoad(0, 0, msg_fr_newhighscore);
    PA_ShowBg(0, 0);
  }

void msg_title_en(void)
  {
    PA_ClearBg(1, 1);
    PA_EasyBgLoad(1, 1, msg_en_title);
    PA_ShowBg(1, 1);
  }

void msg_title_fr(void)
  {
    PA_ClearBg(1, 1);
    PA_EasyBgLoad(1, 1, msg_fr_title);
    PA_ShowBg(1, 1);
  }

void msg_score_en(u8 level, u8 lives, u8 complete, s16 timeLeft, u32 score, u32 bonus)
  {
    PA_OutputText(1, 2, 8,  "Level:             %§9d", level);
    PA_OutputText(1, 2, 10,  "Lives:             %§9d", lives);
    PA_OutputText(1, 2, 12, "Area cleared:     %§9d%", complete);
    PA_OutputText(1, 2, 14, "Time left:         %§9d", timeLeft);
    PA_OutputText(1, 2, 16, "Score:             %§9d", score);
    PA_OutputText(1, 2, 18, "Bonus:             %§9d", bonus);
  }

void msg_score_fr(u8 level, u8 lives, u8 complete, s16 timeLeft, u32 score, u32 bonus)
  {
    PA_OutputText(1, 2, 8,  "Niveau:            %§9d", level);
    PA_OutputText(1, 2, 10,  "Vies:              %§9d", lives);
    PA_OutputText(1, 2, 12, "Zone nettoyée:    %§9d%", complete);
    PA_OutputText(1, 2, 14, "Temps restant:     %§9d", timeLeft);
    PA_OutputText(1, 2, 16, "Score:             %§9d", score);
    PA_OutputText(1, 2, 18, "Bonus:             %§9d", bonus);
  }

void msg_about_en(void)
  {
    PA_ClearBg(0, 0);
    PA_EasyBgLoad(0, 0, msg_en_about);
    PA_ShowBg(0, 0);
  }

void msg_about_fr(void)
  {
    PA_ClearBg(0, 0);
    PA_EasyBgLoad(0, 0, msg_fr_about);
    PA_ShowBg(0, 0);
  }

void msg_highscores_en(u8 blink, score * highscores)
  {
    u8 i;
    
    PA_ClearTextBg(1);
    
    PA_EasyBgLoad(1, 1, msg_en_title_highscores);
    
    for (i=0; i<NB_HIGHSCORES; i++)
      {
        if (highscores[i].score > 0)
          {
            if (!(blink == i))
              PA_OutputText(1, 2, i+11,  "%s %§2d %§9d", highscores[i].name, highscores[i].level, (u32) ((highscores[i].score + 5)/10));
          }
        else
          {
            PA_OutputText(1, 2, i+11,  "--------------- -- ---------");
          }
      }
  }

void msg_highscores_fr(u8 blink, score * highscores)
  {
    u8 i;
    
    PA_ClearTextBg(1);
    
    PA_EasyBgLoad(1, 1, msg_fr_title_highscores);
    
    for (i=0; i<NB_HIGHSCORES; i++)
      {
        if (highscores[i].score > 0)
          {
            if (!(blink == i))
              PA_OutputText(1, 2, i+11,  "%s %§2d %§9d", highscores[i].name, highscores[i].level, (u32) ((highscores[i].score + 5)/10));
          }
        else
          {
            PA_OutputText(1, 2, i+11,  "--------------- -- ---------");
          }
      }
  }
