/* mipslabfunc.c
   This file written 2015 by F Lundevall
   Some parts are original code written by Axel Isaksson

This file was modified 2024-02-28 by Isak Ahlberg and Erik Losin

   For copyright and licensing, see file COPYING */

#include <stdint.h>  /* Declarations of uint_32 and the like */
#include <pic32mx.h> /* Declarations of system-specific addresses etc */
#include "flappy.h"  /* Declatations for these labs */
#include <stdio.h>
/* Declare a helper function which is local to this file */

//FROM LAB 3
static void num32asc(char *s, int);

#define DISPLAY_CHANGE_TO_COMMAND_MODE (PORTFCLR = 0x10)
#define DISPLAY_CHANGE_TO_DATA_MODE (PORTFSET = 0x10)

#define DISPLAY_ACTIVATE_RESET (PORTGCLR = 0x200)
#define DISPLAY_DO_NOT_RESET (PORTGSET = 0x200)

#define DISPLAY_ACTIVATE_VDD (PORTFCLR = 0x40)
#define DISPLAY_ACTIVATE_VBAT (PORTFCLR = 0x20)

#define DISPLAY_TURN_OFF_VDD (PORTFSET = 0x40)
#define DISPLAY_TURN_OFF_VBAT (PORTFSET = 0x20)

/* quicksleep:
   A simple function to create a small delay.
   Very inefficient use of computing resources,
   but very handy in some special cases. */

//FROM LAB 3
void quicksleep(int cyc)
{
  int i;
  for (i = cyc; i > 0; i--)
    ;
}

/* tick:
   Add 1 to time in memory, at location pointed to by parameter.
   Time is stored as 4 pairs of 2 NBCD-digits.
   1st pair (most significant byte) counts days.
   2nd pair counts hours.
   3rd pair counts minutes.
   4th pair (least significant byte) counts seconds.
   In most labs, only the 3rd and 4th pairs are used. */
//FROM LAB 3

void tick(unsigned int *timep)
{
  /* Get current value, store locally */
  register unsigned int t = *timep;
  t += 1; /* Increment local copy */

  /* If result was not a valid BCD-coded time, adjust now */

  if ((t & 0x0000000f) >= 0x0000000a)
    t += 0x00000006;
  if ((t & 0x000000f0) >= 0x00000060)
    t += 0x000000a0;
  /* Seconds are now OK */

  if ((t & 0x00000f00) >= 0x00000a00)
    t += 0x00000600;
  if ((t & 0x0000f000) >= 0x00006000)
    t += 0x0000a000;
  /* Minutes are now OK */

  if ((t & 0x000f0000) >= 0x000a0000)
    t += 0x00060000;
  if ((t & 0x00ff0000) >= 0x00240000)
    t += 0x00dc0000;
  /* Hours are now OK */

  if ((t & 0x0f000000) >= 0x0a000000)
    t += 0x06000000;
  if ((t & 0xf0000000) >= 0xa0000000)
    t = 0;
  /* Days are now OK */

  *timep = t; /* Store new value */
}

/* display_debug
   A function to help debugging.

   After calling display_debug,
   the two middle lines of the display show
   an address and its current contents.

   There's one parameter: the address to read and display.

   Note: When you use this function, you should comment out any
   repeated calls to display_image; display_image overwrites
   about half of the digits shown by display_debug.
*/
//FROM LAB 3
void display_debug(volatile int *const addr)
{
  display_string(1, "Addr");
  display_string(2, "Data");
  num32asc(&textbuffer[1][6], (int)addr);
  num32asc(&textbuffer[2][6], *addr);
}

uint8_t spi_send_recv(uint8_t data)
{
  while (!(SPI2STAT & 0x08))
    ;
  SPI2BUF = data;
  while (!(SPI2STAT & 1))
    ;
  return SPI2BUF;
}

//FROM LAB 3
void display_init(void)
{
  DISPLAY_CHANGE_TO_COMMAND_MODE;
  quicksleep(10);
  DISPLAY_ACTIVATE_VDD;
  quicksleep(1000000);

  spi_send_recv(0xAE);
  DISPLAY_ACTIVATE_RESET;
  quicksleep(10);
  DISPLAY_DO_NOT_RESET;
  quicksleep(10);

  spi_send_recv(0x8D);
  spi_send_recv(0x14);

  spi_send_recv(0xD9);
  spi_send_recv(0xF1);

  DISPLAY_ACTIVATE_VBAT;
  quicksleep(10000000);

  spi_send_recv(0xA1);
  spi_send_recv(0xC8);

  spi_send_recv(0xDA);
  spi_send_recv(0x20);

  spi_send_recv(0xAF);
}
//FROM LAB 3
void display_string(int line, char *s)
{
  int i;
  if (line < 0 || line >= 4)
    return;
  if (!s)
    return;

  for (i = 0; i < 16; i++)
    if (*s)
    {
      textbuffer[line][i] = *s;
      s++;
    }
    else
      textbuffer[line][i] = ' ';
}
 //FROM LAB 3
/* Helper function, local to this file.
   Converts a number to hexadecimal ASCII digits. */
static void num32asc(char *s, int n)
{
  int i;
  for (i = 28; i >= 0; i -= 4)
    *s++ = "0123456789ABCDEF"[(n >> i) & 15];
}

//FROM LAB 3
#define ITOA_BUFSIZ (24)
char *itoaconv(int num)
{
  register int i, sign;
  static char itoa_buffer[ITOA_BUFSIZ];
  static const char maxneg[] = "-2147483648";

  itoa_buffer[ITOA_BUFSIZ - 1] = 0; /* Insert the end-of-string marker. */
  sign = num;                       /* Save sign. */
  if (num < 0 && num - 1 > 0)       /* Check for most negative integer */
  {
    for (i = 0; i < sizeof(maxneg); i += 1)
      itoa_buffer[i + 1] = maxneg[i];
    i = 0;
  }
  else
  {
    if (num < 0)
      num = -num;        /* Make number positive. */
    i = ITOA_BUFSIZ - 2; /* Location for first ASCII digit. */
    do
    {
      itoa_buffer[i] = num % 10 + '0'; /* Insert next digit. */
      num = num / 10;                  /* Remove digit from number. */
      i -= 1;                          /* Move index to next empty position. */
    } while (num > 0);
    if (sign < 0)
    {
      itoa_buffer[i] = '-';
      i -= 1;
    }
  }
  /* Since the loop always sets the index i to the next empty position,
   * we must add 1 in order to return a pointer to the first occupied position. */
  return (&itoa_buffer[i + 1]);
}

// TAKEN FROM LAB 3 (renamed)
void text_update(void)
{
  int i, j, k;
  int c;
  for (i = 0; i < 4; i++)
  {
    DISPLAY_CHANGE_TO_COMMAND_MODE;
    spi_send_recv(0x22);
    spi_send_recv(i);

    spi_send_recv(0x0);
    spi_send_recv(0x10);

    DISPLAY_CHANGE_TO_DATA_MODE;

    for (j = 0; j < 16; j++)
    {
      c = textbuffer[i][j];
      if (c & 0x80)
        continue;

      for (k = 0; k < 8; k++)
        spi_send_recv(font[c * 8 + k]);
    }
  }
}

// erik
// clear_display sets every pixel on the screen equal to zero. Which is necessary before updating the screen.

#define MAXPIXELS_Y 32
#define MAXPIXELS_X 128
#define TOTALPAGES 4

void clear_display(void)
{
  int i, j;
  for (i = 0; i < MAXPIXELS_Y; i++) // for every y-cordinate
  {
    for (j = 0; j < MAXPIXELS_X; j++) // for every x-coordinate
    {
      Pixelsondisplay[i][j] = 0;
    }
  }
}
// WRITTEN BY ISAK AHLBERG AND ERIK LOSIN
void update_display(void) 
{
  int i, j, k, temp_display;
  for (i = 0; i < TOTALPAGES; i++) // per page
  {
    DISPLAY_CHANGE_TO_COMMAND_MODE;

    spi_send_recv(0x22); // Set page command
    spi_send_recv(i);    // page #

    spi_send_recv(0x0);  // set low nybble of column
    spi_send_recv(0x10); // set high nybble of column

    DISPLAY_CHANGE_TO_DATA_MODE;
    for (j = 0; j < MAXPIXELS_X; j++) // 128 x-pixels wide
    {
      temp_display = Pixelsondisplay[i * (MAXPIXELS_Y / TOTALPAGES)][j];
      for (k = 0; k < (MAXPIXELS_Y / TOTALPAGES); k++) // 8 y-pixels per page
      {
        temp_display |= Pixelsondisplay[i * (MAXPIXELS_Y / TOTALPAGES) + k][j] << k;
      }
      spi_send_recv(temp_display);
    }
  }
}
// WRITTEN BY ERIK LOSIN AND ISAK AHLBERG
void birdtodisplay(int y)
{
  int i, j;
  for (j = y; j < y + 5; j++)
  {
    for (i = 29; i < 35; i++)
    {
      Pixelsondisplay[j][i] |= flappybird[j - y][i - 29];
    }
  }
}
// WRITTEN BY ERIK LOSIN
void obstacletodisplay(int x)
{
  int i, j;
  for (j = 0; j < 32; j++)
  {
    for (i = ((128 - x) > 0) ? (128 - x) : 0; i < 128; i++)
    {
      Pixelsondisplay[j][i] |= ObstacleCombined2[j][(i + x) % 128];
    }
  }
}
// WRITTEN BY ISAK AHLBERG
int EnterButton()
{
  static int ButtonEnter = 0;
  int getbuttons = getbtns();

  if (getbuttons & 0b100 && !ButtonEnter)
  {
    ButtonEnter = 1;
    return 1;
  }
  else if (!(getbuttons & 0b100) && ButtonEnter)
  {
    ButtonEnter = 0;
  }
  return 0;
}

// WRITTEN BY ERIK LOSIN

int score_check()
{
  static int Prev_Check = 0;

  if (Pixelsondisplay[0][28] && !Prev_Check)
  {
    Prev_Check = 1;
    return 1;
  }
  else if (!(Pixelsondisplay[0][28]) && Prev_Check)
  {
    Prev_Check = 0;
  }
  return 0;
}

char *to_string(int score)
{
  return (itoaconv(score));
}

void clear_bug_display(void)
{
  int i, j;
  for (i = 0; i < MAXPIXELS_Y; i++) // for every y-cordinate
  {
    for (j = 0; j < 4; j++) // bug portion
    {
      Pixelsondisplay[i][j] = 0;
    }
  }
}