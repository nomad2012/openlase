/*
SAMPLE ASTEROIDS GAME
Developed by Chris DeLeon
November 2009
Created as material for use on http://gamedevlessons.com/
To be included in Free Lessons Vol. 9 - find all past editions at:
  http://gamedevlessons.com/?page=free

This code is written with clarity and understandability to a beginner in mind.
It is not intended to represent the most optimized or most organized way
that the code could possibly be prepared.

Allegro is required to compile this application. Find instructions here:
http://gamedevlessons.com/lessons/letter5.html
(All software needed to compile this, for Mac/PC/Linux, is free!)

Files that are part of this project should be:
  core.cpp
  core.h
  main.cpp

Feel free to make changes, experiment, and distribute. Please leave this
comment at the top of the file, since it points back to my instructional
resources. My mission is to help people - your help in this is appreciated!
*/

#include "core.h"

BITMAP *screenBuffer;

void techInit(){
     allegro_init(); // all Allegro programs need this
     set_color_depth(32);
     set_gfx_mode(GFX_AUTODETECT, 1024, 768, 0, 0);
     
     screenBuffer = create_bitmap(SCREEN_W, SCREEN_H);
     
     install_keyboard();
     install_mouse();
     /* add other initializations here*/
}

void updateScreen() {
     // draw the game's action to the screen
     blit(screenBuffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
}
