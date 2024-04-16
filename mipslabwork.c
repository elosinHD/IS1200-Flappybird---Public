/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog

This file was modified 2024-02-28 by Isak Ahlberg and Erik Losin

   For copyright and licensing, see file COPYING */

#include <stdint.h>  /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "flappy.h"  /* Declatations for these labs */

int mytime = 0x5957;

char textstring[] = "text, more text, and even more text!";


// WRITTEN BY ERIK LOSIN
#define obstaclex_constant 0
#define startingy_constant 11
int timeoutcount = 0;
int startingy = startingy_constant;
int obstacle_x = obstaclex_constant;
int score = 0;
int GAME_ENDED = 0;
int first_time = 1;


/* Interrupt Service Routine */
void user_isr(void)
{
  if (first_time == 1)
  {
    int pressbutton = 0;
    display_string(0, "FLAPPY BIRD");
    display_string(2, "Hold down BTN4");
    display_string(3, "to Play");
    text_update();
    while (pressbutton < 100)
  {
    delay(10);
    if (getbtns() == 0b100)
    {
      pressbutton++;
    }
    else 
    {
      pressbutton = 0;
    }
    
  }
  first_time = 0;
  }
  
  

  clear_display();
  if (EnterButton())
  {
    startingy -= 4;
    birdtodisplay(startingy);
  }
  else
  {
    startingy += 1;
    birdtodisplay(startingy);
  }
  obstacletodisplay(obstacle_x);
  obstacle_x++;
  clear_bug_display();
  if (GAME_ENDED == 0)
  {
    collisioncheck();
  }

  // SCORE by ERIK LOSIN
  if (score_check() == 1)
  {
    score++;
  }
  IFSCLR(0) = 0x100;
}
// MODIFIED FROM LABB 3 BY ISAK AHLBERG & ERIK LOSIN
void TimerInit(void)
{
  IFSCLR(0) = 0x100;
  TMR2 = 0;
  T2CON = 0x8070;
  PR2 = ((80000000) / 256) / 10;
  T2CONSET = 0x08000;
  IECSET(0) = 0x100;
  IPCSET(2) = 0x1f;
  enable_interrupt();
  return;
}
//WRIITEN BY ISAK AHLBERG & ERIK LOSIN
void collisioncheck()
{
#define BirdHeight 5
#define BirdWidth 6
#define BirdPositionX_back 29
#define BirdPositionX_front 35
  // checking if bird has flown too high up or too far down
  if (startingy < 0 || startingy > 32 - BirdHeight)
  {
    GAME_ENDED = 1;
    endgame();
  }
  else
  {
    // CHECK WITH COLLISION WITH OBSTACLE
    int i, j;
    for (i = startingy; i < (startingy + BirdHeight + 1); i++)
    {
      for (j = BirdPositionX_back; j < BirdPositionX_front; j++)
      {
        if ((Pixelsondisplay[i][j] == 1))
        {
          if (((flappybird[i - startingy][j - BirdPositionX_back]) == 0))
          {
            GAME_ENDED = 1;
            endgame();
          }
        }
      }
    }
  }
}

// WRITTEN BY ISAK AHLBERG
void restartgame()
{
  clear_display();
  score = 0;
  GAME_ENDED = 0;
}

// WRITTEN BY ERIK LOSIN
void endgame()
{
  startingy = startingy_constant;
  obstacle_x = obstaclex_constant;
  int pressbutton = 0;
  clear_display();
  display_string(0, "GAME OVER");
  display_string(2, "SCORE");
  display_string(3, to_string(score)); // put in the actual score
  text_update();
  while (pressbutton < 100)
  {
    delay(10);
    if (getbtns() == 0b100)
    {
      pressbutton++;
    }
    else 
    {
      pressbutton = 0;
    }
    
  }
  restartgame();
}